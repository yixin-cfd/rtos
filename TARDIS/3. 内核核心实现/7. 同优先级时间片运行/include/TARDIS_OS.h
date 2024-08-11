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
	Node  linkNode;						//���ȼ��������ӽڵ�
	uint32_t delayTicks;				//��ʱʱ��
	Node   delayNode;					//��ʱ���ӽڵ�
	uint32_t priority;					//���ȼ�
	uint32_t state;						//����״̬
	uint32_t slice;						//ʱ��Ƭʱ��
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

void TaskSchedRdy(Task * task);			//������б��в��룬���޸�λͼ
void TaskSchedUnRdy(Task * task);		//�Ӿ����б���ɾ����������Ҫ�����Ƿ��λͼ��ɾ��

void TimeTaskWait(Task * task,uint32_t ticks);	//������ʱ����
void TimeTaskWakeUp(Task *task);				//����ʱ������ɾ��


void SetSysTickPeriod(uint32_t ms);		//���ö�ʱ��
void TaskSystemTickHandler(void);		//��ʱ���жϴ�����
void TaskDelay(uint32_t delay);			//��ʱ����
//�����ʼ������
void TaskInit(Task *task,void (*entry)(void *),void *param,uint32_t priority,TaskStack *stack);
#endif
