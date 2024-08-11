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

