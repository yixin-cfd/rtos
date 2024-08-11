#ifndef TARDIS_OS_H
#define TARDIS_OS_H
#include<stdint.h>

typedef uint32_t TaskStack;
typedef struct _Task{
	TaskStack * stack;			//堆栈指针
	uint32_t delayTicks;		//延时时间
}Task;


extern Task * currentTask;
extern Task * nextTask;


uint32_t TaskEnterCritical(void);		//临界区保护:进入临界区
void TaskExitCritical(uint32_t);		//临界区保护:退出临界区

void TaskRunFirst(void);
void TaskSwitch(void);
#endif
