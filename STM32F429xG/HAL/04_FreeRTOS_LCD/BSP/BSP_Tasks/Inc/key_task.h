#ifndef _KEY_TASK_H
#define _KEY_TASK_H

#include "key.h"
#include "multi_button.h"
#include "tasks_common.h"

#define TASK_PRIO_KEY        TASK_PRIO_LOW
#define KEY_STK_SIZE         STACK_SIZE_BASE
#define KEY_TaskCycleTime_ms 50 // 任务运行时间间隔

extern TaskHandle_t KEYTask_Handler; // 任务句柄
extern Button button_up;

void vKeyTask(void *pvParameters);

#endif
