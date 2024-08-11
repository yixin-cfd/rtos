#include "TARDIS_OS.h"

Task Task1;				
Task Task2;
Task Task3;
Task Task4;

TaskStack Task1Env[2048];	
TaskStack Task2Env[2048];
TaskStack Task3Env[2048];
TaskStack Task4Env[2048];

Timer timer1;
Timer timer2;
Timer timer3;

uint32_t bit1 = 0;
uint32_t bit2 = 0;
uint32_t bit3 = 0;

void timeFunc(void * arg){
	uint32_t * ptrBit = (uint32_t *)arg;
	*ptrBit ^= 0x1;
}


int task1Flag;
void task1Entry(void * param){
		
	uint8_t stopped = 0;

	SetSysTickPeriod(10);

	TimerInit(&timer1, 100, 10, timeFunc, (void *)&bit1, TIMER_CONFIG_TYPE_HARD);
	TimerStart(&timer1);
	
	TimerInit(&timer2, 200, 20, timeFunc, (void *)&bit2, TIMER_CONFIG_TYPE_HARD);
	TimerStart(&timer2);
	
	TimerInit(&timer3, 300, 0, timeFunc, (void *)&bit3, TIMER_CONFIG_TYPE_SOFT);
	TimerStart(&timer3);
	for(;;){	

		task1Flag = 0;
		TaskDelay(1);
		task1Flag = 1;
		TaskDelay(1);
		
		if(stopped == 0){
			TaskDelay(200);
			TimerStop(&timer1);
			stopped = 1;
		}
		
	}
}

int task2Flag;
void task2Entry(void * param){		


	for(;;){

		task2Flag = 0;
		TaskDelay(1);
		task2Flag = 1;
		TaskDelay(1);

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




