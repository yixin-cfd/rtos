#include "TARDIS_OS.h"

#if TARDIS_OS_ENABLE_FLAGGROUP == 1
void FlagGroupInit(FlagGroup * flagGroup, uint32_t flags)	//初始化
{
	EventInit(&flagGroup->event, EventTypeFlagGroup);
	flagGroup->flag = flags;
}
//param:
//flagGroup : 事件标志组，取出其中的事件标记
//type : 决定标志检查方式，以及是否消耗掉标志
static uint32_t FlagGroupCheckAndConsume(FlagGroup * flagGroup, uint32_t type, uint32_t * flags)		//检查是否满足标志并决定是否消耗掉这些标志
{
	uint32_t srcFlag = *flags;	//源标志
	uint32_t isSet = type & FLAGGROUP_SET;	//是否是置一操作
	uint32_t isAll = type & FLAGGROUP_ALL;	//是否是全部的标志位
	uint32_t isConsume = type & FLAGGROUP_CONSUME;	//是否消耗掉标志位
	
	uint32_t calFlag = isSet ? (flagGroup->flag & srcFlag) : (~flagGroup->flag & srcFlag);	//得到置零或置一的位置
	
	if( ((isAll != 0) && (calFlag == srcFlag)) || ((isAll == 0) && (calFlag != 0 ))){
		if (isConsume){
			if(isSet){
				flagGroup->flag &= ~srcFlag;
			}
			else{
				flagGroup->flag |= srcFlag;
			}
		}
		*flags = calFlag;		//将这些标志中被改变的在传回去
		return NoneError;
	}
	*flags = calFlag;
	return ErrorResourceUnavaliable;
}
uint32_t FlagGroupWait(FlagGroup * flagGroup, uint32_t waitType, uint32_t  requestFlag, uint32_t * resultFlag, uint32_t waitTicks)	//等待事件标记
{
	uint32_t result;
	uint32_t flags = requestFlag;
	uint32_t status = TaskEnterCritical();
	result = FlagGroupCheckAndConsume(flagGroup, waitType, &flags);
	if(result != NoneError){
		currentTask->waitFlagsType = waitType;	//等待事件类型
		currentTask->eventFlags = requestFlag;	//等待事件标记
		EventWait(&flagGroup->event, currentTask, (void *)0, EventTypeFlagGroup, waitTicks);//插入到事件等待队列中
		TaskExitCritical(status);
		
		TaskSched();
		
		*resultFlag = currentTask->eventFlags;	//取出等待事件标记
		result = currentTask->waitEventResult;	//取出调度后的结果
	}
	else{
		*resultFlag = flags;
		TaskExitCritical(status);
	}
	return result;
}
uint32_t FlagGroupNoWait(FlagGroup * flagGroup, uint32_t waitType, uint32_t  requestFlag, uint32_t * resultFlag)//等待事件标记
{
	uint32_t flags = requestFlag;
	uint32_t status = TaskEnterCritical();
	uint32_t result = FlagGroupCheckAndConsume(flagGroup, waitType, &flags);
	TaskExitCritical(status);
	*resultFlag = flags;
	return NoneError;
}
void FlagGroupNotify(FlagGroup * flagGroup, uint8_t isSet, uint32_t flag)	//通知
{
	List * waitList;
	Node * node;
	Node *nextNode;
	uint8_t sched = 0;
	uint32_t status = TaskEnterCritical();
	if(isSet){			//设置事件标记
		flagGroup->flag |= flag;
	}
	else{
		flagGroup->flag &= ~flag;
	}
	waitList = &flagGroup->event.waitList;
	//遍历列表
	for(node = waitList->headNode.nextNode; node != &(waitList->headNode); node = nextNode){
		Task * task = NodeParent(node, Task, linkNode);
		uint32_t flags = task->eventFlags;		//任务等待事件标记
		uint32_t result = FlagGroupCheckAndConsume(flagGroup, task->waitFlagsType, &flags);//检查任务需要的标志是否满足
		nextNode = node->nextNode;
		if(result == NoneError){	//如果满足条件
			task->eventFlags = flags;
			EventWakeUpTask(&flagGroup->event, task, (void *)0, NoneError);//唤醒任务
			sched = 1;
		}
	}
	if(sched){
		TaskSched();
	}
	TaskExitCritical(status);
}


void GetFlagGroupInfo(FlagGroup * flagGroup, FlagGroupInfo * info)//状态查询
{
	uint32_t status = TaskEnterCritical();
	info->flags = flagGroup->flag;
	info->taskCount = EventWaitCount(&flagGroup->event);
	TaskExitCritical(status);
}

uint32_t FlagGroupDestoty(FlagGroup * flagGroup)//事件标志组的删除
{
	uint32_t status = TaskEnterCritical();
	uint32_t count = EventRemoveAll(&flagGroup->event, (void *)0, ErrorDel);
	TaskExitCritical(status);
	
	if(count > 0){
		TaskSched();
	}
	return count;
}

#endif
















