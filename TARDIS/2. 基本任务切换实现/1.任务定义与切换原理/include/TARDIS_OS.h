#ifndef TARDIS_OS_H
#define TARDIS_OS_H

#include<stdint.h>

typedef uint32_t TaskStack;


typedef struct _Task{
	TaskStack * stack;			//��ջָ��
}Task;

#endif
