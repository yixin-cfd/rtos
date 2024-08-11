#ifndef _MUTEXSEMAPHORE_H
#define _MUTEXSEMAPHORE_H

#include "Event.h"

typedef struct _Mutex{
	Event event;				//�¼����ƿ�
	uint32_t lockedCount;		//����������
	Task * owner;				//�ź���ӵ����
	uint32_t ownerOriginalPrio;	//�ź���ӵ���ߵ�ԭʼ���ȼ�
}Mutex;

void MutexInit(Mutex * mutex);	//��ʼ��
uint32_t MutexWait(Mutex * mutex, uint32_t waitTicks);	//�ȴ��ź���
uint32_t MutexNoWait(Mutex * mutex);	//�ȴ��ź���
uint32_t MutexNotify(Mutex * mutex);	//�ͷ��ź���
#endif


