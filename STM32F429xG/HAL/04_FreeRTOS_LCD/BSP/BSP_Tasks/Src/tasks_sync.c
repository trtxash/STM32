#include "tasks_sync.h"
#include "key_task.h"
#include "led_task.h"

QueueHandle_t xQueue_KEY = NULL;
QueueHandle_t xQueue_Led = NULL;

void vSyncResources_Init(void)
{
    xQueue_KEY = xQueueCreate(KeyQueueLen, sizeof(PressEvent));
    xQueue_Led = xQueueCreate(LEDQueueLen, sizeof(LedCmd_t)); // 创建队列
}
