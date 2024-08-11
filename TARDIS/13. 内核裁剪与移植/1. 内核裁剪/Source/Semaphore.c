#include "TARDIS_OS.h"

#if TARDIS_OS_ENABLE_SEMAPHORE == 1
void SemaphoreInit(Semaphore * sem, uint32_t startCount, uint32_t maxCount)//信号量初始化 	
{
	EventInit(&sem->event, EventTypeSem);		//初始化事件控制块
	sem->maxCount = maxCount;					//初始化最大计数值
	
	if( maxCount == 0){							//表示该信号量无限
		sem->count = startCount;
	}
	else{
		sem->count = (startCount > maxCount)? maxCount : startCount;
	}
}

uint32_t SemWait (Semaphore * sem, uint32_t waitTicks)		//等待信号量
{
	uint32_t status = TaskEnterCritical();
	
	if( sem->count > 0){			//如果信号量计数值大于零
		--sem->count;				//计数值减一
		TaskExitCritical(status);
		return NoneError;			//返回NoneError
	}
	else{
		EventWait(&sem->event, currentTask, (void *)0, EventTypeSem, waitTicks);	//插入到信号量的事件控制块中
		TaskExitCritical(status);
		
		TaskSched();	//进行任务调度		
		return currentTask->waitEventResult;		//因为插入的事件等待队列，如果重新切换到当前任务，说明事件等待结束
	}
}

uint32_t SemNoWait(Semaphore * sem){	//不需要等待信号量
	uint32_t status = TaskEnterCritical();
	if( sem->count > 0){			//如果信号量计数值大于零
		--sem->count;				//计数值减一
		TaskExitCritical(status);
		return NoneError;			//返回NoneError
	}
	else{
		TaskExitCritical(status);
			
		return ErrorResourceUnavaliable;	//返回资源无法获取
	}

}

void SemNotify(Semaphore * sem){	//信号量的释放
	uint32_t status = TaskEnterCritical();
	if( EventWaitCount(&sem->event) > 0){	//如果信号量的等待队列中有任务
		
		Task * task = EventWakeUp( &sem->event, (void *)0, NoneError);	//从队列中唤醒一个任务
		
		if( task->priority < currentTask->priority){	//如果唤醒的任务优先级更高
			TaskSched();								//进行任务调度
		}
	}
	else{
		++sem->count;		//没有任务在等待，将可用的信号量自增一
		if ( ( sem->maxCount != 0) && ( sem->count > sem->maxCount)){//如果资源有限并且当前计数值大于最大计数值
			sem->count = sem->maxCount;
		}
	}
	TaskExitCritical(status);
}

void GetSemInfo(Semaphore * sem, SemInfo * info)			//查询信号量
{
	uint32_t status = TaskEnterCritical();
	info->count = sem->count;
	info->maxCount = sem->maxCount;
	info->taskCount = EventWaitCount( &sem->event);
	TaskExitCritical(status);
}
uint32_t SemDestory(Semaphore * sem)						//删除信号量
{

	uint32_t status = TaskEnterCritical();
	uint32_t count = EventRemoveAll( &sem->event, (void *)0, ErrorDel);	//从等待队列中移除所有任务
	sem->count = 0;	
	TaskExitCritical(status);
	if(count > 0){	//如果有任务之前在等待
		TaskSched();//进行任务调度
	}
	return count;

}

#endif
