#ifndef _LED_TASK_H
#define _LED_TASK_H

#include "led.h"
#include "multi_led.h"
#include "tasks_common.h"

#define TASK_PRIO_LED       TASK_PRIO_NORMAL // 任务优先级,越大越高优先级
#define LED_TSTK_SIZE       STACK_SIZE_BASE  // 任务堆栈大小,实际为32word=32*4byte=128byte=128*8bit
#define LED_TaskCycleTime_ms 100              // 任务运行时间间隔

extern TaskHandle_t LEDTask_Handler;

extern Led_t led0;
extern Led_t led1;
extern Led_t led2;
extern Led_t led3;

void vLedTask(void *pvParameters); // 任务函数

#endif /* _LED_TASK_H */
