#ifndef _TASK_H
#define _TASK_H

#include <stdint.h>
#include "DS_Lib.h"

#define TARDIS_OS_TASK_STATE_RDY 				0				//�������״̬
#define TARDIS_OS_TASK_STATE_DESTORYED 			(1 << 1)		//����ɾ��״̬
#define TARDIS_OS_TASK_STATE_DELAYED 			(1 << 2)		//������ʱ����״̬
#define TARDIS_OS_TASK_STATE_SUSPEND			(1 << 3)		//�������״̬

//��16λ�����ȴ��¼��ı�־
#define TARDIS_OS_TASK_WAIT_MASK				(0xFF << 16)	//�ȴ��¼�������

struct _Event;

typedef uint32_t TaskStack;
typedef struct _Task{
	TaskStack * stack;					//��ջָ��
	Node  linkNode;						//���ӽڵ�;���ӵ����ֶ����У��磺���ȼ����С��ȴ��¼�����
	uint32_t delayTicks;				//��ʱʱ��
	Node   delayNode;					//��ʱ���ӽڵ�
	uint32_t priority;					//���ȼ�
	uint32_t state;						//����״̬
	uint32_t slice;						//ʱ��Ƭ��С
	uint32_t suspendCount;				//���������
	
	void (*clean)(void * param);		//����ɾ������
	void * cleanParam;					//ɾ���Ĳ���
	uint8_t requestDeleteFlag;			//����ɾ�����
	
	struct _Event * waitEvent;			//����ȴ��ĸ��¼����ƿ�
	void * eventMsg;					//�¼���ص����ݴ��λ��
	uint32_t waitEventResult;			//�ȴ��¼��Ľ��
	
	uint32_t waitFlagsType;				//�ȴ��¼�����
	uint32_t eventFlags;				//�ȴ��¼����
}Task;

typedef struct _TaskInfo{
	uint32_t delayTicks;				//��ʱʱ��
	uint32_t priority;					//���ȼ�
	uint32_t state;						//����״̬
	uint32_t slice;						//ʱ��Ƭ��С
	uint32_t suspendCount;				//���������
}TaskInfo;								//����״̬�ṹ��
void GetTaskInfo(Task * task, TaskInfo * info);		//����״̬��ѯ����
void TaskInit(Task *task,void (*entry)(void *),void *param,uint32_t priority,TaskStack *stack); //�����ʼ������

void TaskSuspend(Task * task);  		//���������
void TaskWakeUp(Task * task);			//�����Ѻ���

void TaskSetCleanCallFunc(Task * task,void (*clean)(void * param),void * param); //��������ɾ������
void TaskForceDelete(Task * task);		//������ǿ��ɾ��
void TaskRequestDelete(Task * task);	//����ɾ������
uint8_t TaskIsRequestDeleted (void);	//��鵱ǰ�����Ƿ���Ҫɾ��
void TaskDeleteSelf(void);          	//��ǰ����ɾ���Լ�

#endif

