#include "TARDIS_OS.h"
#include "ARMCM4.h"
Task * currentTask;
Task * nextTask;
Task * idleTaskPtr;
uint8_t schedLockCount;

Bitmap taskPriorityBitamp;
List taskTable[TARDIS_OS_PRIORITY_COUNT];

List taskDelayedList;		//������ʱ����

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
	
	TaskExitCritical(status);
	
	TimerModuleTickNotify();	//��ʱ���жϴ���
	
	TaskSched();
}



//�����������
Task idleTask;
TaskStack idleTaskEnv[TARDIS_OS_IDLETASK_STACK_SIZE];
void idleTaskEntry(void *param){
	for(;;){
	
	}
}
 int main(void){

	TaskSchedInit();		//���ȼ����г�ʼ��
	taskDelayedInit();		//��ʱ���г�ʼ��
	TimerModuleInit();		//��ʱ����ʼ��
	InitApp();
	TaskInit(&idleTask,idleTaskEntry,(void *)0x00,TARDIS_OS_PRIORITY_COUNT - 1,idleTaskEnv, sizeof(idleTaskEnv));
	idleTaskPtr = &idleTask;
	nextTask = TaskHighestReady();
	TaskRunFirst();
	
	return 0;
}


