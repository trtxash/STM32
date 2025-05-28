#ifndef _START_TASK_H
#define _START_TASK_H

#include "tasks_common.h"

#define TASK_PRIO_START TASK_PRIO_LOW 
#define START_STK_SIZE  STACK_SIZE_BASE

void freertos_enter(void);

#endif
