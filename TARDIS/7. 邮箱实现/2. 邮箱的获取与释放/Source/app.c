#include "TARDIS_OS.h"

Task Task1;				
Task Task2;
Task Task3;
Task Task4;

TaskStack Task1Env[512];	
TaskStack Task2Env[512];
TaskStack Task3Env[512];
TaskStack Task4Env[512];

Mbox mbox1;
Mbox mbox2;
void * mbox1MsgBuffer[20];
void * mbox2MsgBuffer[20];
uint32_t msg[20];

int task1Flag;
void task1Entry(void * param){
		

	SetSysTickPeriod(10);
	
	MboxInit(&mbox1, mbox1MsgBuffer, 20);
	for(;;){	
		uint32_t i = 0;
		for( i = 0; i< 20; i++){
			msg[i] = i;
			MboxNotify(&mbox1, &msg[i], MBOXSendNormal);
		}
		TaskDelay(100);
		for( i = 0; i< 20; i++){
			msg[i] = i;
			MboxNotify(&mbox1, &msg[i], MBOXSendFront);
		}
		TaskDelay(100);
		task1Flag = 0;
		TaskDelay(1);
		task1Flag = 1;
		TaskDelay(1);
	}
}

int task2Flag;
void task2Entry(void * param){		


	
	for(;;){
		
		void * msg;
		
		uint32_t error = MboxWait(&mbox1, &msg, 0);
		
		if( error == NoneError){
			uint32_t value = *( uint32_t *)msg;
			task2Flag = value;
			TaskDelay(1);
		}
		
		task2Flag = 0;
		TaskDelay(1);
		task2Flag = 1;
		TaskDelay(1);
	}
}

int task3Flag;
void task3Entry(void * param){		

	MboxInit(&mbox2, mbox2MsgBuffer, 20);
	for(;;){
		void * msg;
		MboxWait(&mbox2, &msg, 100);
		
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
	TaskInit(&Task1,task1Entry,(void *)0x01,0,&Task1Env[512]);
	TaskInit(&Task2,task2Entry,(void *)0x02,1,&Task2Env[512]);
	TaskInit(&Task3,task3Entry,(void *)0x03,1,&Task3Env[512]);
	TaskInit(&Task4,task4Entry,(void *)0x04,1,&Task4Env[512]);
}




