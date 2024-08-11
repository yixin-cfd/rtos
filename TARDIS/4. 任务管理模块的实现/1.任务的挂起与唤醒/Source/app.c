#include "TARDIS_OS.h"

Task Task1;				//定义两个任务
Task Task2;
Task Task3;
TaskStack Task1Env[24];	//定义两个堆栈
TaskStack Task2Env[24];
TaskStack Task3Env[24];

//定义任务对应的功能函数
int task1Flag;
void task1Entry(void * param){
	

	SetSysTickPeriod(10);

	for(;;){	
		task1Flag = 1;
		TaskSuspend(&Task1);
		task1Flag = 0;
		TaskSuspend(&Task1);
	}
}

int task2Flag;
void task2Entry(void * param){

	for(;;){
		task2Flag = 0;
		TaskDelay(1);
		TaskWakeUp(&Task1);
		task2Flag = 1;
		TaskDelay(1);
		TaskWakeUp(&Task1);
	}
}

int task3Flag;
void task3Entry(void * param){

	for(;;){
		task3Flag = 1;
		TaskDelay(1);
		task3Flag = 0;
		TaskDelay(1);
	}
}


void InitApp(void){
	TaskInit(&Task1,task1Entry,(void *)0x00,0,&Task1Env[24]);
	TaskInit(&Task2,task2Entry,(void *)0x00,1,&Task2Env[24]);
	TaskInit(&Task3,task3Entry,(void *)0x00,1,&Task3Env[24]);
}




