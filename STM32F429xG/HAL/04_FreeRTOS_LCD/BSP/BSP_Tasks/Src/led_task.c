#include "led_task.h"
#include "tasks_sync.h"

TaskHandle_t LEDTask_Handler;



// led任务函数
void vLedTask(void *pvParameters)
{
    (void)pvParameters; // 明确标记未使用参数

    // LedCmd_t cmd = {BREATHE, 0, 0, 128};
    // xQueueSend(xQueue_Led, &cmd, pdMS_TO_TICKS(100)); // 发送控制命令
    if (xQueue_Led == NULL)
    {
        vSyncResources_Init();
    }
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        LED0_Reverse();
        vTaskDelayUntil(&xLastWakeTime, 250);
    }
}
