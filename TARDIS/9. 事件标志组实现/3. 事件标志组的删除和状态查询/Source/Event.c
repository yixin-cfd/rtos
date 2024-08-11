#include "TARDIS_OS.h"


void EventInit(Event * event , EventType type) {	//事件初始化函数
	
	event->type = type;
	ListInit(&event->waitList);
	
}

void EventWait(Event * event, Task * task, void * msg, uint32_t state, uint32_t timeout)	//等待事件
{
	uint32_t status = TaskEnterCritical();
	
	task->state |= state;	//后续拓展
	task->waitEvent = event;//任务等待控制块
	task->eventMsg = msg;	//事件的消息存放位置
	task->waitEventResult = NoneError;//清空事件结果
	
	TaskSchedUnRdy(task);	//从就绪队列中移除
	ListAddTail(&event->waitList,&task->linkNode);	//插入到等待队列的尾部
	
	if(timeout){			//如果设置超时
		TimeTaskWait(task,timeout);	//插入到延时队列中
	}
	
	TaskExitCritical(status);

}

Task * EventWakeUp(Event * event, void * msg, uint32_t result){		//从事件控制块中唤醒任务

	Node * node;
	Task *task = (Task *)0;
	uint32_t status = TaskEnterCritical();
	if( (node = ListRemoveFirst(&event->waitList)) != (Node *)0){	//若这个事件的等待队列中有任务
		task = NodeParent(node, Task, linkNode);					//获取链接节点对应的任务
		task->waitEvent = (Event *)0;								//初始化任务的事件控制块
		task->eventMsg = msg;										//设置任务的消息位置	
		task->waitEventResult = result;								//设置任务的事件结果
		task->state &= ~TARDIS_OS_TASK_WAIT_MASK;					//清除等待事件标标志
		
		if(task->delayTicks){		//如果还在延时队列中
			TimeTaskWakeUp(task);	//从延时队列中唤醒
		}
		TaskSchedRdy(task);			//插入到就绪队列中
	}
	
	TaskExitCritical(status);
	return task;
	
}
	
void EventWakeUpTask(Event * event, Task * task, void * msg, uint32_t result){//从事件控制块中唤醒特定任务


	uint32_t status = TaskEnterCritical();
	
	ListRemoveNode(&event->waitList, &task->linkNode);
	
	task->waitEvent = (Event *)0;								//初始化任务的事件控制块
	task->eventMsg = msg;										//设置任务的消息位置	
	task->waitEventResult = result;								//设置任务的事件结果
	task->state &= ~TARDIS_OS_TASK_WAIT_MASK;					//清除等待事件标标志
		
	if(task->delayTicks){		//如果还在延时队列中
		TimeTaskWakeUp(task);	//从延时队列中唤醒
	}
	TaskSchedRdy(task);			//插入到就绪队列中

	
	TaskExitCritical(status);

	
}

void EventRemoveTask(Task * task,void * msg, uint32_t result){		//将任务从事件控制块中删除，由时钟中断处理函数调用  

	uint32_t status = TaskEnterCritical();
	
	ListRemoveNode(&task->waitEvent->waitList,&task->linkNode);	//将任务从事件控制队列中删除
	
	task->waitEvent = (Event *)0;									//初始化任务的事件控制块
	task->eventMsg = msg;											//设置任务的消息位置	
	task->waitEventResult = result;									//设置任务的事件结果
	task->state &= ~TARDIS_OS_TASK_WAIT_MASK;						//清除等待事件标标志
	
	TaskExitCritical(status);
}

uint32_t EventRemoveAll( Event * event, void * msg, uint32_t result){	//清空事件控制块
	Node * node;
	uint32_t count = 0;
	uint32_t status = TaskEnterCritical();
	
	count = GetListCount(&event->waitList);
	
	while( (node = ListRemoveFirst(&event->waitList)) != (Node *)0){
		Task * task = NodeParent(node, Task, linkNode);				//获取链接节点对应的任务
		task->waitEvent = (Event *)0;								//初始化任务的事件控制块
		task->eventMsg = msg;										//设置任务的消息位置	
		task->waitEventResult = result;								//设置任务的事件结果
		task->state &= ~TARDIS_OS_TASK_WAIT_MASK;					//清除等待事件标标志
		
		if(task->delayTicks){		//如果还在延时队列中
			TimeTaskWakeUp(task);	//从延时队列中唤醒
		}
		TaskSchedRdy(task);			//插入到就绪队列中
	}
	
	TaskExitCritical(status);
	return count;
}

uint32_t EventWaitCount(Event * event){		//获取等待事件任务的数量
	uint32_t count = 0;
	
	uint32_t status = TaskEnterCritical();	 
	count = GetListCount(&event->waitList);
	TaskExitCritical(status);
	
	return count;
}


