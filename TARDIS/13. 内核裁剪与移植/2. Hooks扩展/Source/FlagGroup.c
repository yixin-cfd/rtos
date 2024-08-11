#include "TARDIS_OS.h"

#if TARDIS_OS_ENABLE_FLAGGROUP == 1
void FlagGroupInit(FlagGroup * flagGroup, uint32_t flags)	//��ʼ��
{
	EventInit(&flagGroup->event, EventTypeFlagGroup);
	flagGroup->flag = flags;
}
//param:
//flagGroup : �¼���־�飬ȡ�����е��¼����
//type : ������־��鷽ʽ���Լ��Ƿ����ĵ���־
static uint32_t FlagGroupCheckAndConsume(FlagGroup * flagGroup, uint32_t type, uint32_t * flags)		//����Ƿ������־�������Ƿ����ĵ���Щ��־
{
	uint32_t srcFlag = *flags;	//Դ��־
	uint32_t isSet = type & FLAGGROUP_SET;	//�Ƿ�����һ����
	uint32_t isAll = type & FLAGGROUP_ALL;	//�Ƿ���ȫ���ı�־λ
	uint32_t isConsume = type & FLAGGROUP_CONSUME;	//�Ƿ����ĵ���־λ
	
	uint32_t calFlag = isSet ? (flagGroup->flag & srcFlag) : (~flagGroup->flag & srcFlag);	//�õ��������һ��λ��
	
	if( ((isAll != 0) && (calFlag == srcFlag)) || ((isAll == 0) && (calFlag != 0 ))){
		if (isConsume){
			if(isSet){
				flagGroup->flag &= ~srcFlag;
			}
			else{
				flagGroup->flag |= srcFlag;
			}
		}
		*flags = calFlag;		//����Щ��־�б��ı���ڴ���ȥ
		return NoneError;
	}
	*flags = calFlag;
	return ErrorResourceUnavaliable;
}
uint32_t FlagGroupWait(FlagGroup * flagGroup, uint32_t waitType, uint32_t  requestFlag, uint32_t * resultFlag, uint32_t waitTicks)	//�ȴ��¼����
{
	uint32_t result;
	uint32_t flags = requestFlag;
	uint32_t status = TaskEnterCritical();
	result = FlagGroupCheckAndConsume(flagGroup, waitType, &flags);
	if(result != NoneError){
		currentTask->waitFlagsType = waitType;	//�ȴ��¼�����
		currentTask->eventFlags = requestFlag;	//�ȴ��¼����
		EventWait(&flagGroup->event, currentTask, (void *)0, EventTypeFlagGroup, waitTicks);//���뵽�¼��ȴ�������
		TaskExitCritical(status);
		
		TaskSched();
		
		*resultFlag = currentTask->eventFlags;	//ȡ���ȴ��¼����
		result = currentTask->waitEventResult;	//ȡ�����Ⱥ�Ľ��
	}
	else{
		*resultFlag = flags;
		TaskExitCritical(status);
	}
	return result;
}
uint32_t FlagGroupNoWait(FlagGroup * flagGroup, uint32_t waitType, uint32_t  requestFlag, uint32_t * resultFlag)//�ȴ��¼����
{
	uint32_t flags = requestFlag;
	uint32_t status = TaskEnterCritical();
	uint32_t result = FlagGroupCheckAndConsume(flagGroup, waitType, &flags);
	TaskExitCritical(status);
	*resultFlag = flags;
	return NoneError;
}
void FlagGroupNotify(FlagGroup * flagGroup, uint8_t isSet, uint32_t flag)	//֪ͨ
{
	List * waitList;
	Node * node;
	Node *nextNode;
	uint8_t sched = 0;
	uint32_t status = TaskEnterCritical();
	if(isSet){			//�����¼����
		flagGroup->flag |= flag;
	}
	else{
		flagGroup->flag &= ~flag;
	}
	waitList = &flagGroup->event.waitList;
	//�����б�
	for(node = waitList->headNode.nextNode; node != &(waitList->headNode); node = nextNode){
		Task * task = NodeParent(node, Task, linkNode);
		uint32_t flags = task->eventFlags;		//����ȴ��¼����
		uint32_t result = FlagGroupCheckAndConsume(flagGroup, task->waitFlagsType, &flags);//���������Ҫ�ı�־�Ƿ�����
		nextNode = node->nextNode;
		if(result == NoneError){	//�����������
			task->eventFlags = flags;
			EventWakeUpTask(&flagGroup->event, task, (void *)0, NoneError);//��������
			sched = 1;
		}
	}
	if(sched){
		TaskSched();
	}
	TaskExitCritical(status);
}


void GetFlagGroupInfo(FlagGroup * flagGroup, FlagGroupInfo * info)//״̬��ѯ
{
	uint32_t status = TaskEnterCritical();
	info->flags = flagGroup->flag;
	info->taskCount = EventWaitCount(&flagGroup->event);
	TaskExitCritical(status);
}

uint32_t FlagGroupDestoty(FlagGroup * flagGroup)//�¼���־���ɾ��
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
















