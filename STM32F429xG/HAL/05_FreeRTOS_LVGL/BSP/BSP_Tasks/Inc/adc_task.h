#ifndef _ADC_TASK_H
#define _ADC_TASK_H

#include "adc.h"
#include "filter.h"
#include "tasks_common.h"

#define TASK_PRIO_ADC       TASK_PRIO_NORMAL
#define ADC_STK_SIZE        STACK_SIZE_LEVEL1
#define ADC_TaskCycleTime_ms 10 // 任务运行时间间隔

extern TaskHandle_t ADCTask_Handler; // 任务句柄

void vADCTask(void *pvParameters);

#endif
