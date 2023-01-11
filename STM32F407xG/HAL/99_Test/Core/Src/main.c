/**
 * @file	99_Test
 * @brief 	FreeRTOS，Test
 * @note  	Printf函数对应UART1，Vlauepack对应UART6
 * 			FreeRTOS任务计时用了Tim11，USMART用了TIM13，定时任务用了TIM14
 * @author 	TRTX-gamer      https://github.com/TRTX-gamer；
 *          突然吐血    https://space.bilibili.com/12890038;
 * @version 1.00
 * @date 	2023年1月11号18点41分
 */
#include "main.h"

#define Debug 1 // 控制Debug的一些相关函数

#define BOUND 115200 // 串口波特率

#define START_TASK_PRIO 1			 // 任务优先级
#define START_STK_SIZE 128			 // 任务堆栈大小
TaskHandle_t StartTask_Handler;		 // 任务句柄
void start_task(void *pvParameters); // 任务函数

#define TEST_TASK_PRIO 3			// 任务优先级
#define TEST_STK_SIZE 512			// 任务堆栈大小
TaskHandle_t TESTTask_Handler;		// 任务句柄
void test_task(void *pvParameters); // 任务函数

#define LED_TASK_PRIO 4			   // 任务优先级
#define LED_STK_SIZE 32			   // 任务堆栈大小
TaskHandle_t LEDTask_Handler;	   // 任务句柄
void led_task(void *pvParameters); // 任务函数

extern uint32_t SystemCoreClock;

/**
 * @brief   主函数,程序入口
 * @param   none
 * @arg		none
 * @note
 * @retval  int
 */
int main(void)
{
	if (HAL_Init()) // 初始化HAL库
	{
		Error_Handler();
	}
	Stm32_Clock_Init(240, 4U, 2U, 4U);	   // 初始化时钟
	MX_DMA_Init();						   // 要先初始化DMA
	delay_init(SystemCoreClock / 1000000); // 初始化延时函数
	uart_init(BOUND);					   // 初始化串口

	printf("\r\n初始化中...\r\n");

	initValuePack(BOUND);						// Valuepack初始化，用了uart6+DMA
	usmart_dev.init(SystemCoreClock / 1000000); // 初始化USMART，用了tim13,100ms定时，0.1ms计数时间
	ConfigureTimerForTask();					// 定时任务，定时器14初始化，周期1ms
	LED_Init();

	printf("\r\n初始化完成\r\n");

	// 创建开始任务
	xTaskCreate((TaskFunction_t)start_task,			 // 任务函数
				(const char *)"start_task",			 // 任务名称
				(uint16_t)START_STK_SIZE,			 // 任务堆栈大小
				(void *)NULL,						 // 传递给任务函数的参数
				(UBaseType_t)START_TASK_PRIO,		 // 任务优先级
				(TaskHandle_t *)&StartTask_Handler); // 任务句柄
	vTaskStartScheduler();
}

// 开始任务任务函数
void start_task(void *pvParameters)
{
	taskENTER_CRITICAL(); // 进入临界区
	// 创建led任务
	xTaskCreate((TaskFunction_t)led_task,
				(const char *)"led_task",
				(uint16_t)LED_STK_SIZE,
				(void *)NULL,
				(UBaseType_t)LED_TASK_PRIO,
				(TaskHandle_t *)&LEDTask_Handler);
	// 创建test任务
	xTaskCreate((TaskFunction_t)test_task,
				(const char *)"test_task",
				(uint16_t)TEST_STK_SIZE,
				(void *)NULL,
				(UBaseType_t)TEST_TASK_PRIO,
				(TaskHandle_t *)&TESTTask_Handler);
	vTaskDelete(StartTask_Handler); // 删除开始任务
	taskEXIT_CRITICAL();			// 退出临界区
}

// LED任务函数
void led_task(void *pvParameters)
{
	while (1)
	{
		LED0_Reverse();
		vTaskDelay(500);
	}
}

// 测试任务函数
void test_task(void *pvParameters)
{
	txvaluepack.bools[0] = 1;
	txvaluepack.bools[1] = 0;
	txvaluepack.bools[2] = 1;
	txvaluepack.integers[0] = 666;
	txvaluepack.integers[1] = 999;

	while (1)
	{
		sendValuePack(&txvaluepack);
		printf("RX:%d,%d,%d,%d,%d\r\n", rxvaluepack.bools[0], rxvaluepack.bools[1], rxvaluepack.bools[2], rxvaluepack.integers[0], rxvaluepack.integers[1]);
		vTaskDelay(5000);
	}
}

#if Debug == 1
/**
 * @brief   栈溢出钩子函数
 * @param   xTask
 * @arg		the task that just exceeded its stack boundaries.
 * @param   pcTaskName
 * @arg		A character string containing the name of the offending task.
 * @note    FreeRTOS打印栈溢出的任务，关联#define configCHECK_FOR_STACK_OVERFLOW 2，在FreeRTOSConfig.h下
 * @retval  void
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
	called if a stack overflow is detected. */
	printf("任务：%s 溢出\r\n", pcTaskName);
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