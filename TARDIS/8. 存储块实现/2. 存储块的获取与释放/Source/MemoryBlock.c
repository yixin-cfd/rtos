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


uint32_t MemBlockWait(MemBlock * memBlock, uint8_t ** mem, uint32_t waitTicks){		//等待存储块
	uint32_t status = TaskEnterCritical();
	
	if( GetListCount(&memBlock->blockList) > 0){	//如果有空闲的存储块
		*mem = (uint8_t *)ListRemoveFirst(&memBlock->blockList);	//获取一个存储块
		TaskExitCritical(status);
		return NoneError;
	}
	else{
		EventWait(&memBlock->event, currentTask, (void *)0, EventTypeMemBlock, waitTicks);	//插入到等待队列
		TaskExitCritical(status);
		
		TaskSched();
		*mem = currentTask->eventMsg;	//调度返回时，取出等待的存储块
		return currentTask->waitEventResult;	//返回任务等待结果
	}
}


uint32_t MemBlockNoWait(MemBlock * memBlock, uint8_t ** mem){//获取存储块，但不等待
	uint32_t status = TaskEnterCritical();
	
	if( GetListCount(&memBlock->blockList) > 0){	//如果有空闲的存储块
		*mem = (uint8_t *)ListRemoveFirst(&memBlock->blockList);	//获取一个存储块
		TaskExitCritical(status);
		return NoneError;
	}
	else{
		TaskExitCritical(status);
		return ErrorResourceUnavaliable;	//返回资源不够用
	}
}

void MemBlockNotify(MemBlock * memBlock, uint8_t * mem){	//存储块的释放
	uint32_t status = TaskEnterCritical();
	if(EventWaitCount(&memBlock->event) > 0){	//如果有任务在等待存储块
		Task * task = EventWakeUp(&memBlock->event, (void *)mem, NoneError);	//唤醒一个任务，并给它分配存贮块
		if(task->priority < currentTask->priority){	//如果唤醒的任务优先级更高的话，进行任务调度
			TaskSched();
		}
	}
	else{
		ListAddHead(&memBlock->blockList, (Node *)mem);		//没有任务在等待存储块插入到存储块列表
	}
	TaskExitCritical(status);
}











