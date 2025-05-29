#include "led_task.h"
#include "led.h"

TaskHandle_t LEDTask_Handler;
QueueHandle_t xQueue_Led = NULL;

// led任务函数
void vLedTask(void *pvParameters)
{
    (void)pvParameters; // 明确标记未使用参数

    xQueue_Led = xQueueCreate(LEDQueueLen, sizeof(LedCmd_t)); // 创建队列

    LedCmd_t cmd = {BREATHE, 0, 0, 128};
    xQueueSend(xQueue_Led, &cmd, pdMS_TO_TICKS(100)); // 发送控制命令

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        LED0_Reverse();
        vTaskDelayUntil(&xLastWakeTime, 250);
    }
}
