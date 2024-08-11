#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#include "Event.h"


typedef struct _Semaphore{
	Event event;		//�¼����ƿ�
	uint32_t count;		//��ǰ����ֵ
	uint32_t maxCount;	//������ֵ
}Semaphore;

void SemaphoreInit(Semaphore * sem, uint32_t startCount, uint32_t maxCount);//�ź�����ʼ�� 	

#endif

