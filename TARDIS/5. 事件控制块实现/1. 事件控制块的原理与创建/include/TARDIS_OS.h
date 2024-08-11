#ifndef TARDIS_OS_H
#define TARDIS_OS_H

#include <stdint.h>
#include "Config.h"
#include "DS_Lib.h"
#include "Event.h"

typedef enum _Error{			//�¼����������
	NoneError = 0,
}Error;

#define TARDIS_OS_TASK_STATE_RDY 				0				//�������״̬
#define TARDIS_OS_TASK_STATE_DESTORYED 			( 1 << 1 )		//����ɾ��״̬
#define TARDIS_OS_TASK_STATE_DELAYED 			( 1 << 2 )		//������ʱ����״̬
#define TARDIS_OS_TASK_STATE_SUSPEND			( 1 << 3 )		//�������״̬

typedef uint32_t TaskStack;
typedef struct _Task{
	TaskStack * stack;					//��ջָ��
	Node  linkNode;						//���ȼ��������ӽڵ�
	uint32_t delayTicks;				//��ʱʱ��
	Node   delayNode;					//��ʱ���ӽڵ�
	uint32_t priority;					//���ȼ�
	uint32_t state;						//����״̬
	uint32_t slice;						//ʱ��Ƭ��С
	uint32_t suspendCount;				//���������
	
	void (*clean)(void * param);		//����ɾ������
	void * cleanParam;					//ɾ���Ĳ���
	uint8_t requestDeleteFlag;			//����ɾ�����
	
	Event * waitEvent;					//����ȴ��ĸ��¼����ƿ�
	void * eventMsg;					//�¼���ص����ݴ��λ��
	uint32_t waitEventResult;			//�ȴ��¼��Ľ��
}Task;


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

void TaskSuspend(Task * task);  		//���������
void TaskWakeUp(Task * task);			//�����Ѻ���

void TaskSchedRemove(Task * task);		//������Ӿ����������Ƴ�
void TimeTaskRemove(Task *task);		//���������ʱ�������Ƴ�


void TaskSetCleanCallFunc(Task * task,void (*clean)(void * param),void * param); //��������ɾ������
void TaskForceDelete(Task * task);		//������ǿ��ɾ��
void TaskRequestDelete(Task * task);	//����ɾ������
uint8_t TaskIsRequestDeleted (void);	//��鵱ǰ�����Ƿ���Ҫɾ��
void TaskDeleteSelf(void);          	//��ǰ����ɾ���Լ�

typedef struct _TaskInfo{
	uint32_t delayTicks;				//��ʱʱ��
	uint32_t priority;					//���ȼ�
	uint32_t state;						//����״̬
	uint32_t slice;						//ʱ��Ƭ��С
	uint32_t suspendCount;				//���������
}TaskInfo;								//����״̬�ṹ��
void GetTaskInfo(Task * task, TaskInfo * info);		//����״̬��ѯ����

void TaskInit(Task *task,void (*entry)(void *),void *param,uint32_t priority,TaskStack *stack); //�����ʼ������
void InitApp(void);						//Ӧ�ó�ʼ�����������������ĳ�ʼ������

#endif
