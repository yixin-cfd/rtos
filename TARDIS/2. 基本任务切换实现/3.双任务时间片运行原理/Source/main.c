#include "TARDIS_OS.h"
#include "ARMCM4.h"
Task * currentTask;
Task * nextTask;
Task * taskTable[2];

void TaskSched(void){
	nextTask = (currentTask==taskTable[0])?taskTable[1]:taskTable[0];
	TaskSwitch();
}
//���ö�ʱ��
void SetSysTickPeriod(uint32_t ms){
	SysTick->LOAD = ms * SystemCoreClock / 1000 - 1;
	NVIC_SetPriority(SysTick_IRQn,(1<<__NVIC_PRIO_BITS)-1);
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk|
					SysTick_CTRL_TICKINT_Msk|
					SysTick_CTRL_ENABLE_Msk;
}
void SysTick_Handler(){
	TaskSched();
}
void delay(int count){
	while(--count);
}

Task Task1;				//������������
Task Task2;

TaskStack Task1Env[24];	//����������ջ
TaskStack Task2Env[24];

//���������Ӧ�Ĺ��ܺ���
int task1Flag;
void task1Entry(void * param){

	SetSysTickPeriod(10);
	for(;;){
		task1Flag = 0;
		delay(100);
		task1Flag = 1;
		delay(100);
	}
}
int task2Flag;
void task2Entry(void * param){

	for(;;){
		task2Flag = 0;
		delay(100);
		task2Flag = 1;
		delay(100);
	}
}
//���������ʼ����
void TaskInit(Task *task,void (*entry)(void *),void *param,TaskStack *stack){
	
	/*********�Զ�����****************/
	*(--stack) = (unsigned long)(1<<24);
	*(--stack) = (unsigned long)entry;
	*(--stack) = (unsigned long)0x14;
	*(--stack) = (unsigned long)0x12;
	*(--stack) = (unsigned long)0x3;
	*(--stack) = (unsigned long)0x2;
	*(--stack) = (unsigned long)0x1;
	*(--stack) = (unsigned long)param;       //R0
	/*********�Զ�����****************/
	*(--stack) = (unsigned long)0x11;
	*(--stack) = (unsigned long)0x10;
	*(--stack) = (unsigned long)0x9;
	*(--stack) = (unsigned long)0x8;
	*(--stack) = (unsigned long)0x7;
	*(--stack) = (unsigned long)0x6;
	*(--stack) = (unsigned long)0x5;
	*(--stack) = (unsigned long)0x4;

	task->stack = stack;
	
	//stack ָ��տ�ʼ���ڶ�ջ��׶�
}

int main(void){
	//�����ʼ��

	TaskInit(&Task1,task1Entry,(void *)0x00,&Task1Env[24]);
	TaskInit(&Task2,task2Entry,(void *)0x00,&Task2Env[24]);
	
	taskTable[0] = &Task1;
	taskTable[1] = &Task2;
	
	nextTask = taskTable[0];
	TaskRunFirst();
	
	return 0;
}


