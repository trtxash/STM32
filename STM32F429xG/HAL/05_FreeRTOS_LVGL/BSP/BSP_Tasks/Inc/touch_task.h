#ifndef _TOUCH_TASK_H
#define _TOUCH_TASK_H

#include "ft5446.h"
#include "tasks_common.h"

#define TASK_PRIO_TOUCH        TASK_PRIO_HIGH
#define TOUCH_STK_SIZE         STACK_SIZE_BASE
#define TOUCH_TaskCycleTime_ms 5 // 任务运行时间间隔

extern TaskHandle_t TouchTask_Handler;     // 任务句柄
extern uint8_t Touch_Data[Touch_Data_Len]; // 触摸屏原始读取数据

void vTouchTask(void *pvParameters);

#endif
