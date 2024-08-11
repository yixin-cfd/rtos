#include "TARDIS_OS.h"
#include "ARMCM4.h"
Task * currentTask;
Task * nextTask;
Task * idleTaskPtr;
uint8_t schedLockCount;	//��������������ֵ

uint32_t tickCount;		//ϵͳʱ�ӽ��ķ�������
uint32_t idleCount;		//�����������ֵ
uint32_t idleMaxCount;	//��������������ֵ
uint32_t FirstSec;		//��ǰ����ֵ�ǲ��ǵ�һ��

Bitmap taskPriorityBitamp;	//���ȼ�λͼ
List taskTable[TARDIS_OS_PRIORITY_COUNT];

List taskDelayedList;		//������ʱ����


static void initCpuUsageState(void);		//Cpuʹ������ر�����ʼ�� 
static void checkCpuUsage(void);			//���Cpu��ʹ����
static void cpuUsageSynWithSysTick (void);	//ʵ��ʱ��ͬ��������������ʼ����ʱʱ�ӽ��ĸտ�ʼ

void TaskSchedRdy(Task * task){
	ListAddHead(&taskTable[task->priority],&task->linkNode);
	BitmapSet(&taskPriorityBitamp,task->priority); 
}
void TaskSchedUnRdy(Task * task){								//������Ӿ���״̬���Ƴ�
	ListRemoveNode(&taskTable[task->priority],&task->linkNode);	//������Ӿ����������Ƴ�	
	if(GetListCount(&taskTable[task->priority])==0){			//��������ȼ��Ķ���û���������λͼ���
		BitmapClear(&taskPriorityBitamp,task->priority); 		//���λͼ���
	}
}

void TaskSchedRemove(Task * task){								//������Ӿ����������Ƴ�
	ListRemoveNode(&taskTable[task->priority],&task->linkNode);	//�Ӿ����������Ƴ�	
	if(GetListCount(&taskTable[task->priority])==0){			//��������ȼ��Ķ���û���������λͼ���
		BitmapClear(&taskPriorityBitamp,task->priority); 		//���λͼ���
	}
}
Task * TaskHighestReady(void){
	uint32_t hightPriority = GetBitmapFirstSet(&taskPriorityBitamp);
	return NodeParent(ListFirstNode(&taskTable[hightPriority]),Task,linkNode);
}
void TimeTaskWait(Task * task,uint32_t ticks){
	task->delayTicks = ticks;
	ListAddHead(&taskDelayedList,&task->delayNode);
	task->state |= TARDIS_OS_TASK_STATE_DELAYED;

}

void TimeTaskWakeUp(Task *task){						//����ʱ�����л�������		
	ListRemoveNode(&taskDelayedList,&(task->delayNode));//����ʱ�������Ƴ�����
	task->state &= ~TARDIS_OS_TASK_STATE_DELAYED;		//�޸�����״̬
}
void TimeTaskRemove(Task *task){						//����ʱ�������Ƴ�����
	ListRemoveNode(&taskDelayedList,&(task->delayNode));//�Ƴ�����
}
void TaskSched(void){
	Task * tempTask;
	uint32_t status = TaskEnterCritical();
	
	if(schedLockCount > 0){					//���������������ֱ���˳�������ִ�е�ǰ����
		TaskExitCritical(status);
		return;
	}
	tempTask = TaskHighestReady();
	if( currentTask != tempTask){
		nextTask = tempTask;
		TaskSwitch();
	}
	TaskExitCritical(status);
}

void TaskSchedInit(void){	
	int i;	
	schedLockCount = 0;
	BitmapInit(&taskPriorityBitamp);
	for(i = 0;i< TARDIS_OS_PRIORITY_COUNT;i++){
		ListInit(&taskTable[i]);
	}
}
void TaskSchedDisable(void){				//����������
	uint32_t status = TaskEnterCritical();
	if(schedLockCount < 255){
		schedLockCount++;
	}
	TaskExitCritical(status);
}
void TaskSchedEnable(void){					//����������
	uint32_t status = TaskEnterCritical();
	if(schedLockCount > 0){
		if(--schedLockCount == 0){
			TaskSched();
		}
	}
	TaskExitCritical(status);
}



void taskDelayedInit(void){
	ListInit(&taskDelayedList);
}

void TimeTickInit(void)		//ʱ�Ӽ���ֵ��ʼ��
{
	tickCount = 0;
}

void TaskSystemTickHandler(){
	Node *node;
	uint32_t status = TaskEnterCritical();
	for(node = taskDelayedList.headNode.nextNode;node != &(taskDelayedList.headNode);node = node->nextNode){
		Task * task = NodeParent(node,Task,delayNode);
		if(--task->delayTicks == 0){
			
			if(task->waitEvent){		//��������ٵȴ��¼�
				EventRemoveTask(task, (void *)0, ErrorTimeout);	//�ӵȴ��¼�������ɾ�����ȴ����Ϊ��ʱ 
			}
			
			TimeTaskWakeUp(task);
			
			TaskSchedRdy(task);
		}
	}
	
	if(--currentTask->slice==0){
		if(GetListCount(&taskTable[currentTask->priority])!=0){
			ListRemoveFirst(&taskTable[currentTask->priority]);
			ListAddTail(&taskTable[currentTask->priority],&currentTask->linkNode);
			
			currentTask->slice = TARDIS_OS_SLICE_MAX;
		}
	}
	tickCount++;		//����ֵ����һ
	checkCpuUsage();	//���Cpu��ʹ����
	TaskExitCritical(status);
	
	TimerModuleTickNotify();	//��ʱ���жϴ���
	
	TaskSched();
}
static float cpuUsage;
static uint32_t enableCpuUsageState;
static void initCpuUsageState(void)	//Cpuʹ������ر�����ʼ�� 
{
	FirstSec = 1;	//�ǵ�һ��
	idleCount = 0;
	idleMaxCount = 0;
	cpuUsage = 0.0f;
	enableCpuUsageState = 0;
}
static void checkCpuUsage(void)		//���Cpu��ʹ����
{
	if(enableCpuUsageState == 0){	//��һ�ν���ʱ���жϣ���û��ͬ��
		enableCpuUsageState = 1;	//ͬ��
		tickCount = 0;				//��ʼ����
		return;
	}
	if(FirstSec && (tickCount == TICKS_PER_SEC)){	//��һ��
		idleMaxCount = idleCount;
		idleCount = 0;
		TaskSchedEnable();		//�������
		FirstSec = 0;			//���ǵ�һ��
	}
	else if((FirstSec == 0 )&&(tickCount % TICKS_PER_SEC == 0)){
		cpuUsage = 100 - (idleCount * 100.0 / idleMaxCount);
		idleCount = 0;
	}
}

static void cpuUsageSynWithSysTick (void)	//ʵ��ʱ��ͬ��������������ʼ����ʱʱ�ӽ��ĸտ�ʼ
{
	while(enableCpuUsageState == 0);
}
float CpuUsageGet(void){	//��ȡCPUʹ����
	float usage = 0;
	uint32_t status = TaskEnterCritical();
	usage = cpuUsage;
	TaskExitCritical(status);
	return usage;
}

//�����������
Task idleTask;
TaskStack idleTaskEnv[TARDIS_OS_IDLETASK_STACK_SIZE];
void idleTaskEntry(void *param){
	
	TaskSchedDisable();	//��������������ʼʱһ������ֻ���ڿ�������������
	
	InitApp();
	TimerInitTask();	//��ʱ����ʼ�� & ����һ����ʱ������
	
	SetSysTickPeriod(TARDIS_OS_SYSTICK_MS);	//����ϵͳ��ʱ��
	
	cpuUsageSynWithSysTick();		//ʱ��ͬ��
	
	for(;;){
		uint32_t status = TaskEnterCritical();
		idleCount++;
		TaskExitCritical(status);
	}
}
 int main(void){

	TaskSchedInit();		//���ȼ����г�ʼ��
	taskDelayedInit();		//��ʱ���г�ʼ��
	TimerModuleInit();		//��ʱ����ʼ��
	TimeTickInit();			//ʱ�Ӽ���ֵ��ʼ��
	initCpuUsageState();	//Cpuʹ������ر�����ʼ�� 
	TaskInit(&idleTask,idleTaskEntry,(void *)0x00,TARDIS_OS_PRIORITY_COUNT - 1,idleTaskEnv, sizeof(idleTaskEnv));
	idleTaskPtr = &idleTask;
	nextTask = TaskHighestReady();
	TaskRunFirst();
	
	return 0;
}


