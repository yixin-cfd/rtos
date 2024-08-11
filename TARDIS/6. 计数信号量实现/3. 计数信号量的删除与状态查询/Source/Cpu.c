#include "TARDIS_OS.h"
#include "ARMCM4.h"
//配置定时器
void SetSysTickPeriod(uint32_t ms){
	SysTick->LOAD = ms * SystemCoreClock / 1000 - 1;
	NVIC_SetPriority(SysTick_IRQn,(1<<__NVIC_PRIO_BITS)-1);
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk|
					SysTick_CTRL_TICKINT_Msk|
					SysTick_CTRL_ENABLE_Msk;
}
//系统定时器的中断处理函数
void SysTick_Handler(){
	//时钟中断任务处理函数
	TaskSystemTickHandler();		//在终端中要对任务延时队列和任务时间片进行操作、并进行任务调度
}

