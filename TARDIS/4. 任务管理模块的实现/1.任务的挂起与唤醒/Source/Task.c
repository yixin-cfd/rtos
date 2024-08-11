#include "TARDIS_OS.h"

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

	task->slice = TARDIS_OS_SLICE_MAX;		//时间片大小
	task->stack = stack;					//stack 指针刚开始处于堆栈最底端
	task->delayTicks = 0;					//延时时间
	task->priority = priority;				//优先级
	task->state = TARDIS_OS_TASK_STATE_RDY;	//任务状态
	
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

