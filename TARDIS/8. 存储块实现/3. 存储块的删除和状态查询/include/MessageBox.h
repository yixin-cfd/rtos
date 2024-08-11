#ifndef _MESSAGEBOX_H
#define _MESSAGEBOX_H

#include "Event.h"

#define MBOXSendNormal			0x00		
#define MBOXSendFront		 	0x01

typedef struct _Mbox{
	Event event;		//�¼����ƿ�
	uint32_t count;		//��ǰ����Ϣ����
	uint32_t read;		//��ȡ��Ϣ������
	uint32_t write;		//д��Ϣ������
	uint32_t maxCount;	//����������ɵ���Ϣ����
	void ** msgBuffer;	//��Ϣ�洢������
}Mbox;


typedef struct _MboxInfo{
	uint32_t count;		//��ǰ����Ϣ����
	uint32_t maxCount;	//����������ɵ���Ϣ����
	uint32_t taskCount;	//��ǰ�ȴ���������
}MboxInfo;

void MboxInit(Mbox * mbox, void ** msgBuffer, uint32_t maxCount);	//�����ʼ��
uint32_t MboxWait(Mbox * mbox, void **msg, uint32_t waitTicks);		//��ȡ���䣬�ȴ���Ϣ
uint32_t MboxNoWait(Mbox * mbox, void **msg);						//��ȡ���䣬���ȴ���Ϣ
uint32_t MboxNotify(Mbox * mbox, void * msg, uint32_t notifyOption);//�����䷢����Ϣ

void MboxClear(Mbox * mbox);		//�������
uint32_t  MboxDestory(Mbox * mbox);	//ɾ������

void GetMboxInfo(Mbox * mbox, MboxInfo * info);		//״̬��ѯ
#endif

