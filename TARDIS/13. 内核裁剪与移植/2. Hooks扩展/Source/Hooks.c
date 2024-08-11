#include "Hooks.h"

#if TARDIS_OS_ENABLE_HOOKS == 1
void HooksCpuIdle(void)	//空闲任务的钩子函数
{





}

void HooksSysTick(void)	//SysTick中断发生时的钩子函数
{



}


void HooksTaskSwitch(Task * from, Task * to)//任务切换的钩子函数
{



}

void HooksTaskInit(Task * task)//任务初始化的钩子函数	
{



}

#endif







