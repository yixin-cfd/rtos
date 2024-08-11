#ifndef _MUTEXSEMAPHORE_H
#define _MUTEXSEMAPHORE_H

#include "Event.h"

typedef struct _Mutex{
	Event event;				//�¼����ƿ�
	uint32_t lockedCount;		//����������
	Task * owner;				//�ź���ӵ����
	uint32_t ownerOriginalPrio;	//�ź���ӵ���ߵ�ԭʼ���ȼ�
}Mutex;
typedef struct _MutexInfo{
	uint32_t taskCount;		//�ȴ���������
	uint32_t ownerPrio;		//ӵ�������ȼ�
	uint32_t inheritedPrio;	//�̳е����ȼ�
	Task * owner;			//�ź�����ӵ����
	uint32_t lockedCount;
}MutexInfo;

void MutexInit(Mutex * mutex);	//��ʼ��
uint32_t MutexWait(Mutex * mutex, uint32_t waitTicks);	//�ȴ��ź���
uint32_t MutexNoWait(Mutex * mutex);	//�ȴ��ź���
uint32_t MutexNotify(Mutex * mutex);	//�ͷ��ź���

uint32_t MutexDestory(Mutex * mutex);	//ɾ���ź���
void GetMutexInfo(Mutex * mutex, MutexInfo * info);	//״̬��ѯ
#endif


