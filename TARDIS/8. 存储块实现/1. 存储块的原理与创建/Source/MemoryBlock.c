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

