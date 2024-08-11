#include "TARDIS_OS.h"

#define NVIC_INT_CTRL    0xE000ED04
#define NVIC_PENDSVSET   0x10000000
#define NVIC_SYSPRI2     0xE000ED22
#define NVIC_PENDSV_PRI  0x000000FF

//д�Ĵ�����
#define MEM32(addr)   *(volatile unsigned long *)(addr)
#define MEM8(addr)    *(volatile unsigned char *)(addr)
	
void triggerPendSVC(void){
	//����PendSVC�쳣�����ȼ�Ϊ���
	MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;
	//����PendSVC�쳣
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}

typedef struct _BlockType_t{
	unsigned long * stackPtr;    //��ջָ��
	
}BlockType_t;

BlockType_t * blockPtr;
void delay(int count){
	while(--count);
}

int flag;
BlockType_t block;
unsigned long stackBuffer[50];


Task Task1;				//������������
Task Task2;

TaskStack Task1Env[32];	//����������ջ
TaskStack Task2Env[32];

//���������Ӧ�Ĺ��ܺ���

void task1(void * param){

	for(;;){
		
		
	}
}

void task2(void * param){

	for(;;){
		
		
	}
}
//���������ʼ����
void TaskInit(Task *task,void (*entry)(void *),void *param,TaskStack *stack){
	task->stack = stack;
}

int main(void){
	//�����ʼ��
	block.stackPtr = &stackBuffer[50];
	blockPtr=&block;
	TaskInit(&Task1,task1,(void *)0x00,&Task1Env[32]);
	TaskInit(&Task2,task2,(void *)0x00,&Task2Env[32]);
	
	
	while(1){
		flag = 1;
		delay(100);
		flag = 0;
		delay(100);	
		triggerPendSVC();
	}
}


