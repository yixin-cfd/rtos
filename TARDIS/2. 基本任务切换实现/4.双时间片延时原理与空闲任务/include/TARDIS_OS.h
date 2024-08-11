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

void TaskRunFirst(void);
void TaskSwitch(void);
#endif
