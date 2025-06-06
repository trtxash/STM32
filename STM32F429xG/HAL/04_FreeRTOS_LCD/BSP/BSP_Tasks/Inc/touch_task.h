#ifndef _TOUCH_TASK_H
#define _TOUCH_TASK_H

#include "ft5446.h"
#include "tasks_common.h"

#define TASK_PRIO_TOUCH TASK_PRIO_HIGH
#define TOUCH_STK_SIZE  STACK_SIZE_BASE

#define Touch_Data_Len 0X3F

extern TaskHandle_t TouchTask_Handler;     // 任务句柄
extern uint8_t Touch_Data[Touch_Data_Len]; // 触摸屏数据

void vTouchTask(void *pvParameters);

#endif
