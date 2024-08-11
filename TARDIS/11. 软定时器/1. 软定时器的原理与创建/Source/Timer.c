#include "TARDIS_OS.h"


void TimerInit(Timer * timer, uint32_t startDelayTicks, uint32_t durationTicks, void (*timerFunc)(void * arg), void * arg, uint32_t config)
{
	NodeInit(&timer->linkNode);
	timer->startDelayTicks = startDelayTicks;
	timer->durationTicks = durationTicks;
	timer->timerFunc = timerFunc;
	timer->arg = arg;
	timer->config = config;
	
	if(startDelayTicks == 0){	//�����ʼ����ֵΪ��
		timer->delayTicks = durationTicks;//��ô����ֵΪ���ڼ���ֵ
	}
	else{
		timer->delayTicks = startDelayTicks;//����Ϊ��ʼ����ֵ
	}
	
	timer->state = TimerCreated;
}










