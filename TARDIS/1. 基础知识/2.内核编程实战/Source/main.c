#define NVIC_INT_CTRL    0xE000ED04
#define NVIC_PENDSVSET   0x10000000
#define NVIC_SYSPRI2     0xE000ED22
#define NVIC_PENDSV_PRI  0x000000FF

//写寄存器宏
#define MEM32(addr)   *(volatile unsigned long *)(addr)
#define MEM8(addr)    *(volatile unsigned char *)(addr)
	
void triggerPendSVC(void){
	//设置PendSVC异常的优先级为最低
	MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;
	//触发PendSVC异常
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}

typedef struct _BlockType_t{
	unsigned long * stackPtr;    //堆栈指针
	
}BlockType_t;

BlockType_t * blockPtr;
void delay(int count){
	while(--count);
}

int flag;
BlockType_t block;
unsigned long stackBuffer[50];
int main(void){
	block.stackPtr = &stackBuffer[50];
	blockPtr=&block;
	while(1){
		flag = 1;
		delay(100);
		flag = 0;
		delay(100);	
		triggerPendSVC();
	}
}


