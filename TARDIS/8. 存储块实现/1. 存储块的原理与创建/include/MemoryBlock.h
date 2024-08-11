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

void MemBlockInit(MemBlock * memBlock, uint8_t * memStart, uint32_t blockSize, uint32_t maxCount);//�洢���ʼ��
#endif

