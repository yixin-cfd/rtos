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
	task->suspendCount = 0;					//���������
	
	task->clean = ( void (*) (void *))0;	//����ɾ������
	task->cleanParam = (void *)0;				//ɾ������
	task->requestDeleteFlag = 0;			//����ɾ�����
	
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

void TaskSetCleanCallFunc(Task * task,void (*clean)(void * param),void * param){	//��������ɾ������
	task->clean = clean;
	task->cleanParam = param;
}

void TaskForceDelete(Task * task){			//������ǿ��ɾ��
	uint32_t status = TaskEnterCritical();
	if( task->state & TARDIS_OS_TASK_STATE_DELAYED){		//�����������ʱ״̬
		TimeTaskRemove(task);
	}
	else if( !(task->state & TARDIS_OS_TASK_STATE_SUSPEND)){//������ڹ���״̬����ô���ھ���״̬
		TaskSchedRemove(task);
	}
	if( task->clean ){					//���������ɾ������
		task->clean(task->cleanParam);	//��������ɾ��
	}
	if( currentTask == task){			//�����ɾ���������ǵ�ǰ����
		TaskSched();					//�����������
	}
	TaskExitCritical(status);
}

void TaskRequestDelete(Task * task){			//����ɾ������
	uint32_t status = TaskEnterCritical();
	task->requestDeleteFlag = 1;				//���������־λ
	TaskExitCritical(status);
}

uint8_t TaskIsRequestDeleted (void){			//��鵱ǰ�����Ƿ���Ҫɾ��
	uint8_t deleted = 0;
	uint32_t status = TaskEnterCritical();
	deleted = currentTask->requestDeleteFlag;	
	TaskExitCritical(status);
	return deleted;
}

void TaskDeleteSelf(void){		//��ǰ����ɾ���Լ�
	uint32_t status = TaskEnterCritical();
	TaskSchedRemove(currentTask);						//����ǰ����Ӿ����������Ƴ�
	if( currentTask->clean ){							//���������ɾ������
		currentTask->clean(currentTask->cleanParam);	//��������ɾ��
	}
	TaskExitCritical(status);
	TaskSched();
}

void GetTaskInfo(Task * task, TaskInfo * info){		//����״̬��ѯ����
	uint32_t status = TaskEnterCritical();

	info->delayTicks = task->delayTicks;
	info->priority = task->priority;
	info->slice = task->slice;
	info->state = task->state;
	info->suspendCount = task->suspendCount;
	
	TaskExitCritical(status);
}

