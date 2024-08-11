#ifndef TARDIS_OS_H
#define TARDIS_OS_H

#include<stdint.h>

typedef uint32_t TaskStack;


typedef struct _Task{
	TaskStack * stack;			//∂—’ª÷∏’Î
}Task;


extern Task * currentTask;
extern Task * nextTask;

void TaskRunFirst(void);
void TaskSwitch(void);
#endif
