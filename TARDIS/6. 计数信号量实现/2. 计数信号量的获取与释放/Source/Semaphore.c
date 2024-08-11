#include "TARDIS_OS.h"

void SemaphoreInit(Semaphore * sem, uint32_t startCount, uint32_t maxCount)//�ź�����ʼ�� 	
{
	EventInit(&sem->event, EventTypeSem);		//��ʼ���¼����ƿ�
	sem->maxCount = maxCount;					//��ʼ��������ֵ
	
	if( maxCount == 0){							//��ʾ���ź�������
		sem->count = startCount;
	}
	else{
		sem->count = (startCount > maxCount)? maxCount : startCount;
	}
}

uint32_t SemWait (Semaphore * sem, uint32_t waitTicks)		//�ȴ��ź���
{
	uint32_t status = TaskEnterCritical();
	
	if( sem->count > 0){			//����ź�������ֵ������
		--sem->count;				//����ֵ��һ
		TaskExitCritical(status);
		return NoneError;			//����NoneError
	}
	else{
		EventWait(&sem->event, currentTask, (void *)0, EventTypeSem, waitTicks);	//���뵽�ź������¼����ƿ���
		TaskExitCritical(status);
		
		TaskSched();	//�����������		
		return currentTask->waitEventResult;		//��Ϊ������¼��ȴ����У���������л�����ǰ����˵���¼��ȴ�����
	}
}

uint32_t SemNoWait(Semaphore * sem){	//����Ҫ�ȴ��ź���
	uint32_t status = TaskEnterCritical();
	if( sem->count > 0){			//����ź�������ֵ������
		--sem->count;				//����ֵ��һ
		TaskExitCritical(status);
		return NoneError;			//����NoneError
	}
	else{
		TaskExitCritical(status);
			
		return ErrorResourceUnavaliable;	//������Դ�޷���ȡ
	}

}

void SemNotify(Semaphore * sem){	//�ź������ͷ�
	uint32_t status = TaskEnterCritical();
	if( EventWaitCount(&sem->event) > 0){	//����ź����ĵȴ�������������
		
		Task * task = EventWakeUp( &sem->event, (void *)0, NoneError);	//�Ӷ����л���һ������
		
		if( task->priority < currentTask->priority){	//������ѵ��������ȼ�����
			TaskSched();								//�����������
		}
	}
	else{
		++sem->count;		//û�������ڵȴ��������õ��ź�������һ
		if ( ( sem->maxCount != 0) && ( sem->count > sem->maxCount)){//�����Դ���޲��ҵ�ǰ����ֵ����������ֵ
			sem->count = sem->maxCount;
		}
	}
	TaskExitCritical(status);
}


