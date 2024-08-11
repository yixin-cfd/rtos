#include "TARDIS_OS.h"

Task Task1;				
Task Task2;
Task Task3;
Task Task4;

TaskStack Task1Env[24];	
TaskStack Task2Env[24];
TaskStack Task3Env[24];
TaskStack Task4Env[24];

int task1Flag;

void task1DestoryFunc(void * param){
	task1Flag = 1;
}

void task1Entry(void * param){
	

	SetSysTickPeriod(10);

	TaskSetCleanCallFunc(currentTask,task1DestoryFunc,(void *)0);
	for(;;){	
		task1Flag = 1;
		TaskDelay(1);
		task1Flag = 0;
		TaskDelay(1);
	}
}

int task2Flag;
void task2Entry(void * param){		//任务2删除任务1

	uint8_t task1Deleted = 0;
	for(;;){
		task2Flag = 0;
		TaskDelay(1);
		task2Flag = 1;
		TaskDelay(1);
		
		if(!task1Deleted){
			TaskForceDelete(&Task1);
			task1Deleted = 1;
		}
	}
}

int task3Flag;
void task3Entry(void * param){		//任务3自己删除自己

	for(;;){
		if( TaskIsRequestDeleted()){
			TaskDeleteSelf();
			task3Flag = 0;
		}
		
		task3Flag = 1;
		TaskDelay(1);
		task3Flag = 0;
		TaskDelay(1);
	}
}
int task4Flag;
void task4Entry(void * param){		//任务4请求删除任务3

	uint8_t task3Deleted = 0; 
	for(;;){
		task4Flag = 1;
		TaskDelay(1);
		task4Flag = 0;
		TaskDelay(1);
		
		if(!task3Deleted){
			TaskRequestDelete(&Task3);
			task3Deleted = 1;
		}
	}
}


void InitApp(void){
	TaskInit(&Task1,task1Entry,(void *)0x01,0,&Task1Env[24]);
	TaskInit(&Task2,task2Entry,(void *)0x02,1,&Task2Env[24]);
	TaskInit(&Task3,task3Entry,(void *)0x03,0,&Task3Env[24]);
	TaskInit(&Task4,task4Entry,(void *)0x04,1,&Task4Env[24]);
}




