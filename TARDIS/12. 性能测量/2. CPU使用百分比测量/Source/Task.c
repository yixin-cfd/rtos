#include "TARDIS_OS.h"
#include <string.h>
//定义任务初始函数
void TaskInit(Task *task,void (*entry)(void *),void *param,uint32_t priority,TaskStack *stack, uint32_t size){
	
	uint32_t * stackTop;	//堆栈末端指针
	task->stackBase = stack;//堆栈其实地址
	task->stackSize = size;	//堆栈大小
	memset(stack, 0, size);	//堆栈清零
	
	stackTop = stack + size / sizeof(TaskStack);
	/*********自动保存****************/
	*(--stackTop) = (unsigned long)(1<<24);
	*(--stackTop) = (unsigned long)entry;
	*(--stackTop) = (unsigned long)0x14;
	*(--stackTop) = (unsigned long)0x12;
	*(--stackTop) = (unsigned long)0x3;
	*(--stackTop) = (unsigned long)0x2;
	*(--stackTop) = (unsigned long)0x1;
	*(--stackTop) = (unsigned long)param;       //R0
	/*********自动保存****************/
	*(--stackTop) = (unsigned long)0x11;
	*(--stackTop) = (unsigned long)0x10;
	*(--stackTop) = (unsigned long)0x9;
	*(--stackTop) = (unsigned long)0x8;
	*(--stackTop) = (unsigned long)0x7;
	*(--stackTop) = (unsigned long)0x6;
	*(--stackTop) = (unsigned long)0x5;
	*(--stackTop) = (unsigned long)0x4;

	task->slice = TARDIS_OS_SLICE_MAX;		//时间片大小
	task->stack = stackTop;					//stack 指针刚开始处于堆栈最底端
	task->delayTicks = 0;					//延时时间
	task->priority = priority;				//优先级
	task->state = TARDIS_OS_TASK_STATE_RDY;	//任务状态
	task->suspendCount = 0;					//挂起计数器
	
	task->clean = ( void (*) (void *))0;	//任务删除函数
	task->cleanParam = (void *)0;				//删除参数
	task->requestDeleteFlag = 0;			//请求删除标记
	
	NodeInit(&(task->delayNode));			//初始化任务结构体的延时节点
	NodeInit(&(task->linkNode));			//初始化任务结构体的链接到优先级队列的节点
	
	TaskSchedRdy(task);	//将任务的链接节点添加到对应的优先级队列的头部;设置对应的位图，说明这个优先级的有任务存在
}

//任务挂起函数
void TaskSuspend(Task * task){
	uint32_t status = TaskEnterCritical();
	if( !(task->state & TARDIS_OS_TASK_STATE_DELAYED) ){	//如果是延时状态不能挂起	
		if(++task->suspendCount <= 1){						//第一次挂起
			task->state |= TARDIS_OS_TASK_STATE_SUSPEND;	//更新任务状态
			TaskSchedUnRdy(task);							//从就绪队列中移除 & 修改位图
			if(task == currentTask){						//如果这个任务是当前任务的话进行任务调度
				TaskSched();								//任务调度
			}
		}
	}
	TaskExitCritical(status);
}

//任务唤醒函数
void TaskWakeUp(Task * task){
	uint32_t status = TaskEnterCritical();
	if( task->state & TARDIS_OS_TASK_STATE_SUSPEND ){	//判断任务是不是挂起状态
		if( --task->suspendCount == 0){					//如果挂起计数器为零
			task->state &=  ~TARDIS_OS_TASK_STATE_SUSPEND;		//清除挂起状态
			TaskSchedRdy(task);							//插入就绪队列
			TaskSched();								//任务调度
		}
	}
	TaskExitCritical(status);
}

void TaskSetCleanCallFunc(Task * task,void (*clean)(void * param),void * param){	//设置任务删除函数
	task->clean = clean;
	task->cleanParam = param;
}

void TaskForceDelete(Task * task){			//将任务强制删除
	uint32_t status = TaskEnterCritical();
	if( task->state & TARDIS_OS_TASK_STATE_DELAYED){		//如果任务处于延时状态
		TimeTaskRemove(task);
	}
	else if( !(task->state & TARDIS_OS_TASK_STATE_SUSPEND)){//如果不在挂起状态，那么就在就绪状态
		TaskSchedRemove(task);
	}
	if( task->clean ){					//如果定义了删除函数
		task->clean(task->cleanParam);	//进行任务删除
	}
	if( currentTask == task){			//如果被删除的任务是当前任务
		TaskSched();					//进行任务调度
	}
	TaskExitCritical(status);
}

void TaskRequestDelete(Task * task){			//请求删除任务
	uint32_t status = TaskEnterCritical();
	task->requestDeleteFlag = 1;				//设置请求标志位
	TaskExitCritical(status);
}

uint8_t TaskIsRequestDeleted (void){			//检查当前任务是否需要删除
	uint8_t deleted = 0;
	uint32_t status = TaskEnterCritical();
	deleted = currentTask->requestDeleteFlag;	
	TaskExitCritical(status);
	return deleted;
}

void TaskDeleteSelf(void){		//当前任务删除自己
	uint32_t status = TaskEnterCritical();
	TaskSchedRemove(currentTask);						//将当前任务从就绪队列中移除
	if( currentTask->clean ){							//如果定义了删除函数
		currentTask->clean(currentTask->cleanParam);	//进行任务删除
	}
	TaskExitCritical(status);
	TaskSched();
}

void GetTaskInfo(Task * task, TaskInfo * info){		//任务状态查询函数
	uint32_t * stackEnd;	//堆栈末端地址
	uint32_t status = TaskEnterCritical();

	info->delayTicks = task->delayTicks;
	info->priority = task->priority;
	info->slice = task->slice;
	info->state = task->state;
	info->suspendCount = task->suspendCount;
	info->stackSize = task->stackSize;
	
	info->stackFree = 0;
	stackEnd = task->stackBase;
	while( (*stackEnd++ == 0) && (stackEnd <= task->stackBase + task->stackSize / sizeof(TaskStack)))
	{
		info->stackFree++;
	}
	info->stackFree *= sizeof(TaskStack);
	TaskExitCritical(status);
}

