#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#include "Event.h"


typedef struct _Semaphore{
	Event event;		//�¼����ƿ�
	uint32_t count;		//��ǰ����ֵ
	uint32_t maxCount;	//������ֵ
}Semaphore;

typedef struct _SemInfo{
	uint32_t count;		//��ǰ����ֵ
	uint32_t maxCount;	//������ֵ
	uint32_t taskCount;	//�ȴ������е���������
}SemInfo;

void SemaphoreInit(Semaphore * sem, uint32_t startCount, uint32_t maxCount);//�ź�����ʼ�� 	
uint32_t SemWait (Semaphore * sem, uint32_t waitTicks);		//�ȴ��ź���
uint32_t SemNoWait(Semaphore * sem);						//����Ҫ�ȴ��ź���
void SemNotify(Semaphore * sem);							//�ź������ͷ�

void GetSemInfo(Semaphore * sem, SemInfo * info);			//��ѯ�ź���
uint32_t SemDestory(Semaphore * sem);						//ɾ���ź���
#endif



