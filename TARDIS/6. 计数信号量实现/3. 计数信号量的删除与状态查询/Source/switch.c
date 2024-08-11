#include "TARDIS_OS.h"
#include "ARMCM4.h"

#define NVIC_INT_CTRL    0xE000ED04
#define NVIC_PENDSVSET   0x10000000
#define NVIC_SYSPRI2     0xE000ED22
#define NVIC_PENDSV_PRI  0x000000FF

//д�Ĵ�����
#define MEM32(addr)   *(volatile unsigned long *)(addr)
#define MEM8(addr)    *(volatile unsigned char *)(addr)
	

//�ٽ�������:�����ٽ���
uint32_t TaskEnterCritical(void){
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	return primask;
}
//�ٽ�������:�˳��ٽ���
void TaskExitCritical(uint32_t status){
	__set_PRIMASK(status);
}

__asm void PendSV_Handler(void){
	IMPORT currentTask
	IMPORT nextTask
	
	MRS	R0, PSP					//��PSP��ֵ���浽R0��
	CBZ R0, PendSVHandler_nosave  
	STMDB R0!,{R4-R11}			//�ָ�R4-R11
	
	LDR R1, =currentTask
	LDR R1, [R1]
	STR R0, [R1]				//����current��ǰ����״ָ̬��
	
PendSVHandler_nosave			//��ʼʱ������״ֱ̬�ӻָ��������б���״̬��ظ���һ������
	LDR R0, =currentTask		//���ص�ǰ����ָ������ĵ�ַ
	LDR R1, =nextTask			//�����¸�����ָ������ĵ�ַ
	LDR R2, [R1]				//ȡ���¸������ָ������
	STR R2, [R0]				//���� currentRask ��ָ��Ϊ nextTask
	
	LDR R0, [R2]				//��nextTask�Ķ�ջָ����ص�R0��
	LDMIA R0!, {R4-R11}			//�Ӷ�ջָ����׶˿�ʼ�����λָ�R4-R11,����R0ָ���ջָ�������λ��
	
	MSR PSP, R0					//���û�����ջ�л���currentTask�Ķ�ջ
	ORR LR, LR, #0x04			//�˳��쳣�����ʹ���û����Ķ�ջ
	BX LR						//�˳��쳣�󣬼�����currentTask�������ָ�cpu��״̬��Ϣ
}
	
	
void TaskRunFirst(void){
	__set_PSP(0);
	
	//����PendSVC�쳣�����ȼ�Ϊ���
	MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;
	//����PendSVC�쳣
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}

void TaskSwitch(void){
	//����PendSVC�쳣
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}
