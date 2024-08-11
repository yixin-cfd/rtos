#include "TARDIS_OS.h"


#if TARDIS_OS_ENABLE_TIMER == 1
static List TimerHardList;		//Ӳ����ʱ�����У��ж��д���
static List TimerSoftList;		//��ʱ��������Ķ���

static Semaphore TimerProtectSem;	//��ʱ������������������TimerSoftList���ź���
static Semaphore TimerTickSem;		//���жϴ��������֪ͨ��ʱ���������жϷ���

void TimerInit(Timer * timer, uint32_t startDelayTicks, uint32_t durationTicks, void (*timerFunc)(void * arg), void * arg, uint32_t config)
{
	NodeInit(&timer->linkNode);
	timer->startDelayTicks = startDelayTicks;
	timer->durationTicks = durationTicks;
	timer->timerFunc = timerFunc;
	timer->arg = arg;
	timer->config = config;
	
	if(startDelayTicks == 0){	//�����ʼ����ֵΪ��
		timer->delayTicks = durationTicks;//��ô����ֵΪ���ڼ���ֵ
	}
	else{
		timer->delayTicks = startDelayTicks;//����Ϊ��ʼ����ֵ
	}
	
	timer->state = TimerCreated;
}

static Task TimerTask;
static TaskStack TimerTaskStack[TARDIS_OS_TIMERTASK_STACK_SIZE];

static void TimerCallFuncList(List * timerList){	//�����б����ö�ʱ��������
	Node * node, *nextNode;
	for(node = timerList->headNode.nextNode; node != &(timerList->headNode); node = nextNode){
		Timer * timer = NodeParent(node, Timer, linkNode);
		nextNode = node->nextNode;
		if( (timer->delayTicks == 0) || (--timer->delayTicks == 0)){
			timer->state = TimerRunning;
			timer->timerFunc(timer->arg);
			timer->state = TimerStarted;
			
			if(timer->durationTicks > 0){
				timer->delayTicks = timer->durationTicks;
			}
			else{
				ListRemoveNode(timerList, &timer->linkNode);
				timer->state =TimerStopped;
			}
		}
	}
}
static void TimerSoftTask(void * param)	//��ʱ������
{
	for(;;)
	{
		SemWait(&TimerTickSem, 0);		//�ȴ�ϵͳʱ���жϷ���
		SemWait(&TimerProtectSem, 0);	//��ʱ��ӵ�ж�TimerSoftList�ķ���Ȩ
		TimerCallFuncList(&TimerSoftList);//ִ�ж�ʱ������
		SemNotify(&TimerProtectSem);	//��������������ʶ�TimerSoftList��
	}
}

void TimerModuleInit(void)	//��ʱ����ʼ��
{
	ListInit(&TimerHardList);
	ListInit(&TimerSoftList);
	
	SemaphoreInit(&TimerProtectSem, 1, 1);
	SemaphoreInit(&TimerTickSem, 0, 0);
}
void TimerInitTask(void)	//��ʱ����ʼ��
{
#if TARDIS_OS_TIMERTASK_PRIO >= (TARDIS_OS_PRIORITY_COUNT - 1)	//��ʱ����������ȼ�Ҫ�ȿ�����������ȼ�Ҫ��
	#error " The priority of timer task must be greater than (TARDIS_OS_PRIORITY_COUNT - 1)"
#endif
	
	TaskInit(&TimerTask, TimerSoftTask, (void *)0, TARDIS_OS_TIMERTASK_PRIO, TimerTaskStack, sizeof(TimerTaskStack));
}

void TimerStart(Timer * timer)	//��ʱ������
{
	switch(timer->state)
	{
		//��ʱ��ֻ�д��ڴ�����ֹͣ״̬��������
		case TimerCreated:					
		case TimerStopped:
			timer->delayTicks = timer->startDelayTicks ? timer->startDelayTicks : timer->durationTicks;//��ʱ��װ�س�ʼֵ
			timer->state = TimerStarted;	//��ʱ��״̬Ϊ�Ѿ�����
		
			if(timer->config & TIMER_CONFIG_TYPE_HARD) //������ʱ�����뵽�Ǹ�������
			{	//���뵽Ӳ����ʱ��������
				uint32_t status = TaskEnterCritical();
				
				ListAddHead(&TimerHardList, &timer->linkNode);	//�ж���Ҳ��������б��ˣ��ù��жϷ�ʽ
				
				TaskExitCritical(status);
			}
			else{
				//���뵽��ʱ�����������
				SemWait(&TimerProtectSem, 0);
				
				ListAddHead(&TimerSoftList, &timer->linkNode);	//�ڲ�ͬ�����з�������б����ź����ķ�ʽ
				
				SemNotify(&TimerProtectSem);
			}
			break;
		default:
			break;
	}
}


void TimerStop(Timer * timer){	//��ʱ��ֹͣ
	switch(timer->state)
	{
		case TimerStarted:
		case TimerRunning:
			if(timer->config & TIMER_CONFIG_TYPE_HARD) //��ʱ�����ĸ�������
			{	//��Ӳ����ʱ��������
				uint32_t status = TaskEnterCritical();
				
				ListRemoveNode(&TimerHardList, &timer->linkNode);	//�Ƴ���ʱ��
				
				TaskExitCritical(status);
			}
			else{
				//�ڶ�ʱ�����������
				SemWait(&TimerProtectSem, 0);
				
				ListRemoveNode(&TimerSoftList, &timer->linkNode);	//�Ƴ���ʱ��
				
				SemNotify(&TimerProtectSem);
			}
			break;
		default:
			break;
	}
}

void TimerDestory(Timer * timer)	//ɾ����ʱ��
{
	TimerStop(timer);
	timer->state = TimerDestoryed;
}

void GetTimerInfo(Timer * timer, TimerInfo * info)//״̬��ѯ
{
	uint32_t status = TaskEnterCritical();

	info->startDelayTicks = timer->startDelayTicks;
	info->durationTicks = timer->delayTicks;
	info->timerFunc = timer->timerFunc;
	info->arg = timer->arg;
	info->config = timer->config;
	info->state = timer->state;
	
	TaskExitCritical(status);
}


void TimerModuleTickNotify(void){	//��ʱ���жϴ���
	uint32_t status = TaskEnterCritical();
	
	TimerCallFuncList(&TimerHardList);	//����Ӳ����ʱ������
	
	TaskExitCritical(status);
	
	SemNotify(&TimerTickSem);	//֪ͨ�ź���
}

#endif

