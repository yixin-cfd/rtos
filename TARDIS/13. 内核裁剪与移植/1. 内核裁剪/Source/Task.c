#include "TARDIS_OS.h"
#include <string.h>
//���������ʼ����
void TaskInit(Task *task,void (*entry)(void *),void *param,uint32_t priority,TaskStack *stack, uint32_t size){
	
	uint32_t * stackTop;	//��ջĩ��ָ��
	task->stackBase = stack;//��ջ��ʵ��ַ
	task->stackSize = size;	//��ջ��С
	memset(stack, 0, size);	//��ջ����
	
	stackTop = stack + size / sizeof(TaskStack);
	/*********�Զ�����****************/
	*(--stackTop) = (unsigned long)(1<<24);
	*(--stackTop) = (unsigned long)entry;
	*(--stackTop) = (unsigned long)0x14;
	*(--stackTop) = (unsigned long)0x12;
	*(--stackTop) = (unsigned long)0x3;
	*(--stackTop) = (unsigned long)0x2;
	*(--stackTop) = (unsigned long)0x1;
	*(--stackTop) = (unsigned long)param;       //R0
	/*********�Զ�����****************/
	*(--stackTop) = (unsigned long)0x11;
	*(--stackTop) = (unsigned long)0x10;
	*(--stackTop) = (unsigned long)0x9;
	*(--stackTop) = (unsigned long)0x8;
	*(--stackTop) = (unsigned long)0x7;
	*(--stackTop) = (unsigned long)0x6;
	*(--stackTop) = (unsigned long)0x5;
	*(--stackTop) = (unsigned long)0x4;

	task->slice = TARDIS_OS_SLICE_MAX;		//ʱ��Ƭ��С
	task->stack = stackTop;					//stack ָ��տ�ʼ���ڶ�ջ��׶�
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
	uint32_t * stackEnd;	//��ջĩ�˵�ַ
	uint32_t status = TaskEnterCritical();

	info->delayTicks = task->delayTicks;
	info->priority = task->priority;
	info->slice = task->slice;
	info->state = task->state;
	info->suspendCount = task->suspendCount;
	info->stackSize = task->stackSize;
	
	info->stackFree = 0;
	stackEnd = task->stackBase;
	while( (*stackEnd++ == 0) && (stackEnd <= task->stackBase + task->stackSize / sizeof(TaskStack)))
	{
		info->stackFree++;
	}
	info->stackFree *= sizeof(TaskStack);
	TaskExitCritical(status);
}

