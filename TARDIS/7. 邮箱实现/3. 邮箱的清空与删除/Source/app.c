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
void * mbox1Buffer[20];
uint32_t msg1[20];

Mbox mbox2;
void * mbox2Buffer[20];
uint32_t msg2[20];

int task1Flag;
void task1Entry(void * param){
		
	void * msg;
	
	SetSysTickPeriod(10);
	
	MboxInit(&mbox1, mbox1Buffer, 20);
	MboxInit(&mbox2, mbox2Buffer, 20);
	
	MboxWait(&mbox1, (void *)msg, 0);
	for(;;){	
		task1Flag = 0;
		TaskDelay(1);
		task1Flag = 1;
		TaskDelay(1);
	}
}

int task2Flag;
void task2Entry(void * param){		

	int destoryed = 0;
	
	for(;;){
		
		task2Flag = 0;
		TaskDelay(1);
		task2Flag = 1;
		TaskDelay(1);
		
		if(!destoryed){
			MboxDestory(&mbox1);
			destoryed = 1;
		}
	}
}

int task3Flag;
void task3Entry(void * param){		

	for(;;){	
		int i = 0;
		for (i = 0;i<20;i++){
			msg2[i]=i;
			MboxNotify(&mbox2, &msg2[i], MBOXSendNormal);
		}
		task3Flag = 0;
		TaskDelay(1);
		task3Flag = 1;
		TaskDelay(1);
	}
}
int task4Flag;
void task4Entry(void * param){		//任务4请求删除任务3

	for(;;){
		uint32_t * msg;
		
		MboxWait(&mbox2, (void *)&msg, 0);
		task4Flag = *msg;
		
		MboxClear(&mbox2);
		
	}
}


void InitApp(void){
	TaskInit(&Task1,task1Entry,(void *)0x01,0,&Task1Env[512]);
	TaskInit(&Task2,task2Entry,(void *)0x02,1,&Task2Env[512]);
	TaskInit(&Task3,task3Entry,(void *)0x03,1,&Task3Env[512]);
	TaskInit(&Task4,task4Entry,(void *)0x04,1,&Task4Env[512]);
}




