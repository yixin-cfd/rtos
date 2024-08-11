#ifndef _FLAGGROUP_H
#define _FLAGGROUP_H

#include "Event.h"

typedef struct _FlagGroup{
	Event event;		//事件控制块
	uint32_t flag;		//标志集合 	 
}FlagGroup;

void FlagGroupInit(FlagGroup * flagGroup, uint32_t flags);	//初始化


#endif


