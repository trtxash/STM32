#include "led_task.h"
#include "led.h"
#include "multi_led.h"
#include "tasks_sync.h"

TaskHandle_t LEDTask_Handler;

Led_t led0;
Led_t led1;
Led_t led2;
Led_t led3;

static uint8_t set_led_GPIO(uint8_t led_id, uint8_t order)
{
    // you can share the GPIO read function with multiple Buttons
    switch (led_id)
    {
    case 0:
        HAL_GPIO_WritePin(LED0_GPIO, LED0_PIN, order);
        break;
    case 1:
        HAL_GPIO_WritePin(LED1_GPIO, LED1_PIN, order);
        break;
    case 2:
        HAL_GPIO_WritePin(LED2_GPIO, LED2_PIN, order);
        break;
    case 3:
        HAL_GPIO_WritePin(LED3_GPIO, LED3_PIN, order);
        break;
    default:
        return 0;
        break;
    }
    return 1;
}

// led任务函数
void vLedTask(void *pvParameters)
{
    (void)pvParameters; // 明确标记未使用参数

    // xQueueSend(xQueue_Led, &cmd, pdMS_TO_TICKS(100)); // 发送控制命令

    led_init(&led0, set_led_GPIO, 1, 0, SOLID); // 初始化LED
    led_init(&led1, set_led_GPIO, 1, 1, BLINK);
    led_init(&led2, set_led_GPIO, 1, 2, SOLID);
    led_init(&led3, set_led_GPIO, 1, 3, BLINK);

    led_set(&led0, SOLID, 1, 1, 0);
    led_set(&led1, SOLID, 2, 1, 0);
    led_set(&led2, SOLID, 3, 1, 0);
    led_set(&led3, BLINK, 10, 5, 1);

    led_start(&led0);
    led_start(&led1);
    led_start(&led2);
    led_start(&led3);

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        // 刷新LED状态
        led_ticks();
        // 读取控制命令
        Led_t led_temp;
        if (xQueueReceive(xQueue_Led, &led_temp, 0) == pdPASS)
        {
            switch (led_temp.led_id)
            {
            case 0:
                led_set(&led0, led_temp.mode, led_temp.cycle, led_temp.dutycycle, led_temp.led_level);
                break;
            case 1:
                led_set(&led1, led_temp.mode, led_temp.cycle, led_temp.dutycycle, led_temp.led_level);
                break;
            case 2:
                led_set(&led2, led_temp.mode, led_temp.cycle, led_temp.dutycycle, led_temp.led_level);
                break;
            case 3:
                led_set(&led3, led_temp.mode, led_temp.cycle, led_temp.dutycycle, led_temp.led_level);
                break;
            default:
                break;
            };
        }
        vTaskDelayUntil(&xLastWakeTime, LED_TaskCycleTime_ms);
    }
    vTaskDelete(NULL);
}
