#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#include "Event.h"


typedef struct _Semaphore{
	Event event;		//事件控制块
	uint32_t count;		//当前计数值
	uint32_t maxCount;	//最大计数值
}Semaphore;

void SemaphoreInit(Semaphore * sem, uint32_t startCount, uint32_t maxCount);//信号量初始化 	

#endif

