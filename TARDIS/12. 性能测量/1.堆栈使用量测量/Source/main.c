#include "TARDIS_OS.h"
#include "ARMCM4.h"
Task * currentTask;
Task * nextTask;
Task * idleTaskPtr;
uint8_t schedLockCount;

Bitmap taskPriorityBitamp;
List taskTable[TARDIS_OS_PRIORITY_COUNT];

List taskDelayedList;		//任务延时队列

void TaskSchedRdy(Task * task){
	ListAddHead(&taskTable[task->priority],&task->linkNode);
	BitmapSet(&taskPriorityBitamp,task->priority); 
}
void TaskSchedUnRdy(Task * task){								//将任务从就绪状态中移除
	ListRemoveNode(&taskTable[task->priority],&task->linkNode);	//将任务从就绪队列中移除	
	if(GetListCount(&taskTable[task->priority])==0){			//如果该优先级的队列没有任务，清除位图标记
		BitmapClear(&taskPriorityBitamp,task->priority); 		//清除位图标记
	}
}

void TaskSchedRemove(Task * task){								//将任务从就绪队列中移除
	ListRemoveNode(&taskTable[task->priority],&task->linkNode);	//从就绪队列中移除	
	if(GetListCount(&taskTable[task->priority])==0){			//如果该优先级的队列没有任务，清除位图标记
		BitmapClear(&taskPriorityBitamp,task->priority); 		//清除位图标记
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

void TimeTaskWakeUp(Task *task){						//从延时队列中唤醒任务		
	ListRemoveNode(&taskDelayedList,&(task->delayNode));//从延时队列中移除任务
	task->state &= ~TARDIS_OS_TASK_STATE_DELAYED;		//修改任务状态
}
void TimeTaskRemove(Task *task){						//从延时队列中移除任务
	ListRemoveNode(&taskDelayedList,&(task->delayNode));//移除任务
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
	int i;	
	schedLockCount = 0;
	BitmapInit(&taskPriorityBitamp);
	for(i = 0;i< TARDIS_OS_PRIORITY_COUNT;i++){
		ListInit(&taskTable[i]);
	}
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



void taskDelayedInit(void){
	ListInit(&taskDelayedList);
}



void TaskSystemTickHandler(){
	Node *node;
	uint32_t status = TaskEnterCritical();
	for(node = taskDelayedList.headNode.nextNode;node != &(taskDelayedList.headNode);node = node->nextNode){
		Task * task = NodeParent(node,Task,delayNode);
		if(--task->delayTicks == 0){
			
			if(task->waitEvent){		//如果任务再等待事件
				EventRemoveTask(task, (void *)0, ErrorTimeout);	//从等待事件队列中删除，等待结果为超时 
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
	
	TimerModuleTickNotify();	//软定时器中断处理
	
	TaskSched();
}



//定义空闲任务
Task idleTask;
TaskStack idleTaskEnv[TARDIS_OS_IDLETASK_STACK_SIZE];
void idleTaskEntry(void *param){
	for(;;){
	
	}
}
 int main(void){

	TaskSchedInit();		//优先级队列初始化
	taskDelayedInit();		//延时队列初始化
	TimerModuleInit();		//软定时器初始化
	InitApp();
	TaskInit(&idleTask,idleTaskEntry,(void *)0x00,TARDIS_OS_PRIORITY_COUNT - 1,idleTaskEnv, sizeof(idleTaskEnv));
	idleTaskPtr = &idleTask;
	nextTask = TaskHighestReady();
	TaskRunFirst();
	
	return 0;
}


