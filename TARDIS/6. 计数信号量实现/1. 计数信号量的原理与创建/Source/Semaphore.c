#include "Semaphore.h"

void SemaphoreInit(Semaphore * sem, uint32_t startCount, uint32_t maxCount)//信号量初始化 	
{
	EventInit(&sem->event, EventTypeSem);		//初始化事件控制块
	sem->maxCount = maxCount;					//初始化最大计数值
	
	if( maxCount == 0){
		sem->count = startCount;
	}
	else{
		sem->count = (startCount > maxCount)? maxCount : startCount;
	}
}
