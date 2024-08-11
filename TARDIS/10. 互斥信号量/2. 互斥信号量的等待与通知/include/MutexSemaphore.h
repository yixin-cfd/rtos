#ifndef _MUTEXSEMAPHORE_H
#define _MUTEXSEMAPHORE_H

#include "Event.h"

typedef struct _Mutex{
	Event event;				//事件控制块
	uint32_t lockedCount;		//锁定计数器
	Task * owner;				//信号量拥有者
	uint32_t ownerOriginalPrio;	//信号量拥有者的原始优先级
}Mutex;

void MutexInit(Mutex * mutex);	//初始化
uint32_t MutexWait(Mutex * mutex, uint32_t waitTicks);	//等待信号量
uint32_t MutexNoWait(Mutex * mutex);	//等待信号量
uint32_t MutexNotify(Mutex * mutex);	//释放信号量
#endif


