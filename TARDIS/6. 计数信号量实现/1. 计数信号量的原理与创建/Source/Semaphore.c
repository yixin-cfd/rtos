#include "Semaphore.h"

void SemaphoreInit(Semaphore * sem, uint32_t startCount, uint32_t maxCount)//�ź�����ʼ�� 	
{
	EventInit(&sem->event, EventTypeSem);		//��ʼ���¼����ƿ�
	sem->maxCount = maxCount;					//��ʼ��������ֵ
	
	if( maxCount == 0){
		sem->count = startCount;
	}
	else{
		sem->count = (startCount > maxCount)? maxCount : startCount;
	}
}
