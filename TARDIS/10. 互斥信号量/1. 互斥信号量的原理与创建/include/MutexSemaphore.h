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

#endif


