#ifndef _TIMER_H
#define _TIMER_H

#include "DS_Lib.h"

#define TIMER_CONFIG_TYPE_HARD			(1 << 0)	//��systick�ж��д���ʱ��
#define TIMER_CONFIG_TYPE_SOFT			(0 << 0)	//�ڶ�ʱ�������д���

typedef enum _TimerState{
	TimerCreated,		//��ʱ����������
	TimerStarted,		//��ʱ����������
	TimerRunning,		//��ʱ����������
	TimerStopped,		//��ʱ��ֹͣ
	TimerDestoryed		//��ʱ���Ѿ���������
}TimerState;


typedef struct _Timer{
	Node linkNode;				//���ӽڵ�
	uint32_t startDelayTicks;	//��ʼ�ȴ�ʱ��
	uint32_t durationTicks;		//������ʱʱ��
	uint32_t delayTicks;		//��ǰ����ֵ
	void (*timerFunc)(void * arg);	//��ʱ��������
	void * arg;					//����
	uint32_t config;			//��ʱ�����ò��� 
	
	TimerState state;			//��ʱ��״̬
}Timer;

typedef struct _TimerInfo{
	uint32_t startDelayTicks;	//��ʼ�ȴ�ʱ��
	uint32_t durationTicks;		//������ʱʱ��
	void (*timerFunc)(void * arg);	//��ʱ��������
	void * arg;					//����
	uint32_t config;			//��ʱ�����ò��� 
	
	TimerState state;			//��ʱ��״̬
}TimerInfo;


//��ʱ���ڵ��ʼ������
void TimerInit(Timer * timer, uint32_t startDelayTicks, uint32_t durationTicks, void (*timerFunc)(void * arg), void * arg, uint32_t config);
void TimerStart(Timer * timer);	//��ʱ������
void TimerStop(Timer * timer);	//��ʱ��ֹͣ
void TimerModuleInit(void);	//��ʱ��ģ���ʼ��
void TimerModuleTickNotify(void);//��ʱ���жϴ���

void TimerDestory(Timer * timer);	//ɾ����ʱ��
void GetTimerInfo(Timer * timer, TimerInfo * info);//״̬��ѯ
#endif



