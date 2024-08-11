#include "TARDIS_OS.h"


void TimerInit(Timer * timer, uint32_t startDelayTicks, uint32_t durationTicks, void (*timerFunc)(void * arg), void * arg, uint32_t config)
{
	NodeInit(&timer->linkNode);
	timer->startDelayTicks = startDelayTicks;
	timer->durationTicks = durationTicks;
	timer->timerFunc = timerFunc;
	timer->arg = arg;
	timer->config = config;
	
	if(startDelayTicks == 0){	//如果初始计数值为零
		timer->delayTicks = durationTicks;//那么计数值为周期计数值
	}
	else{
		timer->delayTicks = startDelayTicks;//否则为初始计数值
	}
	
	timer->state = TimerCreated;
}










