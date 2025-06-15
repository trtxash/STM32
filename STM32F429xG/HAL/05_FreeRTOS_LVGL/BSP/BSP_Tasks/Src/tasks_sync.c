#include "tasks_sync.h"

QueueHandle_t xQueue_KEY = NULL;
QueueHandle_t xQueue_Led = NULL;
QueueHandle_t xQueue_ADC_Temp = NULL;
QueueHandle_t xQueue_ADC_Bat = NULL;
QueueHandle_t xQueue_ADC_BatVolt = NULL;
QueueHandle_t xQueue_CPU = NULL;
QueueHandle_t xQueue_Touch = NULL;

QueueHandle_t xSemaphore_ADC = NULL;
QueueHandle_t xSemaphore_Touch_i2c = NULL;
QueueHandle_t xSemaphore_VSync = NULL;
QueueHandle_t xSemaphore_VSync_Wait = NULL;

void vSyncResources_Init(void)
{
    // Create Queues for KEYs
    xQueue_KEY = xQueueCreate(KeyQueueLen, sizeof(Button));
    xQueue_Led = xQueueCreate(LEDQueueLen, sizeof(Led_t));
    xQueue_ADC_Temp = xQueueCreate(ADCQueueLen, sizeof(float));
    xQueue_ADC_Bat = xQueueCreate(ADCQueueLen, sizeof(float));
    xQueue_ADC_BatVolt = xQueueCreate(ADCQueueLen, sizeof(float));
    xQueue_CPU = xQueueCreate(CPUQueueLen, sizeof(CPU_RunInfo));
    xQueue_Touch = xQueueCreate(TouchQueueLen, sizeof(_m_tp_dev));

    xSemaphore_ADC = xSemaphoreCreateBinary();
    xSemaphore_Touch_i2c = xSemaphoreCreateBinary();
    xSemaphore_VSync = xSemaphoreCreateBinary();
    xSemaphore_VSync_Wait = xSemaphoreCreateBinary();
}
