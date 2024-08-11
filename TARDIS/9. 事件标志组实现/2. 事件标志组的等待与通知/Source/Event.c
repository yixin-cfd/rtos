#include "TARDIS_OS.h"


void EventInit(Event * event , EventType type) {	//�¼���ʼ������
	
	event->type = type;
	ListInit(&event->waitList);
	
}

void EventWait(Event * event, Task * task, void * msg, uint32_t state, uint32_t timeout)	//�ȴ��¼�
{
	uint32_t status = TaskEnterCritical();
	
	task->state |= state;	//������չ
	task->waitEvent = event;//����ȴ����ƿ�
	task->eventMsg = msg;	//�¼�����Ϣ���λ��
	task->waitEventResult = NoneError;//����¼����
	
	TaskSchedUnRdy(task);	//�Ӿ����������Ƴ�
	ListAddTail(&event->waitList,&task->linkNode);	//���뵽�ȴ����е�β��
	
	if(timeout){			//������ó�ʱ
		TimeTaskWait(task,timeout);	//���뵽��ʱ������
	}
	
	TaskExitCritical(status);

}

Task * EventWakeUp(Event * event, void * msg, uint32_t result){		//���¼����ƿ��л�������

	Node * node;
	Task *task = (Task *)0;
	uint32_t status = TaskEnterCritical();
	if( (node = ListRemoveFirst(&event->waitList)) != (Node *)0){	//������¼��ĵȴ�������������
		task = NodeParent(node, Task, linkNode);					//��ȡ���ӽڵ��Ӧ������
		task->waitEvent = (Event *)0;								//��ʼ��������¼����ƿ�
		task->eventMsg = msg;										//�����������Ϣλ��	
		task->waitEventResult = result;								//����������¼����
		task->state &= ~TARDIS_OS_TASK_WAIT_MASK;					//����ȴ��¼����־
		
		if(task->delayTicks){		//���������ʱ������
			TimeTaskWakeUp(task);	//����ʱ�����л���
		}
		TaskSchedRdy(task);			//���뵽����������
	}
	
	TaskExitCritical(status);
	return task;
	
}
	
void EventWakeUpTask(Event * event, Task * task, void * msg, uint32_t result){//���¼����ƿ��л����ض�����


	uint32_t status = TaskEnterCritical();
	
	ListRemoveNode(&event->waitList, &task->linkNode);
	
	task->waitEvent = (Event *)0;								//��ʼ��������¼����ƿ�
	task->eventMsg = msg;										//�����������Ϣλ��	
	task->waitEventResult = result;								//����������¼����
	task->state &= ~TARDIS_OS_TASK_WAIT_MASK;					//����ȴ��¼����־
		
	if(task->delayTicks){		//���������ʱ������
		TimeTaskWakeUp(task);	//����ʱ�����л���
	}
	TaskSchedRdy(task);			//���뵽����������

	
	TaskExitCritical(status);

	
}

void EventRemoveTask(Task * task,void * msg, uint32_t result){		//��������¼����ƿ���ɾ������ʱ���жϴ���������  

	uint32_t status = TaskEnterCritical();
	
	ListRemoveNode(&task->waitEvent->waitList,&task->linkNode);	//��������¼����ƶ�����ɾ��
	
	task->waitEvent = (Event *)0;									//��ʼ��������¼����ƿ�
	task->eventMsg = msg;											//�����������Ϣλ��	
	task->waitEventResult = result;									//����������¼����
	task->state &= ~TARDIS_OS_TASK_WAIT_MASK;						//����ȴ��¼����־
	
	TaskExitCritical(status);
}

uint32_t EventRemoveAll( Event * event, void * msg, uint32_t result){	//����¼����ƿ�
	Node * node;
	uint32_t count = 0;
	uint32_t status = TaskEnterCritical();
	
	count = GetListCount(&event->waitList);
	
	while( (node = ListRemoveFirst(&event->waitList)) != (Node *)0){
		Task * task = NodeParent(node, Task, linkNode);				//��ȡ���ӽڵ��Ӧ������
		task->waitEvent = (Event *)0;								//��ʼ��������¼����ƿ�
		task->eventMsg = msg;										//�����������Ϣλ��	
		task->waitEventResult = result;								//����������¼����
		task->state &= ~TARDIS_OS_TASK_WAIT_MASK;					//����ȴ��¼����־
		
		if(task->delayTicks){		//���������ʱ������
			TimeTaskWakeUp(task);	//����ʱ�����л���
		}
		TaskSchedRdy(task);			//���뵽����������
	}
	
	TaskExitCritical(status);
	return count;
}

uint32_t EventWaitCount(Event * event){		//��ȡ�ȴ��¼����������
	uint32_t count = 0;
	
	uint32_t status = TaskEnterCritical();	 
	count = GetListCount(&event->waitList);
	TaskExitCritical(status);
	
	return count;
}


