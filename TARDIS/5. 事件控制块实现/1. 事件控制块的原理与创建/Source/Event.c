#include "Event.h"


void EventInit(Event * event , EventType type) {	//�¼���ʼ������
	
	event->type = type;
	ListInit(&event->waitList);
	
}


