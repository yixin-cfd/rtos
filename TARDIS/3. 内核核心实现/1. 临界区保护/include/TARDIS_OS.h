#ifndef TARDIS_OS_H
#define TARDIS_OS_H
#include<stdint.h>

typedef uint32_t TaskStack;
typedef struct _Task{
	TaskStack * stack;			//��ջָ��
	uint32_t delayTicks;		//��ʱʱ��
}Task;


extern Task * currentTask;
extern Task * nextTask;


uint32_t TaskEnterCritical(void);		//�ٽ�������:�����ٽ���
void TaskExitCritical(uint32_t);		//�ٽ�������:�˳��ٽ���

void TaskRunFirst(void);
void TaskSwitch(void);
#endif
