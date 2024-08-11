#ifndef TARDIS_OS_H
#define TARDIS_OS_H

#include <stdint.h>
#include "Config.h"
#include "DS_Lib.h"
#include "Task.h"
#include "Event.h"
#include "Semaphore.h"

typedef enum _Error{			//�¼����������
	NoneError = 0,
	ErrorTimeout = 1,
}Error;


extern Task * currentTask;
extern Task * nextTask;


uint32_t TaskEnterCritical(void);		//�ٽ�������:�����ٽ���
void TaskExitCritical(uint32_t);		//�ٽ�������:�˳��ٽ���

void TaskRunFirst(void);				//���е�һ������
void TaskSwitch(void);					//�л�����



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


void TaskSchedRemove(Task * task);		//������Ӿ����������Ƴ�
void TimeTaskRemove(Task *task);		//���������ʱ�������Ƴ�

void InitApp(void);						//Ӧ�ó�ʼ�����������������ĳ�ʼ������

#endif
