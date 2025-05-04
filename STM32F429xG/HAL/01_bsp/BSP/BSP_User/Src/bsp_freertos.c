#include "bsp_freertos.h"

#define START_TASK_PRIO 1       // 任务优先级
#define START_STK_SIZE  128     // 任务堆栈大小
TaskHandle_t StartTask_Handler; // 任务句柄
// void start_task(void *pvParameters); // 任务函数
void start_task(void); // 任务函数

#define LED_TASK_PRIO 2       // 任务优先级
#define LED_STK_SIZE  32      // 任务堆栈大小
TaskHandle_t LEDTask_Handler; // 任务句柄
void led_task(void);          // 任务函数

void freertos_main(void)
{
    // 创建开始任务
    xTaskCreate((TaskFunction_t)start_task,          // 任务函数
                (const char *)"start_task",          // 任务名称
                (uint16_t)START_STK_SIZE,            // 任务堆栈大小
                (void *)NULL,                        // 传递给任务函数的参数
                (UBaseType_t)START_TASK_PRIO,        // 任务优先级
                (TaskHandle_t *)&StartTask_Handler); // 任务句柄
    vTaskStartScheduler();
}

// 开始任务任务函数
void start_task(void)
{
    taskENTER_CRITICAL(); // 进入临界区

    // 创建LED0任务
    xTaskCreate((TaskFunction_t)led_task,
                (const char *)"led_task",
                (uint16_t)LED_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)LED_TASK_PRIO,
                (TaskHandle_t *)&LEDTask_Handler);

    vTaskDelete(StartTask_Handler); // 删除开始任务
    taskEXIT_CRITICAL();            // 退出临界区
}

// led任务函数
void led_task(void)
{
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        LED0_Reverse();
        vTaskDelayUntil(&xLastWakeTime, 500);
    }
}
