#ifndef _EVENT_H
#define _EVENT_H

#include "DS_Lib.h"
#include "Task.h"

typedef enum _EventType{		//�¼�����
	EventTypeUnknow
}EventType;

typedef struct _Event{			
	EventType type;		//�¼�����
	List waitList;		//�ȴ��б�
}Event;

void EventInit(Event * event , EventType type);		//�¼���ʼ������

void EventWait(Event * event, Task * task, void * msg, uint32_t state, uint32_t timeout);	//�ȴ��¼�

Task * EventWakeUp(Event * event, void * msg, uint32_t result);	//���¼����ƿ��л�������

void EventRemoveTask(Task * task,void * msg, uint32_t result);	//��������¼����ƿ���ɾ������ʱ���жϴ��������� 

uint32_t EventRemoveAll( Event * event, void * msg, uint32_t result);	//����¼����ƿ�

uint32_t EventWaitCount(Event * event);	//��ȡ�ȴ��¼����������
#endif


