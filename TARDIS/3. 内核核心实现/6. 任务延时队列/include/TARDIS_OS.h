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
	uint32_t delayTicks;				//延时时间
	Node   delayNode;					//延时节点
	uint32_t priority;					//优先级
	uint32_t state;						//任务状态
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


#endif
