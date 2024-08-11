#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#include "Event.h"


typedef struct _Semaphore{
	Event event;		//�¼����ƿ�
	uint32_t count;		//��ǰ����ֵ
	uint32_t maxCount;	//������ֵ
}Semaphore;

void SemaphoreInit(Semaphore * sem, uint32_t startCount, uint32_t maxCount);//�ź�����ʼ�� 	
uint32_t SemWait (Semaphore * sem, uint32_t waitTicks);		//�ȴ��ź���
uint32_t SemNoWait(Semaphore * sem);						//����Ҫ�ȴ��ź���
void SemNotify(Semaphore * sem);							//�ź������ͷ�
#endif

