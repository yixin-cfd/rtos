#include "TARDIS_OS.h"
#include "ARMCM4.h"
Task * currentTask;
Task * nextTask;
Task * idleTaskPtr;
uint8_t schedLockCount;	//调度锁锁定计数值

uint32_t tickCount;		//系统时钟节拍发生次数
uint32_t idleCount;		//空闲任务计数值
uint32_t idleMaxCount;	//空闲任务最大计数值
uint32_t FirstSec;		//当前计数值是不是第一秒

Bitmap taskPriorityBitamp;	//优先级位图
List taskTable[TARDIS_OS_PRIORITY_COUNT];

List taskDelayedList;		//任务延时队列


static void initCpuUsageState(void);		//Cpu使用率相关变量初始化 
static void checkCpuUsage(void);			//检查Cpu的使用率
static void cpuUsageSynWithSysTick (void);	//实现时钟同步，当空闲任务开始计数时时钟节拍刚开始

void TaskSchedRdy(Task * task){
	ListAddHead(&taskTable[task->priority],&task->linkNode);
	BitmapSet(&taskPriorityBitamp,task->priority); 
}
void TaskSchedUnRdy(Task * task){								//将任务从就绪状态中移除
	ListRemoveNode(&taskTable[task->priority],&task->linkNode);	//将任务从就绪队列中移除	
	if(GetListCount(&taskTable[task->priority])==0){			//如果该优先级的队列没有任务，清除位图标记
		BitmapClear(&taskPriorityBitamp,task->priority); 		//清除位图标记
	}
}

void TaskSchedRemove(Task * task){								//将任务从就绪队列中移除
	ListRemoveNode(&taskTable[task->priority],&task->linkNode);	//从就绪队列中移除	
	if(GetListCount(&taskTable[task->priority])==0){			//如果该优先级的队列没有任务，清除位图标记
		BitmapClear(&taskPriorityBitamp,task->priority); 		//清除位图标记
	}
}
Task * TaskHighestReady(void){
	uint32_t hightPriority = GetBitmapFirstSet(&taskPriorityBitamp);
	return NodeParent(ListFirstNode(&taskTable[hightPriority]),Task,linkNode);
}
void TimeTaskWait(Task * task,uint32_t ticks){
	task->delayTicks = ticks;
	ListAddHead(&taskDelayedList,&task->delayNode);
	task->state |= TARDIS_OS_TASK_STATE_DELAYED;

}

void TimeTaskWakeUp(Task *task){						//从延时队列中唤醒任务		
	ListRemoveNode(&taskDelayedList,&(task->delayNode));//从延时队列中移除任务
	task->state &= ~TARDIS_OS_TASK_STATE_DELAYED;		//修改任务状态
}
void TimeTaskRemove(Task *task){						//从延时队列中移除任务
	ListRemoveNode(&taskDelayedList,&(task->delayNode));//移除任务
}
void TaskSched(void){
	Task * tempTask;
	uint32_t status = TaskEnterCritical();
	
	if(schedLockCount > 0){					//如果有锁，调度器直接退出，继续执行当前任务
		TaskExitCritical(status);
		return;
	}
	tempTask = TaskHighestReady();
	if( currentTask != tempTask){
		nextTask = tempTask;
		TaskSwitch();
	}
	TaskExitCritical(status);
}

void TaskSchedInit(void){	
	int i;	
	schedLockCount = 0;
	BitmapInit(&taskPriorityBitamp);
	for(i = 0;i< TARDIS_OS_PRIORITY_COUNT;i++){
		ListInit(&taskTable[i]);
	}
}
void TaskSchedDisable(void){				//调度器加锁
	uint32_t status = TaskEnterCritical();
	if(schedLockCount < 255){
		schedLockCount++;
	}
	TaskExitCritical(status);
}
void TaskSchedEnable(void){					//调度器解锁
	uint32_t status = TaskEnterCritical();
	if(schedLockCount > 0){
		if(--schedLockCount == 0){
			TaskSched();
		}
	}
	TaskExitCritical(status);
}



void taskDelayedInit(void){
	ListInit(&taskDelayedList);
}

void TimeTickInit(void)		//时钟计数值初始化
{
	tickCount = 0;
}

void TaskSystemTickHandler(){
	Node *node;
	uint32_t status = TaskEnterCritical();
	for(node = taskDelayedList.headNode.nextNode;node != &(taskDelayedList.headNode);node = node->nextNode){
		Task * task = NodeParent(node,Task,delayNode);
		if(--task->delayTicks == 0){
			
			if(task->waitEvent){		//如果任务再等待事件
				EventRemoveTask(task, (void *)0, ErrorTimeout);	//从等待事件队列中删除，等待结果为超时 
			}
			
			TimeTaskWakeUp(task);
			
			TaskSchedRdy(task);
		}
	}
	
	if(--currentTask->slice==0){
		if(GetListCount(&taskTable[currentTask->priority])!=0){
			ListRemoveFirst(&taskTable[currentTask->priority]);
			ListAddTail(&taskTable[currentTask->priority],&currentTask->linkNode);
			
			currentTask->slice = TARDIS_OS_SLICE_MAX;
		}
	}
	tickCount++;		//计数值自增一
	checkCpuUsage();	//检查Cpu的使用率
	TaskExitCritical(status);
	
	TimerModuleTickNotify();	//软定时器中断处理
	
	TaskSched();
}
static float cpuUsage;
static uint32_t enableCpuUsageState;
static void initCpuUsageState(void)	//Cpu使用率相关变量初始化 
{
	FirstSec = 1;	//是第一秒
	idleCount = 0;
	idleMaxCount = 0;
	cpuUsage = 0.0f;
	enableCpuUsageState = 0;
}
static void checkCpuUsage(void)		//检查Cpu的使用率
{
	if(enableCpuUsageState == 0){	//第一次进入时钟中断，还没有同步
		enableCpuUsageState = 1;	//同步
		tickCount = 0;				//开始计数
		return;
	}
	if(FirstSec && (tickCount == TICKS_PER_SEC)){	//第一秒
		idleMaxCount = idleCount;
		idleCount = 0;
		TaskSchedEnable();		//允许调度
		FirstSec = 0;			//不是第一秒
	}
	else if((FirstSec == 0 )&&(tickCount % TICKS_PER_SEC == 0)){
		cpuUsage = 100 - (idleCount * 100.0 / idleMaxCount);
		idleCount = 0;
	}
}

static void cpuUsageSynWithSysTick (void)	//实现时钟同步，当空闲任务开始计数时时钟节拍刚开始
{
	while(enableCpuUsageState == 0);
}
float CpuUsageGet(void){	//获取CPU使用率
	float usage = 0;
	uint32_t status = TaskEnterCritical();
	usage = cpuUsage;
	TaskExitCritical(status);
	return usage;
}

//定义空闲任务
Task idleTask;
TaskStack idleTaskEnv[TARDIS_OS_IDLETASK_STACK_SIZE];
void idleTaskEntry(void *param){
	
	TaskSchedDisable();	//开启调度锁，初始时一秒钟内只能在空闲任务中运行
	
	InitApp();
	TimerInitTask();	//软定时器初始化 & 创建一个定时器任务
	
	SetSysTickPeriod(TARDIS_OS_SYSTICK_MS);	//配置系统定时器
	
	cpuUsageSynWithSysTick();		//时钟同步
	
	for(;;){
		uint32_t status = TaskEnterCritical();
		idleCount++;
		TaskExitCritical(status);
	}
}
 int main(void){

	TaskSchedInit();		//优先级队列初始化
	taskDelayedInit();		//延时队列初始化
	TimerModuleInit();		//软定时器初始化
	TimeTickInit();			//时钟计数值初始化
	initCpuUsageState();	//Cpu使用率相关变量初始化 
	TaskInit(&idleTask,idleTaskEntry,(void *)0x00,TARDIS_OS_PRIORITY_COUNT - 1,idleTaskEnv, sizeof(idleTaskEnv));
	idleTaskPtr = &idleTask;
	nextTask = TaskHighestReady();
	TaskRunFirst();
	
	return 0;
}


