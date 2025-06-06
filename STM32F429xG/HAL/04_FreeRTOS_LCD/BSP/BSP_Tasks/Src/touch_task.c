#include "touch_task.h"
#include "tasks_sync.h"

TaskHandle_t TouchTask_Handler = NULL; // 任务句柄

void vTouchTask(void *pvParameters)
{
    (void)pvParameters;

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        vTaskDelayUntil(&xLastWakeTime, 10);
    }
}
