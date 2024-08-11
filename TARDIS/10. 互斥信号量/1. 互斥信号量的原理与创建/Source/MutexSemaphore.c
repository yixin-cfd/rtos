#include "TARDIS_OS.h"


void MutexInit(Mutex * mutex)	//³õÊ¼»¯
{
	EventInit(&mutex->event, EventTypeMutex);
	
	mutex->lockedCount = 0;
	mutex->owner = (Task *)0;
	mutex->ownerOriginalPrio = TARDIS_OS_PRIORITY_COUNT;
	
}

