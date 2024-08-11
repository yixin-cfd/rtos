#ifndef _MESSAGEBOX_H
#define _MESSAGEBOX_H

#include "Event.h"

#define MBOXSendNormal			0x00		
#define MBOXSendFront		 	0x01

typedef struct _Mbox{
	Event event;		//事件控制块
	uint32_t count;		//当前的消息数量
	uint32_t read;		//读取消息的索引
	uint32_t write;		//写消息的索引
	uint32_t maxCount;	//最大允许容纳的消息数量
	void ** msgBuffer;	//消息存储缓冲区
}Mbox;


typedef struct _MboxInfo{
	uint32_t count;		//当前的消息数量
	uint32_t maxCount;	//最大允许容纳的消息数量
	uint32_t taskCount;	//当前等待的任务数
}MboxInfo;

void MboxInit(Mbox * mbox, void ** msgBuffer, uint32_t maxCount);	//邮箱初始化
uint32_t MboxWait(Mbox * mbox, void **msg, uint32_t waitTicks);		//读取邮箱，等待消息
uint32_t MboxNoWait(Mbox * mbox, void **msg);						//读取邮箱，不等待消息
uint32_t MboxNotify(Mbox * mbox, void * msg, uint32_t notifyOption);//向邮箱发送消息

void MboxClear(Mbox * mbox);		//清空邮箱
uint32_t  MboxDestory(Mbox * mbox);	//删除邮箱

void GetMboxInfo(Mbox * mbox, MboxInfo * info);		//状态查询
#endif

