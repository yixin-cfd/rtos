#include "Event.h"


void EventInit(Event * event , EventType type) {	//事件初始化函数
	
	event->type = type;
	ListInit(&event->waitList);
	
}


