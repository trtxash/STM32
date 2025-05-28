#include "led_task.h"
#include "led.h"

TaskHandle_t LEDTask_Handler;

// led任务函数
void vLedTask(void *pvParameters)
{
    (void)pvParameters; // 明确标记未使用参数

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        LED0_Reverse();
        vTaskDelayUntil(&xLastWakeTime, 250);
    }
}
