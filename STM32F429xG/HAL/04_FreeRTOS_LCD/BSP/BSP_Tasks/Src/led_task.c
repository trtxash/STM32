#include "led_task.h"
#include "tasks_sync.h"

TaskHandle_t LEDTask_Handler;
// Led_t led[LedNumber] = {0};

// led任务函数
void vLedTask(void *pvParameters)
{
    (void)pvParameters; // 明确标记未使用参数

    // xQueueSend(xQueue_Led, &cmd, pdMS_TO_TICKS(100)); // 发送控制命令

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        LED0_Reverse();
        vTaskDelayUntil(&xLastWakeTime, 250);
    }
}
