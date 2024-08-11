#ifndef _MUTEXSEMAPHORE_H
#define _MUTEXSEMAPHORE_H

#include "Event.h"

typedef struct _Mutex{
	Event event;				//事件控制块
	uint32_t lockedCount;		//锁定计数器
	Task * owner;				//信号量拥有者
	uint32_t ownerOriginalPrio;	//信号量拥有者的原始优先级
}Mutex;
typedef struct _MutexInfo{
	uint32_t taskCount;		//等待任务数量
	uint32_t ownerPrio;		//拥有者优先级
	uint32_t inheritedPrio;	//继承的优先级
	Task * owner;			//信号量的拥有者
	uint32_t lockedCount;
}MutexInfo;

void MutexInit(Mutex * mutex);	//初始化
uint32_t MutexWait(Mutex * mutex, uint32_t waitTicks);	//等待信号量
uint32_t MutexNoWait(Mutex * mutex);	//等待信号量
uint32_t MutexNotify(Mutex * mutex);	//释放信号量

uint32_t MutexDestory(Mutex * mutex);	//删除信号量
void GetMutexInfo(Mutex * mutex, MutexInfo * info);	//状态查询
#endif


