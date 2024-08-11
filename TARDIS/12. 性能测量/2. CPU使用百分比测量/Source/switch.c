#include "TARDIS_OS.h"
#include "ARMCM4.h"

#define NVIC_INT_CTRL    0xE000ED04
#define NVIC_PENDSVSET   0x10000000
#define NVIC_SYSPRI2     0xE000ED22
#define NVIC_PENDSV_PRI  0x000000FF

//写寄存器宏
#define MEM32(addr)   *(volatile unsigned long *)(addr)
#define MEM8(addr)    *(volatile unsigned char *)(addr)
	

//临界区保护:进入临界区
uint32_t TaskEnterCritical(void){
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	return primask;
}
//临界区保护:退出临界区
void TaskExitCritical(uint32_t status){
	__set_PRIMASK(status);
}

__asm void PendSV_Handler(void){
	IMPORT currentTask
	IMPORT nextTask
	
	MRS	R0, PSP					//将PSP的值保存到R0里
	CBZ R0, PendSVHandler_nosave  
	STMDB R0!,{R4-R11}			//恢复R4-R11
	
	LDR R1, =currentTask
	LDR R1, [R1]
	STR R0, [R1]				//更新current当前任务状态指针
	
PendSVHandler_nosave			//开始时不保存状态直接恢复或运行中保存状态后回复下一个任务
	LDR R0, =currentTask		//加载当前任务指针变量的地址
	LDR R1, =nextTask			//加载下个任务指针变量的地址
	LDR R2, [R1]				//取出下个任务的指针内容
	STR R2, [R0]				//更改 currentRask 的指针为 nextTask
	
	LDR R0, [R2]				//将nextTask的堆栈指针加载到R0中
	LDMIA R0!, {R4-R11}			//从堆栈指针最底端开始，依次恢复R4-R11,并且R0指向堆栈指针的最终位置
	
	MSR PSP, R0					//把用户级堆栈切换到currentTask的堆栈
	ORR LR, LR, #0x04			//退出异常后继续使用用户级的堆栈
	BX LR						//退出异常后，继续从currentTask弹出并恢复cpu的状态信息
}
	
	
void TaskRunFirst(void){
	__set_PSP(0);
	
	//设置PendSVC异常的优先级为最低
	MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;
	//触发PendSVC异常
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}

void TaskSwitch(void){
	//触发PendSVC异常
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}
