#ifndef TARDIS_OS_H
#define TARDIS_OS_H

#include <stdint.h>
#include "DS_Lib.h"
#include "Config.h"
typedef uint32_t TaskStack;
typedef struct _Task{
	TaskStack * stack;					//��ջָ��
	uint32_t delayTicks;				//��ʱʱ��
	uint32_t priority;					//���ȼ�
}Task;


extern Task * currentTask;
extern Task * nextTask;


uint32_t TaskEnterCritical(void);		//�ٽ�������:�����ٽ���
void TaskExitCritical(uint32_t);		//�ٽ�������:�˳��ٽ���

void TaskRunFirst(void);
void TaskSwitch(void);



void TaskSchedInit(void);				//��������ʼ��
void TaskSchedDisable(void);			//����������
void TaskSchedEnable(void);				//����������
void TaskSched(void);					//��ʼ����


#endif
