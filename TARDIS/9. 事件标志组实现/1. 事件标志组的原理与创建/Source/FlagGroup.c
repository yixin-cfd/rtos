#include "TARDIS_OS.h"

void FlagGroupInit(FlagGroup * flagGroup, uint32_t flags)	//��ʼ��
{
	EventInit(&flagGroup->event, EventTypeFlagGroup);
	flagGroup->flag = flags;
}

