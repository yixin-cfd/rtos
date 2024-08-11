#ifndef _FLAGGROUP_H
#define _FLAGGROUP_H

#include "Event.h"

typedef struct _FlagGroup{
	Event event;		//事件控制块
	uint32_t flag;		//标志集合 	 
}FlagGroup;

#define 	FLAGGROUP_CLEAR  			(0x0 << 0)		//清除
#define 	FLAGGROUP_SET				(0x1 << 0)		//置位
#define 	FLAGGROUP_ANY  				(0x0 << 1)		//任意
#define 	FLAGGROUP_ALL				(0x1 << 1)		//所有


#define 	FLAGGROUP_SET_ALL			(FLAGGROUP_SET | FLAGGROUP_ALL)		//所有位置一
#define 	FLAGGROUP_SET_ANY			(FLAGGROUP_SET | FLAGGROUP_ANY)		//任意位置一
#define 	FLAGGROUP_CLEAR_ALL			(FLAGGROUP_CLEAR | FLAGGROUP_ALL)	//所有位清零
#define 	FLAGGROUP_CLEAR_ANY			(FLAGGROUP_CLEAR | FLAGGROUP_ANY)	//任意位清零

#define 	FLAGGROUP_CONSUME			(1 << 7)		//接受到标志后是否需要恢复

void FlagGroupInit(FlagGroup * flagGroup, uint32_t flags);	//初始化

uint32_t FlagGroupWait(FlagGroup * flagGroup, uint32_t waitType, uint32_t requestFlag, uint32_t * resultFlag, uint32_t waitTicks);	//等待事件标记
uint32_t FlagGroupNoWait(FlagGroup * flagGroup, uint32_t waitType, uint32_t requestFlag, uint32_t * resultFlag);	//等待事件标记
void FlagGroupNotify(FlagGroup * flagGroup, uint8_t isSet, uint32_t flag);	//通知
#endif





