#ifndef _FLAGGROUP_H
#define _FLAGGROUP_H

#include "Event.h"

typedef struct _FlagGroup{
	Event event;		//�¼����ƿ�
	uint32_t flag;		//��־���� 	 
}FlagGroup;

#define 	FLAGGROUP_CLEAR  			(0x0 << 0)		//���
#define 	FLAGGROUP_SET				(0x1 << 0)		//��λ
#define 	FLAGGROUP_ANY  				(0x0 << 1)		//����
#define 	FLAGGROUP_ALL				(0x1 << 1)		//����


#define 	FLAGGROUP_SET_ALL			(FLAGGROUP_SET | FLAGGROUP_ALL)		//����λ��һ
#define 	FLAGGROUP_SET_ANY			(FLAGGROUP_SET | FLAGGROUP_ANY)		//����λ��һ
#define 	FLAGGROUP_CLEAR_ALL			(FLAGGROUP_CLEAR | FLAGGROUP_ALL)	//����λ����
#define 	FLAGGROUP_CLEAR_ANY			(FLAGGROUP_CLEAR | FLAGGROUP_ANY)	//����λ����

#define 	FLAGGROUP_CONSUME			(1 << 7)		//���ܵ���־���Ƿ���Ҫ�ָ�

void FlagGroupInit(FlagGroup * flagGroup, uint32_t flags);	//��ʼ��

uint32_t FlagGroupWait(FlagGroup * flagGroup, uint32_t waitType, uint32_t requestFlag, uint32_t * resultFlag, uint32_t waitTicks);	//�ȴ��¼����
uint32_t FlagGroupNoWait(FlagGroup * flagGroup, uint32_t waitType, uint32_t requestFlag, uint32_t * resultFlag);	//�ȴ��¼����
void FlagGroupNotify(FlagGroup * flagGroup, uint8_t isSet, uint32_t flag);	//֪ͨ
#endif





