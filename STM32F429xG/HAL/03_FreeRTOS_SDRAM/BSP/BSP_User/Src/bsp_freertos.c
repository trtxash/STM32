#include "bsp_freertos.h"
#include "key.h"
#include "led.h"
#include "log_rtt.h"
#include "sdram.h"

u8 keyval = 0;

#define START_TASK_PRIO 1       // 任务优先级,越大越高优先级
#define START_STK_SIZE  128 + 1 // 任务堆栈大小
TaskHandle_t StartTask_Handler; // 任务句柄
// void start_task(void *pvParameters); // 任务函数
void start_task(void); // 任务函数

#define LED_TASK_PRIO 14      // 任务优先级,越大越高优先级
#define LED_TSTK_SIZE 32 + 1  // 任务堆栈大小,实际为32word=32*4byte=128byte=128*8bit
TaskHandle_t LEDTask_Handler; // 任务句柄
void led_task(void);          // 任务函数

#define KEY_TASK_PRIO 15      // 任务优先级,越大越高优先级
#define KEY_TSTK_SIZE 256 + 1 // 任务堆栈大小,实际为32word=32*4byte=128byte=128*8bit
TaskHandle_t KEYTask_Handler; // 任务句柄
void key_task(void);          // 任务函数

#define TEST_TASK_PRIO 11      // 任务优先级,越大越高优先级
#define TEST_TSTK_SIZE 256 + 1 // 任务堆栈大小
TaskHandle_t TESTTask_Handler; // 任务句柄
void test_task(void);          // 任务函数

// #define TEST_TASK_PRIO 11      // 任务优先级,越大越高优先级
// #define TEST_TSTK_SIZE 256 + 1 // 任务堆栈大小
// TaskHandle_t TESTTask_Handler; // 任务句柄
// void test_task(void);          // 任务函数

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
                (uint16_t)LED_TSTK_SIZE,
                (void *)NULL,
                (UBaseType_t)LED_TASK_PRIO,
                (TaskHandle_t *)&LEDTask_Handler);

    // 创建KEY任务
    xTaskCreate((TaskFunction_t)key_task,
                (const char *)"key_task",
                (uint16_t)KEY_TSTK_SIZE,
                (void *)NULL,
                (UBaseType_t)KEY_TASK_PRIO,
                (TaskHandle_t *)&KEYTask_Handler);

    // 创建test任务
    xTaskCreate((TaskFunction_t)test_task,
                (const char *)"test_task",
                (uint16_t)TEST_TSTK_SIZE,
                (void *)NULL,
                (UBaseType_t)TEST_TASK_PRIO,
                (TaskHandle_t *)&TESTTask_Handler);

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

// key任务函数
void key_task(void)
{
    // uint32_t i = 0;
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {

        KEY_Scan(&KEY_UP_DATA, KEY_UP_READ());
        // if (KEY_UP_DATA.K_value)
        // {
        //     i++;
        //     KEY_UP_DATA.K_value = 0;
        //     LOGI("KEY_UP,i=%u", i);
        // }
        if (KEY_UP_DATA.K_value)
        {
            static uint8_t flag = 1;

            KEY_UP_DATA.K_value = 0;
            if (flag)
            {
                LED2_Clr();
                vTaskSuspend(TESTTask_Handler);
                flag = !flag;
            }
            else
            {
                LED2_Set();
                vTaskResume(TESTTask_Handler);
                flag = !flag;
            }
        }
        vTaskDelayUntil(&xLastWakeTime, 50);
    }
}

// 测试任务函数
void test_task(void)
{
    static uint8_t flag = 0;
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        flag = !flag;
        // LED1_Reverse();
        // taskENTER_CRITICAL(); // 进入临界区
        if (flag)
        {
            LED1_Set();
            SDRAM_WriteSpeedTest_32bits();
        }
        else
        {
            LED1_Clr();
            SDRAM_ReadSpeedTest();
        }
        // taskEXIT_CRITICAL(); // 退出临界区
        vTaskDelayUntil(&xLastWakeTime, 1000);
    }
}
