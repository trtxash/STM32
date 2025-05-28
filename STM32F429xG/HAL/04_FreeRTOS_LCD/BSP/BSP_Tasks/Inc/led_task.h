#ifndef _LED_TASK_H
#define _LED_TASK_H

#include "tasks_common.h"

#define TASK_PRIO_LED TASK_PRIO_NORMAL // 任务优先级,越大越高优先级
#define LED_TSTK_SIZE STACK_SIZE_BASE  // 任务堆栈大小,实际为32word=32*4byte=128byte=128*8bit

extern TaskHandle_t LEDTask_Handler; // 任务句柄

void vLedTask(void *pvParameters); // 任务函数

#endif /* _LED_TASK_H */
