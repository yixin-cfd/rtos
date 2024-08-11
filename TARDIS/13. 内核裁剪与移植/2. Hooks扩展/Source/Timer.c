#include "TARDIS_OS.h"


#if TARDIS_OS_ENABLE_TIMER == 1
static List TimerHardList;		//硬件定时器队列，中断中处理
static List TimerSoftList;		//定时器任务处理的队列

static Semaphore TimerProtectSem;	//定时器任务和其他任务访问TimerSoftList的信号量
static Semaphore TimerTickSem;		//在中断处理程序中通知定时器任务有中断发生

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

static Task TimerTask;
static TaskStack TimerTaskStack[TARDIS_OS_TIMERTASK_STACK_SIZE];

static void TimerCallFuncList(List * timerList){	//遍历列表，调用定时器处理函数
	Node * node, *nextNode;
	for(node = timerList->headNode.nextNode; node != &(timerList->headNode); node = nextNode){
		Timer * timer = NodeParent(node, Timer, linkNode);
		nextNode = node->nextNode;
		if( (timer->delayTicks == 0) || (--timer->delayTicks == 0)){
			timer->state = TimerRunning;
			timer->timerFunc(timer->arg);
			timer->state = TimerStarted;
			
			if(timer->durationTicks > 0){
				timer->delayTicks = timer->durationTicks;
			}
			else{
				ListRemoveNode(timerList, &timer->linkNode);
				timer->state =TimerStopped;
			}
		}
	}
}
static void TimerSoftTask(void * param)	//定时器任务
{
	for(;;)
	{
		SemWait(&TimerTickSem, 0);		//等待系统时钟中断发生
		SemWait(&TimerProtectSem, 0);	//定时器拥有对TimerSoftList的访问权
		TimerCallFuncList(&TimerSoftList);//执行定时器函数
		SemNotify(&TimerProtectSem);	//允许其他任务访问对TimerSoftList的
	}
}

void TimerModuleInit(void)	//定时器初始化
{
	ListInit(&TimerHardList);
	ListInit(&TimerSoftList);
	
	SemaphoreInit(&TimerProtectSem, 1, 1);
	SemaphoreInit(&TimerTickSem, 0, 0);
}
void TimerInitTask(void)	//定时器初始化
{
#if TARDIS_OS_TIMERTASK_PRIO >= (TARDIS_OS_PRIORITY_COUNT - 1)	//定时器任务的优先级要比空闲任务的优先级要高
	#error " The priority of timer task must be greater than (TARDIS_OS_PRIORITY_COUNT - 1)"
#endif
	
	TaskInit(&TimerTask, TimerSoftTask, (void *)0, TARDIS_OS_TIMERTASK_PRIO, TimerTaskStack, sizeof(TimerTaskStack));
}

void TimerStart(Timer * timer)	//定时器启动
{
	switch(timer->state)
	{
		//定时器只有处于创建和停止状态才能启动
		case TimerCreated:					
		case TimerStopped:
			timer->delayTicks = timer->startDelayTicks ? timer->startDelayTicks : timer->durationTicks;//定时器装载初始值
			timer->state = TimerStarted;	//定时器状态为已经启动
		
			if(timer->config & TIMER_CONFIG_TYPE_HARD) //决定定时器插入到那个队列中
			{	//插入到硬件定时器队列中
				uint32_t status = TaskEnterCritical();
				
				ListAddHead(&TimerHardList, &timer->linkNode);	//中断中也访问这个列表了，用关中断方式
				
				TaskExitCritical(status);
			}
			else{
				//插入到定时器任务队列中
				SemWait(&TimerProtectSem, 0);
				
				ListAddHead(&TimerSoftList, &timer->linkNode);	//在不同任务中访问这个列表，用信号量的方式
				
				SemNotify(&TimerProtectSem);
			}
			break;
		default:
			break;
	}
}


void TimerStop(Timer * timer){	//定时器停止
	switch(timer->state)
	{
		case TimerStarted:
		case TimerRunning:
			if(timer->config & TIMER_CONFIG_TYPE_HARD) //定时器在哪个队列中
			{	//在硬件定时器队列中
				uint32_t status = TaskEnterCritical();
				
				ListRemoveNode(&TimerHardList, &timer->linkNode);	//移除定时器
				
				TaskExitCritical(status);
			}
			else{
				//在定时器任务队列中
				SemWait(&TimerProtectSem, 0);
				
				ListRemoveNode(&TimerSoftList, &timer->linkNode);	//移除定时器
				
				SemNotify(&TimerProtectSem);
			}
			break;
		default:
			break;
	}
}

void TimerDestory(Timer * timer)	//删除定时器
{
	TimerStop(timer);
	timer->state = TimerDestoryed;
}

void GetTimerInfo(Timer * timer, TimerInfo * info)//状态查询
{
	uint32_t status = TaskEnterCritical();

	info->startDelayTicks = timer->startDelayTicks;
	info->durationTicks = timer->delayTicks;
	info->timerFunc = timer->timerFunc;
	info->arg = timer->arg;
	info->config = timer->config;
	info->state = timer->state;
	
	TaskExitCritical(status);
}


void TimerModuleTickNotify(void){	//定时器中断处理
	uint32_t status = TaskEnterCritical();
	
	TimerCallFuncList(&TimerHardList);	//处理硬件定时器队列
	
	TaskExitCritical(status);
	
	SemNotify(&TimerTickSem);	//通知信号量
}

#endif

