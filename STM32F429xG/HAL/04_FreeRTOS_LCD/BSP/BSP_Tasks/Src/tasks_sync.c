#include "tasks_sync.h"

QueueHandle_t xQueue_KEY = NULL;
QueueHandle_t xQueue_Led = NULL;
QueueHandle_t xQueue_ADC = NULL;
QueueHandle_t xSemaphore_ADC = NULL;

void vSyncResources_Init(void)
{
    // Create Queues for KEYs
    xQueue_KEY = xQueueCreate(KeyQueueLen, sizeof(Button));
    xQueue_Led = xQueueCreate(LEDQueueLen, sizeof(Led_t));
    xQueue_ADC = xQueueCreate(ADCQueueLen, sizeof(float));

    xSemaphore_ADC = xSemaphoreCreateBinary(); // 创建二值信号量
}
