#ifndef _FLAGGROUP_H
#define _FLAGGROUP_H

#include "Event.h"

typedef struct _FlagGroup{
	Event event;		//�¼����ƿ�
	uint32_t flag;		//��־���� 	 
}FlagGroup;

void FlagGroupInit(FlagGroup * flagGroup, uint32_t flags);	//��ʼ��


#endif


