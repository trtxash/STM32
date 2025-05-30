#include "tasks_sync.h"

QueueHandle_t xQueue_KEY = NULL;
// QueueHandle_t xQueue_Led[LedNumber];

void vSyncResources_Init(void)
{
    // Create Queues for KEYs
    xQueue_KEY = xQueueCreate(KeyQueueLen, sizeof(PressEvent));
    // Create Queues for LEDs
    // for (int i = 0; i < LedNumber; i++)
    // {
    //     xQueue_Led[i] = xQueueCreate(LEDQueueLen, sizeof(Led_t));
    // }
}
