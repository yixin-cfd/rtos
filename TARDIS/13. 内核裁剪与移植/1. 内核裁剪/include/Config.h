#ifndef TARDIS_OS_CONFIG_H
#define TARDIS_OS_CONFIG_H

#define TARDIS_OS_PRIORITY_COUNT        32	//���ȼ�����&λͼ����
#define TARDIS_OS_SLICE_MAX				10	//ʱ��Ƭ�Ĵ�С

#define TARDIS_OS_IDLETASK_STACK_SIZE   1024//���������ջ�Ĵ�С

#define TARDIS_OS_TIMERTASK_STACK_SIZE  1024//��ʱ������Ķ�ջ��С
#define TARDIS_OS_TIMERTASK_PRIO		1	//��ʱ����������ȼ�

#define TARDIS_OS_SYSTICK_MS			10	//ϵͳʱ��systick���ж�����



/*************************�ں˲ü�*****************************************/
#define	TARDIS_OS_ENABLE_SEMAPHORE		0	//�ź���
#define TARDIS_OS_ENABLE_MESSAGEBOX     0   //����
#define TARDIS_OS_ENABLE_MEMORYBLOCK	0	//�洢��
#define TARDIS_OS_ENABLE_FLAGGROUP		0	//�¼���־��
#define TARDIS_OS_ENABLE_MUTEX			0	//�����ź���
#define TARDIS_OS_ENABLE_TIMER			0	//��ʱ��
#define TARDIS_OS_ENABLE_CPUUSAGE_STAT	0	//CPU������
/*************************�ں˲ü�*****************************************/
#endif























