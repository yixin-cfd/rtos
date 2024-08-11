#include "TARDIS_OS.h"


void MutexInit(Mutex * mutex)	//��ʼ��
{
	EventInit(&mutex->event, EventTypeMutex);
	
	mutex->lockedCount = 0;
	mutex->owner = (Task *)0;
	mutex->ownerOriginalPrio = TARDIS_OS_PRIORITY_COUNT;
	
}

uint32_t MutexWait(Mutex * mutex, uint32_t waitTicks)	//�ȴ��ź���
{
	uint32_t status = TaskEnterCritical();
	if(mutex->lockedCount <= 0){		//�����ǰ�ź���û�б�ռ��
		
		mutex->owner = currentTask;
		mutex->ownerOriginalPrio = currentTask->priority;
		mutex->lockedCount++;
		
		TaskExitCritical(status);
		return NoneError;
	}
	else{//�ź����Ѿ���������
		if(mutex->owner == currentTask){//��ǰ�ź������Լ�������
			mutex->lockedCount++;
			TaskExitCritical(status);
			return NoneError;
		}
		else{//��ǰ�ź��������Լ�������
			if(currentTask->priority < mutex->owner->priority){//��ǰ��������ȼ�����
				Task * owner = mutex->owner;
				if(owner->state == TARDIS_OS_TASK_STATE_RDY)	//������ھ���״̬
				{
					TaskSchedUnRdy(owner);						//�Ӿ���������ɾ��
					owner->priority = currentTask->priority;	//�޸�Ϊ���ߵ����ȼ�
					TaskSchedRdy(owner);						//���²��뵽����������
				}
				else
				{
					owner->priority = currentTask->priority;	//û�ھ��������У�ֱ���޸����ȼ�
				}
			}
			EventWait(&mutex->event, currentTask, (void *)0, EventTypeMutex, waitTicks);//����ǰ������뵽�ȴ�������
			TaskExitCritical(status);
			
			TaskSched();
			return currentTask->waitEventResult;
		}
	}
}

uint32_t MutexNoWait(Mutex * mutex)	//�ȴ��ź���
{
	uint32_t status = TaskEnterCritical();
	if(mutex->lockedCount <= 0){		//�����ǰ�ź���û�б�ռ��
		
		mutex->owner = currentTask;
		mutex->ownerOriginalPrio = currentTask->priority;
		mutex->lockedCount++;
		
		TaskExitCritical(status);
		return NoneError;
	}
	else{//�ź����Ѿ���������
		if(mutex->owner == currentTask){//��ǰ�ź������Լ�������
			mutex->lockedCount++;
			TaskExitCritical(status);
			return NoneError;
		}
		TaskExitCritical(status);
		return ErrorResourceUnavaliable;
	}
}

uint32_t MutexNotify(Mutex * mutex)	//�ͷ��ź���
{
	uint32_t status = TaskEnterCritical();
	if(mutex->lockedCount <= 0){	//�����ǰ�ź���û�б�������ֱ���˳�
		TaskExitCritical(status);
		return NoneError;
	}
	if(mutex->owner != currentTask){//�ź�����ռ���߲����Լ������ܹ��ͷ��ź��������������ߴ���
		TaskExitCritical(status);
		return ErrorOwner;
	}
	//�ź���ӵ�����Լ��ͷ�
	if(--mutex->lockedCount > 0){
		TaskExitCritical(status);
		return NoneError;
	}
	//������������ֵΪ�㣬��ǰ�ź�����Ҫ�ͷ�
	if(mutex->ownerOriginalPrio != mutex->owner->priority){	//���������ȼ��̳�
		if((mutex->owner->state & TARDIS_OS_TASK_STATE_RDY) == TARDIS_OS_TASK_STATE_RDY){//������ھ���״̬
			TaskSchedUnRdy(mutex->owner);					//�Ӿ����������Ƴ�
			currentTask->priority = mutex->ownerOriginalPrio;//�ָ�ԭʼ���ȼ�
			TaskSchedRdy(mutex->owner);						 //���²��뵽��Ӧ�����ȼ�������
		}
		else{
			currentTask->priority = mutex->ownerOriginalPrio;//�ָ�ԭʼ���ȼ�
		}
	}
	if(EventWaitCount(&mutex->event) > 0){	//���������ڵȴ��ź���
		Task * task = EventWakeUp(&mutex->event, (void *)0, NoneError);	//����һ������
		
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

uint32_t MutexDestory(Mutex * mutex)	//ɾ���ź���
{
	uint32_t count = 0;
	uint32_t status = TaskEnterCritical();
	
	if(mutex->lockedCount > 0)	//�����ǰ�ź���������
	{
		if(mutex->ownerOriginalPrio != mutex->owner->priority)	//���������ȼ��̳�
		{
			if((mutex->owner->state & TARDIS_OS_TASK_STATE_RDY) == TARDIS_OS_TASK_STATE_RDY){//������ھ���״̬
				TaskSchedUnRdy(mutex->owner);						//�Ӿ����������Ƴ�
				mutex->owner->priority = mutex->ownerOriginalPrio;//�ָ�ԭʼ���ȼ�
				TaskSchedRdy(mutex->owner);						 //���²��뵽��Ӧ�����ȼ�������
			}
			else{
				mutex->owner->priority = mutex->ownerOriginalPrio;//�ָ�ԭʼ���ȼ�
			}
		}
		count = EventRemoveAll(&mutex->event, (void *)0, ErrorDel); //������������
		if(count > 0){
			TaskSched(); 
		}
	}
	TaskExitCritical(status);
	return count;
}
void GetMutexInfo(Mutex * mutex, MutexInfo * info)	//״̬��ѯ
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










