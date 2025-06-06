#ifndef _GUI_TASK_H
#define _GUI_TASK_H

#include "lcd.h"
#include "tasks_common.h"

#define TASK_PRIO_GUI TASK_PRIO_LOW
#define GUI_STK_SIZE  STACK_SIZE_LEVEL3

extern TaskHandle_t GUITask_Handler; // 任务句柄

void vGUITask(void *pvParameters);

#endif
