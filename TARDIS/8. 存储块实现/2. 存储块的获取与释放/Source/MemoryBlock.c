#include "TARDIS_OS.h"

void MemBlockInit(MemBlock * memBlock, uint8_t * memStart, uint32_t blockSize, uint32_t maxCount)//�洢���ʼ��
{
	uint8_t * memBlockStart = (uint8_t *)memStart;					//�洢�����ʼ��ַ
	uint8_t * memBlockEnd = memBlockStart + blockSize * maxCount;	//�洢��Ľ�����ַ
	
	if( blockSize < sizeof(Node)){		//�����̫С��ֱ���˳�
		return;
	}
	
	EventInit(&memBlock->event, EventTypeMemBlock);		//���¼����ƿ��ʼ��
	memBlock->memStart = memStart;						//������ʼ��ַ
	memBlock->blockSize = blockSize;					//���ÿ�Ĵ�С
	memBlock->maxCount = maxCount;						//�������������
	
	ListInit(&memBlock->blockList);	//��ʼ���洢������
	
	while(memBlockStart < memBlockEnd){
		NodeInit((Node *)memBlockStart);	//��ÿ����Ŀ�ʼ������һ���ڵ�
		ListAddTail(&memBlock->blockList, (Node *)memBlockStart);	//�ٰ�ÿ������뵽������
		memBlockStart += blockSize;			//������һ����
	}
	
}


uint32_t MemBlockWait(MemBlock * memBlock, uint8_t ** mem, uint32_t waitTicks){		//�ȴ��洢��
	uint32_t status = TaskEnterCritical();
	
	if( GetListCount(&memBlock->blockList) > 0){	//����п��еĴ洢��
		*mem = (uint8_t *)ListRemoveFirst(&memBlock->blockList);	//��ȡһ���洢��
		TaskExitCritical(status);
		return NoneError;
	}
	else{
		EventWait(&memBlock->event, currentTask, (void *)0, EventTypeMemBlock, waitTicks);	//���뵽�ȴ�����
		TaskExitCritical(status);
		
		TaskSched();
		*mem = currentTask->eventMsg;	//���ȷ���ʱ��ȡ���ȴ��Ĵ洢��
		return currentTask->waitEventResult;	//��������ȴ����
	}
}


uint32_t MemBlockNoWait(MemBlock * memBlock, uint8_t ** mem){//��ȡ�洢�飬�����ȴ�
	uint32_t status = TaskEnterCritical();
	
	if( GetListCount(&memBlock->blockList) > 0){	//����п��еĴ洢��
		*mem = (uint8_t *)ListRemoveFirst(&memBlock->blockList);	//��ȡһ���洢��
		TaskExitCritical(status);
		return NoneError;
	}
	else{
		TaskExitCritical(status);
		return ErrorResourceUnavaliable;	//������Դ������
	}
}

void MemBlockNotify(MemBlock * memBlock, uint8_t * mem){	//�洢����ͷ�
	uint32_t status = TaskEnterCritical();
	if(EventWaitCount(&memBlock->event) > 0){	//����������ڵȴ��洢��
		Task * task = EventWakeUp(&memBlock->event, (void *)mem, NoneError);	//����һ�����񣬲��������������
		if(task->priority < currentTask->priority){	//������ѵ��������ȼ����ߵĻ��������������
			TaskSched();
		}
	}
	else{
		ListAddHead(&memBlock->blockList, (Node *)mem);		//û�������ڵȴ��洢����뵽�洢���б�
	}
	TaskExitCritical(status);
}











