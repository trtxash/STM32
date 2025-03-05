#include "bsp_freertos.h"

#define START_TASK_PRIO 1       // 任务优先级
#define START_STK_SIZE 128      // 任务堆栈大小
TaskHandle_t StartTask_Handler; // 任务句柄
// void start_task(void *pvParameters); // 任务函数
void start_task(); // 任务函数

#define LED_TASK_PRIO 2       // 任务优先级
#define LED_STK_SIZE 32       // 任务堆栈大小
TaskHandle_t LEDTask_Handler; // 任务句柄
void led_task();              // 任务函数

#define TEST_TASK_PRIO 31      // 任务优先级
#define TEST_STK_SIZE 256      // 任务堆栈大小
TaskHandle_t TESTTask_Handler; // 任务句柄
void test_task();              // 任务函数

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
void start_task()
{
  taskENTER_CRITICAL(); // 进入临界区
  // 创建LED0任务
  // xTaskCreate((TaskFunction_t)led_task,
  //             (const char *)"led_task",
  //             (uint16_t)LED_STK_SIZE,
  //             (void *)NULL,
  //             (UBaseType_t)LED_TASK_PRIO,
  //             (TaskHandle_t *)&LEDTask_Handler);
  // 创建test任务
  xTaskCreate((TaskFunction_t)test_task,
              (const char *)"test_task",
              (uint16_t)TEST_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)TEST_TASK_PRIO,
              (TaskHandle_t *)&TESTTask_Handler);
  vTaskDelete(StartTask_Handler); // 删除开始任务
  taskEXIT_CRITICAL();            // 退出临界区
}

// led任务函数
void led_task()
{
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  while (1)
  {
    LED0_Reverse();
    vTaskDelayUntil(&xLastWakeTime, 500);
  }
}

// 测试任务函数
void test_task()
{
  static uint8_t flag = 0;
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  while (1)
  {
    flag = !flag;
    LED1_Reverse();
    taskENTER_CRITICAL(); // 进入临界区
    if (flag)
      SDRAM_WriteSpeedTest_32bits();
    else
      SDRAM_ReadSpeedTest();
    taskEXIT_CRITICAL(); // 退出临界区
    vTaskDelayUntil(&xLastWakeTime, 1000);
  }
}