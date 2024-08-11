#include "TARDIS_OS.h"


void MutexInit(Mutex * mutex)	//初始化
{
	EventInit(&mutex->event, EventTypeMutex);
	
	mutex->lockedCount = 0;
	mutex->owner = (Task *)0;
	mutex->ownerOriginalPrio = TARDIS_OS_PRIORITY_COUNT;
	
}

uint32_t MutexWait(Mutex * mutex, uint32_t waitTicks)	//等待信号量
{
	uint32_t status = TaskEnterCritical();
	if(mutex->lockedCount <= 0){		//如果当前信号量没有被占有
		
		mutex->owner = currentTask;
		mutex->ownerOriginalPrio = currentTask->priority;
		mutex->lockedCount++;
		
		TaskExitCritical(status);
		return NoneError;
	}
	else{//信号量已经被锁定了
		if(mutex->owner == currentTask){//当前信号量是自己锁定的
			mutex->lockedCount++;
			TaskExitCritical(status);
			return NoneError;
		}
		else{//当前信号量不是自己锁定的
			if(currentTask->priority < mutex->owner->priority){//当前任务的优先级更高
				Task * owner = mutex->owner;
				if(owner->state == TARDIS_OS_TASK_STATE_RDY)	//如果处于就绪状态
				{
					TaskSchedUnRdy(owner);						//从就绪队列中删除
					owner->priority = currentTask->priority;	//修改为更高的优先级
					TaskSchedRdy(owner);						//重新插入到就绪队列中
				}
				else
				{
					owner->priority = currentTask->priority;	//没在就绪队列中，直接修改优先级
				}
			}
			EventWait(&mutex->event, currentTask, (void *)0, EventTypeMutex, waitTicks);//将当前任务插入到等待队列中
			TaskExitCritical(status);
			
			TaskSched();
			return currentTask->waitEventResult;
		}
	}
}

uint32_t MutexNoWait(Mutex * mutex)	//等待信号量
{
	uint32_t status = TaskEnterCritical();
	if(mutex->lockedCount <= 0){		//如果当前信号量没有被占有
		
		mutex->owner = currentTask;
		mutex->ownerOriginalPrio = currentTask->priority;
		mutex->lockedCount++;
		
		TaskExitCritical(status);
		return NoneError;
	}
	else{//信号量已经被锁定了
		if(mutex->owner == currentTask){//当前信号量是自己锁定的
			mutex->lockedCount++;
			TaskExitCritical(status);
			return NoneError;
		}
		TaskExitCritical(status);
		return ErrorResourceUnavaliable;
	}
}

uint32_t MutexNotify(Mutex * mutex)	//释放信号量
{
	uint32_t status = TaskEnterCritical();
	if(mutex->lockedCount <= 0){	//如果当前信号量没有被锁定，直接退出
		TaskExitCritical(status);
		return NoneError;
	}
	if(mutex->owner != currentTask){//信号量的占有者不是自己，不能够释放信号量，返回所有者错误
		TaskExitCritical(status);
		return ErrorOwner;
	}
	//信号量拥有者自己释放
	if(--mutex->lockedCount > 0){
		TaskExitCritical(status);
		return NoneError;
	}
	//锁定计数器的值为零，当前信号量需要释放
	if(mutex->ownerOriginalPrio != mutex->owner->priority){	//发生了优先级继承
		if((mutex->owner->state & TARDIS_OS_TASK_STATE_RDY) == TARDIS_OS_TASK_STATE_RDY){//如果处于就绪状态
			TaskSchedUnRdy(mutex->owner);					//从就绪队列中移除
			currentTask->priority = mutex->ownerOriginalPrio;//恢复原始优先级
			TaskSchedRdy(mutex->owner);						 //重新插入到对应的优先级队列中
		}
		else{
			currentTask->priority = mutex->ownerOriginalPrio;//恢复原始优先级
		}
	}
	if(EventWaitCount(&mutex->event) > 0){	//还有任务在等待信号量
		Task * task = EventWakeUp(&mutex->event, (void *)0, NoneError);	//唤醒一个任务
		
		mutex->owner = task;
		mutex->ownerOriginalPrio = task->priority;
		mutex->lockedCount++;
		
		if(task->priority < currentTask->priority){
			TaskSched();
		}
	}
	TaskExitCritical(status);
	return NoneError;
}

uint32_t MutexDestory(Mutex * mutex)	//删除信号量
{
	uint32_t count = 0;
	uint32_t status = TaskEnterCritical();
	
	if(mutex->lockedCount > 0)	//如果当前信号量被锁定
	{
		if(mutex->ownerOriginalPrio != mutex->owner->priority)	//发生了优先级继承
		{
			if((mutex->owner->state & TARDIS_OS_TASK_STATE_RDY) == TARDIS_OS_TASK_STATE_RDY){//如果处于就绪状态
				TaskSchedUnRdy(mutex->owner);						//从就绪队列中移除
				mutex->owner->priority = mutex->ownerOriginalPrio;//恢复原始优先级
				TaskSchedRdy(mutex->owner);						 //重新插入到对应的优先级队列中
			}
			else{
				mutex->owner->priority = mutex->ownerOriginalPrio;//恢复原始优先级
			}
		}
		count = EventRemoveAll(&mutex->event, (void *)0, ErrorDel); //唤醒所有任务
		if(count > 0){
			TaskSched(); 
		}
	}
	TaskExitCritical(status);
	return count;
}
void GetMutexInfo(Mutex * mutex, MutexInfo * info)	//状态查询
{
	uint32_t status = TaskEnterCritical();
		
	info->taskCount = EventWaitCount(&mutex->event);
	info->ownerPrio = mutex->ownerOriginalPrio;
	
	if(mutex->owner != (Task *)0)
	{
		info->inheritedPrio = mutex->owner->priority;
	}
	else
	{
		info->inheritedPrio = TARDIS_OS_PRIORITY_COUNT;
	}
	
	info->owner = mutex->owner;
	info->lockedCount = mutex->lockedCount;
	
	TaskExitCritical(status);
}










