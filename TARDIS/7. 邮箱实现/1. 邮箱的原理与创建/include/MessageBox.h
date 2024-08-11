#ifndef _MESSAGEBOX_H
#define _MESSAGEBOX_H

#include "Event.h"

typedef struct _Mbox{
	Event event;		//事件控制块
	uint32_t count;		//当前的消息数量
	uint32_t read;		//读取消息的索引
	uint32_t write;		//写消息的索引
	uint32_t maxCount;	//最大允许容纳的消息数量
	void ** msgBuffer;	//消息存储缓冲区
}Mbox;

void MboxInit(Mbox * mbox, void ** msgBuffer, uint32_t maxCount);	//邮箱初始化

#endif

