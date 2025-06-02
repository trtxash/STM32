#include "tasks_sync.h"

QueueHandle_t xQueue_KEY = NULL;
QueueHandle_t xQueue_Led = NULL;
QueueHandle_t xQueue_ADC_Temp = NULL;
QueueHandle_t xQueue_ADC_Bat = NULL;
QueueHandle_t xQueue_ADC_BatVolt = NULL;
QueueHandle_t xSemaphore_ADC = NULL;
QueueHandle_t xQueue_CPU = NULL;

void vSyncResources_Init(void)
{
    // Create Queues for KEYs
    xQueue_KEY = xQueueCreate(KeyQueueLen, sizeof(Button));
    xQueue_Led = xQueueCreate(LEDQueueLen, sizeof(Led_t));
    xQueue_ADC_Temp = xQueueCreate(ADCQueueLen, sizeof(float));
    xQueue_ADC_Bat = xQueueCreate(ADCQueueLen, sizeof(float));
    xQueue_ADC_BatVolt = xQueueCreate(ADCQueueLen, sizeof(float));
    xQueue_CPU = xQueueCreate(CPUQueueLen, sizeof(CPU_RunInfo));

    xSemaphore_ADC = xSemaphoreCreateBinary(); // 创建二值信号量
}
