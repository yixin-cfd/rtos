#ifndef TARDIS_OS_CONFIG_H
#define TARDIS_OS_CONFIG_H

#define TARDIS_OS_PRIORITY_COUNT        32	//优先级数量&位图长度
#define TARDIS_OS_SLICE_MAX				10	//时间片的大小

#define TARDIS_OS_IDLETASK_STACK_SIZE   1024//空闲任务堆栈的大小

#define TARDIS_OS_TIMERTASK_STACK_SIZE  1024//定时器任务的堆栈大小
#define TARDIS_OS_TIMERTASK_PRIO		1	//定时器任务的优先级

#define TARDIS_OS_SYSTICK_MS			10	//系统时钟systick的中断周期



/*************************内核裁剪*****************************************/
#define	TARDIS_OS_ENABLE_SEMAPHORE		0	//信号量
#define TARDIS_OS_ENABLE_MESSAGEBOX     0   //邮箱
#define TARDIS_OS_ENABLE_MEMORYBLOCK	0	//存储块
#define TARDIS_OS_ENABLE_FLAGGROUP		0	//事件标志组
#define TARDIS_OS_ENABLE_MUTEX			0	//互斥信号量
#define TARDIS_OS_ENABLE_TIMER			0	//软定时器
#define TARDIS_OS_ENABLE_CPUUSAGE_STAT	0	//CPU利用率
/*************************内核裁剪*****************************************/
#endif























