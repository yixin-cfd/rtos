#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#include "Event.h"


typedef struct _Semaphore{
	Event event;		//事件控制块
	uint32_t count;		//当前计数值
	uint32_t maxCount;	//最大计数值
}Semaphore;

void SemaphoreInit(Semaphore * sem, uint32_t startCount, uint32_t maxCount);//信号量初始化 	
uint32_t SemWait (Semaphore * sem, uint32_t waitTicks);		//等待信号量
uint32_t SemNoWait(Semaphore * sem);						//不需要等待信号量
void SemNotify(Semaphore * sem);							//信号量的释放
#endif

