#ifndef TARDIS_OS_H
#define TARDIS_OS_H

#include <stdint.h>
#include "Config.h"
#include "DS_Lib.h"
#include "Event.h"

typedef enum _Error{			//事件结果错误码
	NoneError = 0,
}Error;

#define TARDIS_OS_TASK_STATE_RDY 				0				//任务就绪状态
#define TARDIS_OS_TASK_STATE_DESTORYED 			( 1 << 1 )		//任务删除状态
#define TARDIS_OS_TASK_STATE_DELAYED 			( 1 << 2 )		//任务延时阻塞状态
#define TARDIS_OS_TASK_STATE_SUSPEND			( 1 << 3 )		//任务挂起状态

typedef uint32_t TaskStack;
typedef struct _Task{
	TaskStack * stack;					//堆栈指针
	Node  linkNode;						//优先级队列链接节点
	uint32_t delayTicks;				//延时时间
	Node   delayNode;					//延时链接节点
	uint32_t priority;					//优先级
	uint32_t state;						//任务状态
	uint32_t slice;						//时间片大小
	uint32_t suspendCount;				//挂起计数器
	
	void (*clean)(void * param);		//任务删除函数
	void * cleanParam;					//删除的参数
	uint8_t requestDeleteFlag;			//请求删除标记
	
	Event * waitEvent;					//任务等待哪个事件控制块
	void * eventMsg;					//事件相关的数据存放位置
	uint32_t waitEventResult;			//等待事件的结果
}Task;


extern Task * currentTask;
extern Task * nextTask;


uint32_t TaskEnterCritical(void);		//临界区保护:进入临界区
void TaskExitCritical(uint32_t);		//临界区保护:退出临界区

void TaskRunFirst(void);				//运行第一个任务
void TaskSwitch(void);					//切换任务



void TaskSchedInit(void);				//调度器初始化
void TaskSchedDisable(void);			//调度器加锁
void TaskSchedEnable(void);				//调度器解锁
void TaskSched(void);					//开始调度

void TaskSchedRdy(Task * task);			//向就绪列表中插入，并修改位图
void TaskSchedUnRdy(Task * task);		//从就绪列表中删除，并按需要决定是否从位图中删除

void TimeTaskWait(Task * task,uint32_t ticks);	//插入延时队列
void TimeTaskWakeUp(Task *task);				//从延时队列中删除


void SetSysTickPeriod(uint32_t ms);		//配置定时器
void TaskSystemTickHandler(void);		//定时器中断处理函数
void TaskDelay(uint32_t delay);			//延时函数

void TaskSuspend(Task * task);  		//任务挂起函数
void TaskWakeUp(Task * task);			//任务唤醒函数

void TaskSchedRemove(Task * task);		//将任务从就绪队列中移除
void TimeTaskRemove(Task *task);		//将任务从延时队列中移除


void TaskSetCleanCallFunc(Task * task,void (*clean)(void * param),void * param); //设置任务删除函数
void TaskForceDelete(Task * task);		//将任务强制删除
void TaskRequestDelete(Task * task);	//请求删除任务
uint8_t TaskIsRequestDeleted (void);	//检查当前任务是否需要删除
void TaskDeleteSelf(void);          	//当前任务删除自己

typedef struct _TaskInfo{
	uint32_t delayTicks;				//延时时间
	uint32_t priority;					//优先级
	uint32_t state;						//任务状态
	uint32_t slice;						//时间片大小
	uint32_t suspendCount;				//挂起计数器
}TaskInfo;								//任务状态结构体
void GetTaskInfo(Task * task, TaskInfo * info);		//任务状态查询函数

void TaskInit(Task *task,void (*entry)(void *),void *param,uint32_t priority,TaskStack *stack); //任务初始化函数
void InitApp(void);						//应用初始化函数、即多个任务的初始化函数

#endif
