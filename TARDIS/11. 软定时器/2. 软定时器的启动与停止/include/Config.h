#ifndef TARDIS_OS_CONFIG_H
#define TARDIS_OS_CONFIG_H

#define TARDIS_OS_PRIORITY_COUNT        32	//优先级数量&位图长度
#define TARDIS_OS_SLICE_MAX				10	//时间片的大小

#define TARDIS_OS_IDLETASK_STACK_SIZE   1024//空闲任务堆栈的大小

#define TARDIS_OS_TIMERTASK_STACK_SIZE  1024//定时器任务的堆栈大小
#define TARDIS_OS_TIMERTASK_PRIO		1	//定时器任务的优先级
#endif

