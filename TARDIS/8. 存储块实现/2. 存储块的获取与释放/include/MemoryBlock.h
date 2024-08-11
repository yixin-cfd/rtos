#ifndef _MEMORYBLOCK_H
#define _MEMORYBLOCK_H

#include "Event.h"

typedef struct _MemBlock{
	Event event;		//事件控制块
	void * memStart;	//存储区起始地址
	uint32_t blockSize;	//每个存储块的大小
	uint32_t maxCount;	//最大的存储块数量
	List blockList;		//存储块的链表
}MemBlock;

void MemBlockInit(MemBlock * memBlock, uint8_t * memStart, uint32_t blockSize, uint32_t maxCount);//存储块初始化
uint32_t MemBlockWait(MemBlock * memBlock, uint8_t ** mem, uint32_t waitTicks);	//等待存储块
uint32_t MemBlockNoWait(MemBlock * memBlock, uint8_t ** mem);					//获取存储块，但不等待
void MemBlockNotify(MemBlock * memBlock, uint8_t * mem);						//存储块的释放
#endif

