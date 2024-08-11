#include "TARDIS_OS.h"
#include "ARMCM4.h"
//���ö�ʱ��
void SetSysTickPeriod(uint32_t ms){
	SysTick->LOAD = ms * SystemCoreClock / 1000 - 1;
	NVIC_SetPriority(SysTick_IRQn,(1<<__NVIC_PRIO_BITS)-1);
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk|
					SysTick_CTRL_TICKINT_Msk|
					SysTick_CTRL_ENABLE_Msk;
}
//ϵͳ��ʱ�����жϴ�����
void SysTick_Handler(){
	//ʱ���ж���������
	TaskSystemTickHandler();		//���ն���Ҫ��������ʱ���к�����ʱ��Ƭ���в������������������
}

