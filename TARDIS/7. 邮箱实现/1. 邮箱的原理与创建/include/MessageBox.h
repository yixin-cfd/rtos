#ifndef _MESSAGEBOX_H
#define _MESSAGEBOX_H

#include "Event.h"

typedef struct _Mbox{
	Event event;		//�¼����ƿ�
	uint32_t count;		//��ǰ����Ϣ����
	uint32_t read;		//��ȡ��Ϣ������
	uint32_t write;		//д��Ϣ������
	uint32_t maxCount;	//����������ɵ���Ϣ����
	void ** msgBuffer;	//��Ϣ�洢������
}Mbox;

void MboxInit(Mbox * mbox, void ** msgBuffer, uint32_t maxCount);	//�����ʼ��

#endif

