#include "TARDIS_OS.h"
#include "ARMCM4.h"
Task * currentTask;
Task * nextTask;
Task * taskTable[2];
Task * idleTaskPtr;
uint8_t schedLockCount;


void TaskSched(void){
	uint32_t status = TaskEnterCritical();
	
	if(schedLockCount > 0){					//如果有锁，调度器直接退出，继续执行当前任务
		TaskExitCritical(status);
		return;
	}
	
	
	if(currentTask == idleTaskPtr){
		if(taskTable[0]->delayTicks==0)
			nextTask = taskTable[0];
		else if(taskTable[1]->delayTicks == 0)
			nextTask = taskTable[1];
		else {
			TaskExitCritical(status);
			return;
		}
	}
	else{
		if(currentTask == taskTable[0]){
			if(taskTable[1]->delayTicks == 0){
				nextTask = taskTable[1];
			}
			else if(currentTask->delayTicks!=0){
				nextTask = idleTaskPtr;
			}
			else{
				TaskExitCritical(status);
				return;
			}
		}
		else{	
			if(taskTable[0]->delayTicks == 0){
				nextTask = taskTable[0];
			}
			else if(currentTask->delayTicks!=0){
				nextTask = idleTaskPtr;
			}
			else {
				TaskExitCritical(status);
				return;
			}
		}
	}
	TaskSwitch();
	TaskExitCritical(status);
}

void TaskSchedInit(void){		
	schedLockCount = 0;
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
	TaskExitCritical(status);
	TaskSched();
}
void TaskSystemTickHandler(){
	int i;
	uint32_t status = TaskEnterCritical();
	for(i=0;i<2;i++){
		if(taskTable[i]->delayTicks > 0){
			taskTable[i]->delayTicks--;
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
int firstSet;
//定义任务对应的功能函数
int task1Flag;
void task1Entry(void * param){
	
	Bitmap bitmap;									//位图测试
	int i;
	
	SetSysTickPeriod(10);
	
	BitmapInit(&bitmap);
	for(i = GetBitmapPosCount() - 1;i >= 0;i--){
		BitmapSet(&bitmap, i);
		
		firstSet = GetBitmapFirstSet(&bitmap);
	}
	
	for(i = 0;i < GetBitmapPosCount();i++){
		BitmapClear(&bitmap, i);
		firstSet = GetBitmapFirstSet(&bitmap);
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
void TaskInit(Task *task,void (*entry)(void *),void *param,TaskStack *stack){
	
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

	task->stack = stack;
	task->delayTicks = 0;
	//stack 指针刚开始处于堆栈最底端
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

	TaskInit(&Task1,task1Entry,(void *)0x00,&Task1Env[24]);
	TaskInit(&Task2,task2Entry,(void *)0x00,&Task2Env[24]);
	TaskInit(&idleTask,idleTaskEntry,(void *)0x00,&idleTaskEnv[24]);
	
	taskTable[0] = &Task1;
	taskTable[1] = &Task2;
	idleTaskPtr = &idleTask;
	
	nextTask = taskTable[0];
	TaskRunFirst();
	
	return 0;
}


