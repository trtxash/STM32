#ifndef _GUI_TASK_H
#define _GUI_TASK_H

#include "tasks_common.h"

#define TASK_PRIO_GUI TASK_PRIO_HIGH
#define GUI_STK_SIZE  STACK_SIZE_LEVEL1

extern TaskHandle_t GUITask_Handler; // 任务句柄

void vGUITask(void *pvParameters);

#endif
