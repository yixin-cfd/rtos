#ifndef TARDIS_OS_H
#define TARDIS_OS_H

#include <stdint.h>
#include "DS_Lib.h"
#include "Config.h"

#define TARDIS_OS_TASK_STATE_RDY 				0
#define TARDIS_OS_TASK_STATE_DELAYED 			(1<<1)

typedef uint32_t TaskStack;
typedef struct _Task{
	TaskStack * stack;					//��ջָ��
	uint32_t delayTicks;				//��ʱʱ��
	Node   delayNode;					//��ʱ�ڵ�
	uint32_t priority;					//���ȼ�
	uint32_t state;						//����״̬
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
