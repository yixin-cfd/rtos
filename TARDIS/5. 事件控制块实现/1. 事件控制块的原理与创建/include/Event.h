#ifndef _EVENT_H
#define _EVENT_H

#include "DS_Lib.h"

typedef enum _EventType{		//事件类型
	EventTypeUnknow
}EventType;

typedef struct _Event{			
	EventType type;		//事件类型
	List waitList;		//等待列表
}Event;

void EventInit(Event * event , EventType type);		//事件初始化函数

#endif


