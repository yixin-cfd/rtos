#ifndef _TASK_H
#define _TASK_H

#include <stdint.h>
#include "DS_Lib.h"

#define TARDIS_OS_TASK_STATE_RDY 				0				//任务就绪状态
#define TARDIS_OS_TASK_STATE_DESTORYED 			(1 << 1)		//任务删除状态
#define TARDIS_OS_TASK_STATE_DELAYED 			(1 << 2)		//任务延时阻塞状态
#define TARDIS_OS_TASK_STATE_SUSPEND			(1 << 3)		//任务挂起状态

//高16位用作等待事件的标志
#define TARDIS_OS_TASK_WAIT_MASK				(0xFF << 16)	//等待事件的掩码

struct _Event;

typedef uint32_t TaskStack;
typedef struct _Task{
	TaskStack * stack;					//堆栈指针
	Node  linkNode;						//链接节点;链接到各种队列中；如：优先级队列、等待事件队列
	uint32_t delayTicks;				//延时时间
	Node   delayNode;					//延时链接节点
	uint32_t priority;					//优先级
	uint32_t state;						//任务状态
	uint32_t slice;						//时间片大小
	uint32_t suspendCount;				//挂起计数器
	
	void (*clean)(void * param);		//任务删除函数
	void * cleanParam;					//删除的参数
	uint8_t requestDeleteFlag;			//请求删除标记
	
	struct _Event * waitEvent;			//任务等待哪个事件控制块
	void * eventMsg;					//事件相关的数据存放位置
	uint32_t waitEventResult;			//等待事件的结果
	
	uint32_t waitFlagsType;				//等待事件类型
	uint32_t eventFlags;				//等待事件标记
}Task;

typedef struct _TaskInfo{
	uint32_t delayTicks;				//延时时间
	uint32_t priority;					//优先级
	uint32_t state;						//任务状态
	uint32_t slice;						//时间片大小
	uint32_t suspendCount;				//挂起计数器
}TaskInfo;								//任务状态结构体
void GetTaskInfo(Task * task, TaskInfo * info);		//任务状态查询函数
void TaskInit(Task *task,void (*entry)(void *),void *param,uint32_t priority,TaskStack *stack); //任务初始化函数

void TaskSuspend(Task * task);  		//任务挂起函数
void TaskWakeUp(Task * task);			//任务唤醒函数

void TaskSetCleanCallFunc(Task * task,void (*clean)(void * param),void * param); //设置任务删除函数
void TaskForceDelete(Task * task);		//将任务强制删除
void TaskRequestDelete(Task * task);	//请求删除任务
uint8_t TaskIsRequestDeleted (void);	//检查当前任务是否需要删除
void TaskDeleteSelf(void);          	//当前任务删除自己

#endif

