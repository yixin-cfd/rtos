#include "TARDIS_OS.h"
#include "ARMCM4.h"
Task * currentTask;
Task * nextTask;
Task * idleTaskPtr;
uint8_t schedLockCount;

Bitmap taskPriorityBitamp;
Task * taskTable[TARDIS_OS_PRIORITY_COUNT];

List taskDelayedList;		//������ʱ����

void TaskSchedRdy(Task * task){
	taskTable[task->priority] = task;
	BitmapSet(&taskPriorityBitamp,task->priority); 
}
void TaskSchedUnRdy(Task * task){
	taskTable[task->priority] = (Task *)0;
	BitmapClear(&taskPriorityBitamp,task->priority); 
}
Task * TaskHighestReady(void){
	uint32_t hightPriority = GetBitmapFirstSet(&taskPriorityBitamp);
	return taskTable[hightPriority];
}
void TimeTaskWait(Task * task,uint32_t ticks){
	task->delayTicks = ticks;
	ListAddHead(&taskDelayedList,&task->delayNode);
	task->state |= TARDIS_OS_TASK_STATE_DELAYED;

}

void TimeTaskWakeUp(Task *task){
	ListRemoveNode(&taskDelayedList,&(task->delayNode));
	task->state &= ~TARDIS_OS_TASK_STATE_DELAYED;
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
	schedLockCount = 0;
	BitmapInit(&taskPriorityBitamp);
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

//���ö�ʱ��
void SetSysTickPeriod(uint32_t ms){
	SysTick->LOAD = ms * SystemCoreClock / 1000 - 1;
	NVIC_SetPriority(SysTick_IRQn,(1<<__NVIC_PRIO_BITS)-1);
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk|
					SysTick_CTRL_TICKINT_Msk|
					SysTick_CTRL_ENABLE_Msk;
}
void TaskDelay(uint32_t delay){
	uint32_t status = TaskEnterCritical();
	TimeTaskWait(currentTask,delay);
	TaskSchedUnRdy(currentTask);
	TaskExitCritical(status);
	TaskSched();
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
			TimeTaskWakeUp(task);
			
			TaskSchedRdy(task);
		}
	}
	TaskExitCritical(status);
	TaskSched();
}
void SysTick_Handler(){
	TaskSystemTickHandler();
}

Task Task1;				//������������
Task Task2;

TaskStack Task1Env[24];	//����������ջ
TaskStack Task2Env[24];

int ShareCount;

//���������Ӧ�Ĺ��ܺ���
int task1Flag;

void task1Entry(void * param){
	

	SetSysTickPeriod(10);

	for(;;){	
		task1Flag = 0;
		TaskDelay(1);
		task1Flag = 1;
		TaskDelay(1);
	}
}
int task2Flag;
void task2Entry(void * param){

	for(;;){
		task2Flag = 1;
		TaskDelay(1);
		task2Flag = 0;
		TaskDelay(1);
	}
}
//���������ʼ����
void TaskInit(Task *task,void (*entry)(void *),void *param,uint32_t priority,TaskStack *stack){
	
	/*********�Զ�����****************/
	*(--stack) = (unsigned long)(1<<24);
	*(--stack) = (unsigned long)entry;
	*(--stack) = (unsigned long)0x14;
	*(--stack) = (unsigned long)0x12;
	*(--stack) = (unsigned long)0x3;
	*(--stack) = (unsigned long)0x2;
	*(--stack) = (unsigned long)0x1;
	*(--stack) = (unsigned long)param;       //R0
	/*********�Զ�����****************/
	*(--stack) = (unsigned long)0x11;
	*(--stack) = (unsigned long)0x10;
	*(--stack) = (unsigned long)0x9;
	*(--stack) = (unsigned long)0x8;
	*(--stack) = (unsigned long)0x7;
	*(--stack) = (unsigned long)0x6;
	*(--stack) = (unsigned long)0x5;
	*(--stack) = (unsigned long)0x4;

	task->stack = stack;			//stack ָ��տ�ʼ���ڶ�ջ��׶�
	task->delayTicks = 0;
	task->priority = priority;
	
	task->state = TARDIS_OS_TASK_STATE_RDY;
	NodeInit(&(task->delayNode));
	
	taskTable[priority] = task;
	BitmapSet(&taskPriorityBitamp, priority);
}


//�����������
Task idleTask;
TaskStack idleTaskEnv[24];
void idleTaskEntry(void *param){
	for(;;){
	
	}
}
int main(void){
	//�����ʼ��
	TaskSchedInit();		//��������ʼ��
	taskDelayedInit();
	TaskInit(&Task1,task1Entry,(void *)0x00,0,&Task1Env[24]);
	TaskInit(&Task2,task2Entry,(void *)0x00,1,&Task2Env[24]);
	TaskInit(&idleTask,idleTaskEntry,(void *)0x00,TARDIS_OS_PRIORITY_COUNT - 1,&idleTaskEnv[24]);
	
	taskTable[0] = &Task1;
	taskTable[1] = &Task2;
	idleTaskPtr = &idleTask;
	
	nextTask = TaskHighestReady();
	TaskRunFirst();
	
	return 0;
}


