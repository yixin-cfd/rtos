#include "TARDIS_OS.h"


//������ʱ����
void TaskDelay(uint32_t delay){					
	uint32_t status = TaskEnterCritical();
	TimeTaskWait(currentTask,delay);  //������ʱ���ú������޸��������ʱʱ�䡢��������뵽��ʱ�����С��޸������״̬Ϊ��ʱ״̬		
	TaskSchedUnRdy(currentTask);	  //������Ӿ���������ɾ�������������ȼ���û�������޸�λͼ
	TaskExitCritical(status);		  
	TaskSched();
}

