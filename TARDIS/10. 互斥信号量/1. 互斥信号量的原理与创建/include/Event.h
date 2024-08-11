#ifndef _EVENT_H
#define _EVENT_H

#include "DS_Lib.h"
#include "Task.h"

typedef enum _EventType{		//事件类型
	EventTypeUnknow,	//未知
	EventTypeSem,		//信号量类型
	EventTypeMbox,		//邮箱类型
	EventTypeMemBlock,	//存储块类型 
	EventTypeFlagGroup,	//事件标志组
	EventTypeMutex		//互斥信号量
}EventType;

typedef struct _Event{			
	EventType type;		//事件类型
	List waitList;		//等待列表
}Event;

void EventInit(Event * event , EventType type);		//事件初始化函数

void EventWait(Event * event, Task * task, void * msg, uint32_t state, uint32_t timeout);	//等待事件

Task * EventWakeUp(Event * event, void * msg, uint32_t result);	//从事件控制块中唤醒任务

void EventWakeUpTask(Event * event, Task * task, void * msg, uint32_t result);//从事件控制块中唤醒特定任务

void EventRemoveTask(Task * task,void * msg, uint32_t result);	//将任务从事件控制块中删除，由时钟中断处理函数调用 

uint32_t EventRemoveAll( Event * event, void * msg, uint32_t result);	//清空事件控制块

uint32_t EventWaitCount(Event * event);	//获取等待事件任务的数量
#endif


