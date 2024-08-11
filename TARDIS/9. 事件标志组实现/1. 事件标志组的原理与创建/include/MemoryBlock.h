#ifndef _MEMORYBLOCK_H
#define _MEMORYBLOCK_H

#include "Event.h"

typedef struct _MemBlock{
	Event event;		//�¼����ƿ�
	void * memStart;	//�洢����ʼ��ַ
	uint32_t blockSize;	//ÿ���洢��Ĵ�С
	uint32_t maxCount;	//���Ĵ洢������
	List blockList;		//�洢�������
}MemBlock;

typedef struct _MemBlockInfo{
	uint32_t count;		//��ǰ�洢��ĸ���
	uint32_t maxCount;	//���Ĵ洢������
	uint32_t blockSize;	//�洢��Ĵ�С
	uint32_t taskCount;	//��ǰ�ȴ���������
}MemBlcokInfo;


void MemBlockInit(MemBlock * memBlock, uint8_t * memStart, uint32_t blockSize, uint32_t maxCount);//�洢���ʼ��
uint32_t MemBlockWait(MemBlock * memBlock, uint8_t ** mem, uint32_t waitTicks);	//�ȴ��洢��
uint32_t MemBlockNoWait(MemBlock * memBlock, uint8_t ** mem);					//��ȡ�洢�飬�����ȴ�
void MemBlockNotify(MemBlock * memBlock, uint8_t * mem);						//�洢����ͷ�

void GetMemBlockInfo(MemBlock * memblock, MemBlcokInfo * info);		//�洢��״̬��ѯ
uint32_t MemBlockDestory(MemBlock * memblock);	//�洢��ɾ������
#endif

