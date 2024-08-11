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

