#include "TARDIS_OS.h"

Task Task1;				
Task Task2;
Task Task3;
Task Task4;

TaskStack Task1Env[2048];	
TaskStack Task2Env[2048];
TaskStack Task3Env[2048];
TaskStack Task4Env[2048];

typedef uint8_t (* Block)[100];
uint8_t mem1[20][100];
MemBlock memblock1;

int task1Flag;
void task1Entry(void * param){
		
	uint8_t i;
	Block block[20];
	SetSysTickPeriod(10);
	
	MemBlockInit(&memblock1, (uint8_t *)mem1, 100, 20);
	
	for(i=0;i<20;i++){
		MemBlockWait(&memblock1, (uint8_t **)&block[i], 0);
	}
	TaskDelay(2);
	for(i=0;i<20;i++){
		memset(block[i],i,100);
		MemBlockNotify(&memblock1, (uint8_t *)&block[i]);
		TaskDelay(2);
	}
	for(;;){	


		task1Flag = 0;
		TaskDelay(1);
		task1Flag = 1;
		TaskDelay(1);
	}
}

int task2Flag;
void task2Entry(void * param){		

	
	for(;;){
		
		Block block;
		
		MemBlockWait(&memblock1, (uint8_t **)&block, 0);

		task2Flag = *(uint8_t *)block;
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
	TaskInit(&Task3,task3Entry,(void *)0x03,2,&Task3Env[2048]);
	TaskInit(&Task4,task4Entry,(void *)0x04,2,&Task4Env[2048]);
}




