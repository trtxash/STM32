#ifndef _KEY_TASK_H
#define _KEY_TASK_H

#include "tasks_common.h"

#define TASK_PRIO_KEY TASK_PRIO_HIGH
#define KEY_STK_SIZE  STACK_SIZE_LEVEL1

#define KeyQueueLen 16

extern QueueHandle_t xQueue_KEY;
extern TaskHandle_t KEYTask_Handler; // 任务句柄

void vKeyTask(void *pvParameters);

#endif
