#include "tasks_sync.h"

QueueHandle_t xQueue_KEY = NULL;
QueueHandle_t xQueue_Led = NULL;

void vSyncResources_Init(void)
{
    // Create Queues for KEYs
    xQueue_KEY = xQueueCreate(KeyQueueLen, sizeof(Button));
    xQueue_Led = xQueueCreate(LEDQueueLen, sizeof(Led_t));
}
