#ifndef __TASKS_SYNC_H__
#define __TASKS_SYNC_H__

#include "adc_task.h"
#include "cpu_task.h"
#include "key_task.h"
#include "led_task.h"
#include "tasks_common.h"
#include "touch_task.h"

#define KeyQueueLen   16
#define LEDQueueLen   16
#define ADCQueueLen   16
#define CPUQueueLen   16
#define TouchQueueLen 16

extern QueueHandle_t xQueue_KEY;
extern QueueHandle_t xQueue_Led;
extern QueueHandle_t xQueue_ADC_Temp;
extern QueueHandle_t xQueue_ADC_Bat;
extern QueueHandle_t xQueue_ADC_BatVolt;
extern QueueHandle_t xQueue_CPU;
extern QueueHandle_t xQueue_Touch;

extern QueueHandle_t xSemaphore_ADC;
extern QueueHandle_t xSemaphore_Touch_i2c;
extern QueueHandle_t xSemaphore_VSync;
extern QueueHandle_t xSemaphore_VSync_Wait;

void vSyncResources_Init(void);

#endif /* __TASKS_SYNC_H__ */
