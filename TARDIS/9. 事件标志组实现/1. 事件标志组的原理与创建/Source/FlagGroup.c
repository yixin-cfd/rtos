#include "TARDIS_OS.h"

void FlagGroupInit(FlagGroup * flagGroup, uint32_t flags)	//³õÊ¼»¯
{
	EventInit(&flagGroup->event, EventTypeFlagGroup);
	flagGroup->flag = flags;
}

