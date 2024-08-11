#include "TARDIS_OS.h"
#include "ARMCM4.h"
Task * currentTask;
Task * nextTask;
Task * idleTaskPtr;
uint8_t schedLockCount;

Bitmap taskPriorityBitamp;
Task * taskTable[TARDIS_OS_PRIORITY_COUNT];

Task * TaskHighestReady(void){
	uint32_t hightPriority = GetBitmapFirstSet(&taskPriorityBitamp);
	return taskTable[hightPriority];
}
void TaskSched(void){
	Task * tempTask;
	uint32_t status = TaskEnterCritical();
	
	if(schedLockCount > 0){					//如果有锁，调度器直接退出，继续执行当前任务
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
void TaskSchedDisable(void){				//调度器加锁
	uint32_t status = TaskEnterCritical();
	if(schedLockCount < 255){
		schedLockCount++;
	}
	TaskExitCritical(status);
}
void TaskSchedEnable(void){					//调度器解锁
	uint32_t status = TaskEnterCritical();
	if(schedLockCount > 0){
		if(--schedLockCount == 0){
			TaskSched();
		}
	}
	TaskExitCritical(status);
}

//配置定时器
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
	currentTask->delayTicks = delay;
	BitmapClear(&taskPriorityBitamp,currentTask->priority);		//将当前任务从就绪队列中删除
	TaskExitCritical(status);
	TaskSched();
}
void TaskSystemTickHandler(){
	int i;
	uint32_t status = TaskEnterCritical();
	for(i=0;i< TARDIS_OS_PRIORITY_COUNT;i++){
		if(taskTable[i]->delayTicks > 0){
			taskTable[i]->delayTicks--;
		}
		else{//加入到就绪队列中
			BitmapSet(&taskPriorityBitamp, i);
		}
	}

	TaskExitCritical(status);
	TaskSched();
}
void SysTick_Handler(){
	TaskSystemTickHandler();
}

Task Task1;				//定义两个任务
Task Task2;

TaskStack Task1Env[24];	//定义两个堆栈
TaskStack Task2Env[24];

int ShareCount;

//定义任务对应的功能函数
int task1Flag;
List list;
Node node[8];
void task1Entry(void * param){
	
	int i;
	SetSysTickPeriod(10);
	ListInit(&list);
	for(i = 0;i<8;i++){
		NodeInit(&node[i]);
		ListAddHead(&list,&node[i]);
	}
	for(i = 0;i<8;i++){
		ListRemoveFirst(&list);
	}
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
//定义任务初始函数
void TaskInit(Task *task,void (*entry)(void *),void *param,uint32_t priority,TaskStack *stack){
	
	/*********自动保存****************/
	*(--stack) = (unsigned long)(1<<24);
	*(--stack) = (unsigned long)entry;
	*(--stack) = (unsigned long)0x14;
	*(--stack) = (unsigned long)0x12;
	*(--stack) = (unsigned long)0x3;
	*(--stack) = (unsigned long)0x2;
	*(--stack) = (unsigned long)0x1;
	*(--stack) = (unsigned long)param;       //R0
	/*********自动保存****************/
	*(--stack) = (unsigned long)0x11;
	*(--stack) = (unsigned long)0x10;
	*(--stack) = (unsigned long)0x9;
	*(--stack) = (unsigned long)0x8;
	*(--stack) = (unsigned long)0x7;
	*(--stack) = (unsigned long)0x6;
	*(--stack) = (unsigned long)0x5;
	*(--stack) = (unsigned long)0x4;

	task->stack = stack;			//stack 指针刚开始处于堆栈最底端
	task->delayTicks = 0;
	task->priority = priority;
	
	
	taskTable[priority] = task;
	BitmapSet(&taskPriorityBitamp, priority);
}


//定义空闲任务
Task idleTask;
TaskStack idleTaskEnv[24];
void idleTaskEntry(void *param){
	for(;;){
	
	}
}
int main(void){
	//任务初始化
	TaskSchedInit();		//调度锁初始化

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


