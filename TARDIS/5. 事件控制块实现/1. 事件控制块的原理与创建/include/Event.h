#ifndef _EVENT_H
#define _EVENT_H

#include "DS_Lib.h"

typedef enum _EventType{		//�¼�����
	EventTypeUnknow
}EventType;

typedef struct _Event{			
	EventType type;		//�¼�����
	List waitList;		//�ȴ��б�
}Event;

void EventInit(Event * event , EventType type);		//�¼���ʼ������

#endif


