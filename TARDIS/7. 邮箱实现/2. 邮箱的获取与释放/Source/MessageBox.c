#include "TARDIS_OS.h"


void MboxInit(Mbox * mbox, void ** msgBuffer, uint32_t maxCount)	//�����ʼ��
{
	EventInit(&mbox->event, EventTypeMbox);	//��ʼ���¼����ƿ�

	mbox->msgBuffer = msgBuffer;			//�޸���Ϣ������
	mbox->maxCount = maxCount;				//��Ϣ�������
	mbox->read = 0;							//������
	mbox->write = 0;						//д����
	mbox->count = 0;						//��ǰ����ֵΪ��
}

uint32_t MboxWait(Mbox * mbox, void **msg, uint32_t waitTicks)		//��ȡ���䣬�ȴ���Ϣ
{
	uint32_t status = TaskEnterCritical();
	
	if( mbox->count > 0){	//�����������Ϣ�Ļ�
		--mbox->count;		//��Ϣ������һ
		*msg = mbox->msgBuffer[mbox->read++];	//���ö�������ȡ��Ϣ
		if( mbox->read >= mbox->maxCount){		//��֯��ѭ������
			mbox->read = 0;
		}
		TaskExitCritical(status);
		return NoneError;
	}
	else{
		EventWait( &mbox->event, currentTask, (void *)0, EventTypeMbox, waitTicks);	//��������뵽�¼����ƿ���
		TaskExitCritical(status);
		
		TaskSched();	//�������
		
		*msg = currentTask->eventMsg;			//ȡ������ȴ�����Ϣ
		return currentTask->waitEventResult;	//��������ȴ����
	}
}


uint32_t MboxNoWait(Mbox * mbox, void **msg)		//��ȡ���䣬���ȴ���Ϣ
{
	uint32_t status = TaskEnterCritical();
	
	if( mbox->count > 0){	//�����������Ϣ�Ļ�
		--mbox->count;		//��Ϣ������һ
		*msg = mbox->msgBuffer[mbox->read++];	//���ö�������ȡ��Ϣ
		if( mbox->read >= mbox->maxCount){		//��֯��ѭ������
			mbox->read = 0;
		}
		TaskExitCritical(status);
		return NoneError;
	}
	else{
		TaskExitCritical(status);
		return ErrorResourceUnavaliable;	//������Դ������
	}
}

uint32_t MboxNotify(Mbox * mbox, void * msg, uint32_t notifyOption)		//�����䷢����Ϣ
{
	uint32_t status = TaskEnterCritical();
	
	if( EventWaitCount(&mbox->event) > 0){		//����������ڵȴ�
		Task * task = EventWakeUp( &mbox->event, msg, NoneError);	//����һ������
		if ( task->priority < currentTask->priority){			//������ѵ���������ȼ�����
			TaskSched();		//�����������
		}
	}
	else{
		if( mbox->count == mbox->maxCount){		//�����Ϣ�Ѿ�����
			TaskExitCritical(status);
			return ErrorResourceFull;			//������Դ����
		}
		//��Ϣδ��
		if(notifyOption & MBOXSendFront){		//����Ǹ����ȼ�����Ϣ���嵽ͷ��
			if(mbox->read <= 0){
				mbox->read = mbox->maxCount - 1;
			}
			else{
				--mbox->read;
			}
			mbox->msgBuffer[mbox->read] = msg;
		}
		else{	//��ͨ��ʽ���� 	
			mbox->msgBuffer[mbox->write++] = msg;
			if( mbox->write == mbox->maxCount){
				mbox->write = 0;
			}
		}
		mbox->count++;
	}
	
	TaskExitCritical(status);
	return NoneError;
}


