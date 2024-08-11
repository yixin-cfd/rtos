__asm void PendSV_Handler(void){
	IMPORT blockPtr
	
	LDR R0, = blockPtr     //把blockPtr指针的地址加载到R0中
	LDR R0, [R0]           //blockPtr存放的地址加载到R0中
						   //这个地址也就是结构体中stackPtr的地址
	LDR R0, [R0]           //stackPtr存放的内容即栈的后一个单元的地址放到R0中
	
	STMDB   R0!, {R4-R11}
	//D表示递减，B表示Befor即先递减，！表示最后一个写进单元的地址
	
	//R0现在存放栈中栈顶地址
	//更新堆栈指针
	LDR R1, = blockPtr
	LDR R1, [R1]
	STR R0, [R1]
	
	//测试
	ADD R4, R4, #1
	ADD R5, R5, #1
	
	//恢复R4~R11
	LDMIA    R0!, {R4-R11}
	//退出异常
	BX LR
	
}



