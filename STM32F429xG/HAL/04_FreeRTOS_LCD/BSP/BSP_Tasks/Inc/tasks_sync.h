#ifndef __TASKS_SYNC_H__
#define __TASKS_SYNC_H__

#include "adc_task.h"
#include "cpu_task.h"
#include "key_task.h"
#include "led_task.h"
#include "tasks_common.h"

#define KeyQueueLen 16
#define LEDQueueLen 16
#define ADCQueueLen 16
#define CPUQueueLen 16

extern QueueHandle_t xQueue_KEY;
extern QueueHandle_t xQueue_Led;
extern QueueHandle_t xQueue_ADC_Temp;
extern QueueHandle_t xQueue_ADC_Bat;
extern QueueHandle_t xQueue_ADC_BatVolt;
extern QueueHandle_t xQueue_CPU;

extern QueueHandle_t xSemaphore_ADC;

void vSyncResources_Init(void);

#endif /* __TASKS_SYNC_H__ */
