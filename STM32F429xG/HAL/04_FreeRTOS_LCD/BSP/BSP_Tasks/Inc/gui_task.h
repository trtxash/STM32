#ifndef _GUI_TASK_H
#define _GUI_TASK_H

#include "lcd.h"
#include "tasks_common.h"

#define TASK_PRIO_GUI        TASK_PRIO_NORMAL
#define GUI_STK_SIZE         STACK_SIZE_LEVEL2
// #define GUI_TaskCycleTime_ms 8 // 任务运行时间间隔,125Hz
#define GUI_TaskCycleTime_ms 15 // 任务运行时间间隔,66.66Hz

extern TaskHandle_t GUITask_Handler; // 任务句柄

void vGUITask(void *pvParameters);

#endif
