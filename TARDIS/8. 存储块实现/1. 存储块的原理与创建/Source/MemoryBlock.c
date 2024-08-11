#include "TARDIS_OS.h"

void MemBlockInit(MemBlock * memBlock, uint8_t * memStart, uint32_t blockSize, uint32_t maxCount)//存储块初始化
{
	uint8_t * memBlockStart = (uint8_t *)memStart;					//存储块的起始地址
	uint8_t * memBlockEnd = memBlockStart + blockSize * maxCount;	//存储块的结束地址
	
	if( blockSize < sizeof(Node)){		//如果块太小，直接退出
		return;
	}
	
	EventInit(&memBlock->event, EventTypeMemBlock);		//对事件控制块初始化
	memBlock->memStart = memStart;						//设置起始地址
	memBlock->blockSize = blockSize;					//设置块的大小
	memBlock->maxCount = maxCount;						//设置最大块的数量
	
	ListInit(&memBlock->blockList);	//初始化存储块链表
	
	while(memBlockStart < memBlockEnd){
		NodeInit((Node *)memBlockStart);	//在每个块的开始处生成一个节点
		ListAddTail(&memBlock->blockList, (Node *)memBlockStart);	//再把每个块插入到链表中
		memBlockStart += blockSize;			//继续下一个块
	}
	
}

