#include "TARDIS_OS.h"

Task Task1;				
Task Task2;
Task Task3;
Task Task4;

TaskStack Task1Env[512];	
TaskStack Task2Env[512];
TaskStack Task3Env[512];
TaskStack Task4Env[512];

Semaphore sem1, sem2;


int task1Flag;


void task1Entry(void * param){
		

	SetSysTickPeriod(10);
	
	SemaphoreInit(&sem1, 0, 10);

	for(;;){	
		
		SemWait(&sem1, 0);
		
		task1Flag = 0;
		TaskDelay(1);
		task1Flag = 1;
		TaskDelay(1);
	}
}

int task2Flag;
void task2Entry(void * param){		

	uint32_t error = 0;
	for(;;){
		task2Flag = 0;
		TaskDelay(1);
		task2Flag = 1;
		TaskDelay(1);
		
		SemNotify(&sem1);
		
		error = SemNoWait(&sem1);
	}
}

int task3Flag;
void task3Entry(void * param){		

	SemaphoreInit(&sem2, 0, 0);
	for(;;){
		
		SemWait(&sem2, 10);

		task3Flag = 0;
		TaskDelay(1);
		task3Flag = 1;
		TaskDelay(1);
	}
}
int task4Flag;
void task4Entry(void * param){		//����4����ɾ������3

	for(;;){

		task4Flag = 0;
		TaskDelay(1);
		task4Flag = 1;
		TaskDelay(1);
		
	}
}


void InitApp(void){
	TaskInit(&Task1,task1Entry,(void *)0x01,0,&Task1Env[512]);
	TaskInit(&Task2,task2Entry,(void *)0x02,1,&Task2Env[512]);
	TaskInit(&Task3,task3Entry,(void *)0x03,1,&Task3Env[512]);
	TaskInit(&Task4,task4Entry,(void *)0x04,1,&Task4Env[512]);
}




