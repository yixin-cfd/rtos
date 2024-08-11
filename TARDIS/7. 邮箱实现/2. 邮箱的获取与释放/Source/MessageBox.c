#include "TARDIS_OS.h"


void MboxInit(Mbox * mbox, void ** msgBuffer, uint32_t maxCount)	//邮箱初始化
{
	EventInit(&mbox->event, EventTypeMbox);	//初始化事件控制块

	mbox->msgBuffer = msgBuffer;			//修改消息缓冲区
	mbox->maxCount = maxCount;				//消息最大数量
	mbox->read = 0;							//读索引
	mbox->write = 0;						//写索引
	mbox->count = 0;						//当前计数值为零
}

uint32_t MboxWait(Mbox * mbox, void **msg, uint32_t waitTicks)		//读取邮箱，等待消息
{
	uint32_t status = TaskEnterCritical();
	
	if( mbox->count > 0){	//如果邮箱有消息的话
		--mbox->count;		//消息数减少一
		*msg = mbox->msgBuffer[mbox->read++];	//利用读索引读取消息
		if( mbox->read >= mbox->maxCount){		//组织成循环队列
			mbox->read = 0;
		}
		TaskExitCritical(status);
		return NoneError;
	}
	else{
		EventWait( &mbox->event, currentTask, (void *)0, EventTypeMbox, waitTicks);	//将任务插入到事件控制块中
		TaskExitCritical(status);
		
		TaskSched();	//任务调度
		
		*msg = currentTask->eventMsg;			//取出任务等待的消息
		return currentTask->waitEventResult;	//返回任务等待结果
	}
}


uint32_t MboxNoWait(Mbox * mbox, void **msg)		//读取邮箱，不等待消息
{
	uint32_t status = TaskEnterCritical();
	
	if( mbox->count > 0){	//如果邮箱有消息的话
		--mbox->count;		//消息数减少一
		*msg = mbox->msgBuffer[mbox->read++];	//利用读索引读取消息
		if( mbox->read >= mbox->maxCount){		//组织成循环队列
			mbox->read = 0;
		}
		TaskExitCritical(status);
		return NoneError;
	}
	else{
		TaskExitCritical(status);
		return ErrorResourceUnavaliable;	//返回资源不够用
	}
}

uint32_t MboxNotify(Mbox * mbox, void * msg, uint32_t notifyOption)		//向邮箱发送消息
{
	uint32_t status = TaskEnterCritical();
	
	if( EventWaitCount(&mbox->event) > 0){		//如果有任务在等待
		Task * task = EventWakeUp( &mbox->event, msg, NoneError);	//唤醒一个任务
		if ( task->priority < currentTask->priority){			//如果唤醒的任务的优先级更高
			TaskSched();		//进行任务调度
		}
	}
	else{
		if( mbox->count == mbox->maxCount){		//如果消息已经满了
			TaskExitCritical(status);
			return ErrorResourceFull;			//返回资源已满
		}
		//消息未满
		if(notifyOption & MBOXSendFront){		//如果是高优先级的消息，插到头部
			if(mbox->read <= 0){
				mbox->read = mbox->maxCount - 1;
			}
			else{
				--mbox->read;
			}
			mbox->msgBuffer[mbox->read] = msg;
		}
		else{	//普通方式插入 	
			mbox->msgBuffer[mbox->write++] = msg;
			if( mbox->write == mbox->maxCount){
				mbox->write = 0;
			}
		}
		mbox->count++;
	}
	
	TaskExitCritical(status);
	return NoneError;
}


