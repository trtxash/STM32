#ifndef _TOUCH_TASK_H
#define _TOUCH_TASK_H

#include "ft5446.h"
#include "tasks_common.h"

#define TASK_PRIO_TOUCH TASK_PRIO_NORMAL
#define TOUCH_STK_SIZE  STACK_SIZE_BASE

extern TaskHandle_t TouchTask_Handler; // 任务句柄

void vTouchTask(void *pvParameters);

#endif
