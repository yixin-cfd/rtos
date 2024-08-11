#ifndef TARDIS_OS_H
#define TARDIS_OS_H

#include <stdint.h>
#include "Config.h"
#include "DS_Lib.h"
#include "Task.h"
#include "Event.h"
#include "Semaphore.h"

typedef enum _Error{			//事件结果错误码
	NoneError = 0,
	ErrorTimeout = 1,
}Error;


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


void TaskSchedRemove(Task * task);		//将任务从就绪队列中移除
void TimeTaskRemove(Task *task);		//将任务从延时队列中移除

void InitApp(void);						//应用初始化函数、即多个任务的初始化函数

#endif
