#include "tasks_sync.h"
#include "key.h"
#include "key_task.h"


QueueHandle_t xQueue_KEY = NULL;

void vSyncResources_Init(void)
{
    xQueue_KEY = xQueueCreate(KeyQueueLen, sizeof(PressEvent));
}
