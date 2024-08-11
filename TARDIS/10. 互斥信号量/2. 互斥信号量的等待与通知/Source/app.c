#include "TARDIS_OS.h"

Task Task1;				
Task Task2;
Task Task3;
Task Task4;

TaskStack Task1Env[2048];	
TaskStack Task2Env[2048];
TaskStack Task3Env[2048];
TaskStack Task4Env[2048];

Mutex mutex1;

int task1Flag;
void task1Entry(void * param){
		

	SetSysTickPeriod(10);

	MutexInit(&mutex1);
	for(;;){	
		MutexWait(&mutex1, 0);
		MutexWait(&mutex1, 0);
		task1Flag = 0;
		TaskDelay(1);
		task1Flag = 1;
		TaskDelay(1);
		
		MutexNotify(&mutex1);
		MutexNotify(&mutex1);
	}
}

int task2Flag;
void task2Entry(void * param){		


	for(;;){
		MutexWait(&mutex1, 0);
		MutexWait(&mutex1, 0);
		task2Flag = 0;
		TaskDelay(1);
		task2Flag = 1;
		TaskDelay(1);
		MutexNotify(&mutex1);
		MutexNotify(&mutex1);
	}
}

int task3Flag;
void task3Entry(void * param){		


	for(;;){	

		task3Flag = 0;
		TaskDelay(1);
		task3Flag = 1;
		TaskDelay(1);
	}
}
int task4Flag;
void task4Entry(void * param){		//任务4请求删除任务3

	for(;;){
		task4Flag = 0;
		TaskDelay(1);
		task4Flag = 1;
		TaskDelay(1);
		
	}
}


void InitApp(void){
	TaskInit(&Task1,task1Entry,(void *)0x01,0,&Task1Env[2048]);
	TaskInit(&Task2,task2Entry,(void *)0x02,1,&Task2Env[2048]);
	TaskInit(&Task3,task3Entry,(void *)0x03,1,&Task3Env[2048]);
	TaskInit(&Task4,task4Entry,(void *)0x04,1,&Task4Env[2048]);
}




