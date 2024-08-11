#include "TARDIS_OS.h"


//任务延时函数
void TaskDelay(uint32_t delay){					
	uint32_t status = TaskEnterCritical();
	TimeTaskWait(currentTask,delay);  //任务延时设置函数，修改任务的延时时间、将任务插入到延时队列中、修改任务的状态为延时状态		
	TaskSchedUnRdy(currentTask);	  //将任务从就绪队列中删除，如果这个优先级中没有任务，修改位图
	TaskExitCritical(status);		  
	TaskSched();
}

