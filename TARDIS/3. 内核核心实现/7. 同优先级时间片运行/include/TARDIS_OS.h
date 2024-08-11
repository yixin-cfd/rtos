#ifndef TARDIS_OS_H
#define TARDIS_OS_H

#include <stdint.h>
#include "DS_Lib.h"
#include "Config.h"

#define TARDIS_OS_TASK_STATE_RDY 				0
#define TARDIS_OS_TASK_STATE_DELAYED 			(1<<1)

typedef uint32_t TaskStack;
typedef struct _Task{
	TaskStack * stack;					//堆栈指针
	Node  linkNode;						//优先级队列链接节点
	uint32_t delayTicks;				//延时时间
	Node   delayNode;					//延时链接节点
	uint32_t priority;					//优先级
	uint32_t state;						//任务状态
	uint32_t slice;						//时间片时长
}Task;


extern Task * currentTask;
extern Task * nextTask;


uint32_t TaskEnterCritical(void);		//临界区保护:进入临界区
void TaskExitCritical(uint32_t);		//临界区保护:退出临界区

void TaskRunFirst(void);
void TaskSwitch(void);



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
//任务初始化函数
void TaskInit(Task *task,void (*entry)(void *),void *param,uint32_t priority,TaskStack *stack);
#endif
