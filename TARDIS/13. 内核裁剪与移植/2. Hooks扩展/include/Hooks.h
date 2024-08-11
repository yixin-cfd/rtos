#ifndef _HOOKS_H
#define _HOOKS_H

#include "TARDIS_OS.h"

void HooksCpuIdle(void);	//空闲任务的钩子函数
void HooksSysTick(void);	//SysTick中断发生时的钩子函数
void HooksTaskSwitch(Task * from, Task * to);//任务切换的钩子函数
void HooksTaskInit(Task * task);//任务初始化的钩子函数		
#endif


