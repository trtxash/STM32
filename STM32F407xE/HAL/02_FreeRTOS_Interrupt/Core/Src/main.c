/**
 * @file	  02_FreeRTOS_Interrupt
 * @brief 	移植FreeRTOS和中断
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	  2022年8月22号23点00分
 */
#include "main.h"

#define Debug 1 // 控制Debug的一些相关函数

#define START_TASK_PRIO 1            //任务优先级
#define START_STK_SIZE 128           //任务堆栈大小
TaskHandle_t StartTask_Handler;      //任务句柄
void start_task(void *pvParameters); //任务函数

#define LED0_TASK_PRIO 2            //任务优先级
#define LED0_STK_SIZE 50            //任务堆栈大小
TaskHandle_t LED0Task_Handler;      //任务句柄
void led0_task(void *pvParameters); //任务函数

#define LED1_TASK_PRIO 3            //任务优先级
#define LED1_STK_SIZE 50            //任务堆栈大小
TaskHandle_t LED1Task_Handler;      //任务句柄
void led1_task(void *pvParameters); //任务函数

#define FLOAT_TASK_PRIO 4            //任务优先级
#define FLOAT_STK_SIZE 256           //任务堆栈大小
TaskHandle_t FLOATTask_Handler;      //任务句柄
void float_task(void *pvParameters); //任务函数

/**
 * @brief   主函数,程序入口
 * @param   none
 * @arg		  none
 * @note    循环点亮和熄灭LED
 * @retval  int
 */
int main(void)
{
  if (HAL_Init()) // 初始化HAL库
  {
    Error_Handler();
  }
  Stm32_Clock_Init(168, 4, 2, 4); // 初始化时钟
  delay_init(168);                // 初始化延时函数
  uart6_init(115200);             // 初始化串口
  LED_Init();                     // 初始化LED

  //创建开始任务
  xTaskCreate((TaskFunction_t)start_task,          //任务函数
              (const char *)"start_task",          //任务名称
              (uint16_t)START_STK_SIZE,            //任务堆栈大小
              (void *)NULL,                        //传递给任务函数的参数
              (UBaseType_t)START_TASK_PRIO,        //任务优先级
              (TaskHandle_t *)&StartTask_Handler); //任务句柄
  vTaskStartScheduler();                           //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{
  taskENTER_CRITICAL(); //进入临界区
  //创建LED0任务
  xTaskCreate((TaskFunction_t)led0_task,
              (const char *)"led0_task",
              (uint16_t)LED0_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)LED0_TASK_PRIO,
              (TaskHandle_t *)&LED0Task_Handler);
  //创建LED1任务
  xTaskCreate((TaskFunction_t)led1_task,
              (const char *)"led1_task",
              (uint16_t)LED1_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)LED1_TASK_PRIO,
              (TaskHandle_t *)&LED1Task_Handler);
  //浮点测试任务
  xTaskCreate((TaskFunction_t)float_task,
              (const char *)"float_task",
              (uint16_t)FLOAT_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)FLOAT_TASK_PRIO,
              (TaskHandle_t *)&FLOATTask_Handler);
  vTaskDelete(StartTask_Handler); //删除开始任务
  taskEXIT_CRITICAL();            //退出临界区
}

// LED0任务函数
void led0_task(void *pvParameters)
{
  while (1)
  {
    LED0_Reverse();
    vTaskDelay(500);
  }
}

// LED1任务函数
void led1_task(void *pvParameters)
{
  while (1)
  {
    LED1_Reverse();
    vTaskDelay(200);
    LED1_Reverse();
    vTaskDelay(800);
  }
}

//浮点测试任务
void float_task(void *pvParameters)
{
  static float float_num = 0.00;
  while (1)
  {
    float_num += 0.01f;
    if (float_num > 10.00)
    {
      float_num = 0.00;
    }
    printf("float_num的值为: %.4f\n", float_num);
    vTaskDelay(1000);
  }
}

#ifdef Debug
/**
 * @brief   栈溢出钩子函数
 * @param   xTask
 * @arg		  the task that just exceeded its stack boundaries.
 * @param   pcTaskName
 * @arg		  A character string containing the name of the offending task.
 * @note    FreeRTOS打印栈溢出的任务，关联#define configCHECK_FOR_STACK_OVERFLOW 2，在FreeRTOSConfig.h下
 * @retval  void
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
  /* Run time stack overflow checking is performed if
  configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
  called if a stack overflow is detected. */
  printf("任务：%s 溢出\r\n", pcTaskName);
  while (1)
    ;
}
#endif

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
