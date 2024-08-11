#include "TARDIS_OS.h"

//���������ʼ����
void TaskInit(Task *task,void (*entry)(void *),void *param,uint32_t priority,TaskStack *stack){
	
	/*********�Զ�����****************/
	*(--stack) = (unsigned long)(1<<24);
	*(--stack) = (unsigned long)entry;
	*(--stack) = (unsigned long)0x14;
	*(--stack) = (unsigned long)0x12;
	*(--stack) = (unsigned long)0x3;
	*(--stack) = (unsigned long)0x2;
	*(--stack) = (unsigned long)0x1;
	*(--stack) = (unsigned long)param;       //R0
	/*********�Զ�����****************/
	*(--stack) = (unsigned long)0x11;
	*(--stack) = (unsigned long)0x10;
	*(--stack) = (unsigned long)0x9;
	*(--stack) = (unsigned long)0x8;
	*(--stack) = (unsigned long)0x7;
	*(--stack) = (unsigned long)0x6;
	*(--stack) = (unsigned long)0x5;
	*(--stack) = (unsigned long)0x4;

	task->slice = TARDIS_OS_SLICE_MAX;		//ʱ��Ƭ��С
	task->stack = stack;					//stack ָ��տ�ʼ���ڶ�ջ��׶�
	task->delayTicks = 0;					//��ʱʱ��
	task->priority = priority;				//���ȼ�
	task->state = TARDIS_OS_TASK_STATE_RDY;	//����״̬
	
	NodeInit(&(task->delayNode));			//��ʼ������ṹ�����ʱ�ڵ�
	NodeInit(&(task->linkNode));			//��ʼ������ṹ������ӵ����ȼ����еĽڵ�
	
	TaskSchedRdy(task);	//����������ӽڵ���ӵ���Ӧ�����ȼ����е�ͷ��;���ö�Ӧ��λͼ��˵��������ȼ������������
}

//���������
void TaskSuspend(Task * task){
	uint32_t status = TaskEnterCritical();
	if( !(task->state & TARDIS_OS_TASK_STATE_DELAYED) ){	//�������ʱ״̬���ܹ���	
		if(++task->suspendCount <= 1){						//��һ�ι���
			task->state |= TARDIS_OS_TASK_STATE_SUSPEND;	//��������״̬
			TaskSchedUnRdy(task);							//�Ӿ����������Ƴ� & �޸�λͼ
			if(task == currentTask){						//�����������ǵ�ǰ����Ļ������������
				TaskSched();								//�������
			}
		}
	}
	TaskExitCritical(status);
}

//�����Ѻ���
void TaskWakeUp(Task * task){
	uint32_t status = TaskEnterCritical();
	if( task->state & TARDIS_OS_TASK_STATE_SUSPEND ){	//�ж������ǲ��ǹ���״̬
		if( --task->suspendCount == 0){					//������������Ϊ��
			task->state &=  ~TARDIS_OS_TASK_STATE_SUSPEND;		//�������״̬
			TaskSchedRdy(task);							//�����������
			TaskSched();								//�������
		}
	}
	TaskExitCritical(status);
}

