#ifndef _CPU_TASK_H
#define _CPU_TASK_H

#include "tasks_common.h"

#define TASK_PRIO_CPU TASK_PRIO_NORMAL
#define CPU_STK_SIZE  STACK_SIZE_BASE

#define CPU_RUNINFO_SIZE (512)

extern TaskHandle_t CPUTask_Handler; // 任务句柄
extern char CPU_RunInfo[CPU_RUNINFO_SIZE];

void vCPUTask(void *pvParameters);

#endif
