#ifndef _HOOKS_H
#define _HOOKS_H

#include "TARDIS_OS.h"

void HooksCpuIdle(void);	//��������Ĺ��Ӻ���
void HooksSysTick(void);	//SysTick�жϷ���ʱ�Ĺ��Ӻ���
void HooksTaskSwitch(Task * from, Task * to);//�����л��Ĺ��Ӻ���
void HooksTaskInit(Task * task);//�����ʼ���Ĺ��Ӻ���		
#endif


