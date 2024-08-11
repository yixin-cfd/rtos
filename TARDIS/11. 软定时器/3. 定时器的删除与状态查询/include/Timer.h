#ifndef _TIMER_H
#define _TIMER_H

#include "DS_Lib.h"

#define TIMER_CONFIG_TYPE_HARD			(1 << 0)	//在systick中断中处理定时器
#define TIMER_CONFIG_TYPE_SOFT			(0 << 0)	//在定时器任务中处理

typedef enum _TimerState{
	TimerCreated,		//定时器创建好了
	TimerStarted,		//定时器正在启动
	TimerRunning,		//定时器正在运行
	TimerStopped,		//定时器停止
	TimerDestoryed		//定时器已经被销毁了
}TimerState;


typedef struct _Timer{
	Node linkNode;				//链接节点
	uint32_t startDelayTicks;	//初始等待时间
	uint32_t durationTicks;		//周期延时时间
	uint32_t delayTicks;		//当前计数值
	void (*timerFunc)(void * arg);	//定时器处理函数
	void * arg;					//参数
	uint32_t config;			//定时器配置参数 
	
	TimerState state;			//定时器状态
}Timer;

typedef struct _TimerInfo{
	uint32_t startDelayTicks;	//初始等待时间
	uint32_t durationTicks;		//周期延时时间
	void (*timerFunc)(void * arg);	//定时器处理函数
	void * arg;					//参数
	uint32_t config;			//定时器配置参数 
	
	TimerState state;			//定时器状态
}TimerInfo;


//定时器节点初始化函数
void TimerInit(Timer * timer, uint32_t startDelayTicks, uint32_t durationTicks, void (*timerFunc)(void * arg), void * arg, uint32_t config);
void TimerStart(Timer * timer);	//定时器启动
void TimerStop(Timer * timer);	//定时器停止
void TimerModuleInit(void);	//定时器模块初始化
void TimerModuleTickNotify(void);//定时器中断处理

void TimerDestory(Timer * timer);	//删除定时器
void GetTimerInfo(Timer * timer, TimerInfo * info);//状态查询
#endif



