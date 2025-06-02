#ifndef _ADC_TASK_H
#define _ADC_TASK_H

#include "adc.h"
#include "filter.h"
#include "tasks_common.h"

#define TASK_PRIO_ADC TASK_PRIO_NORMAL
#define ADC_STK_SIZE  STACK_SIZE_LEVEL1

extern TaskHandle_t ADCTask_Handler; // 任务句柄

float battery_voltage_to_soc(float voltage);
void vADCTask(void *pvParameters);

#endif
