/**
 * @file	07_FreeRTOS_W25QXXX
 * @brief 	移植FreeRTOS和W25QXXX
 * @author 	TRTX-gamer      https://github.com/TRTX-gamer；
 *          突然吐血    https://space.bilibili.com/12890038;
 * @version 1.02
 * @date 	2022年11月1号1点30分
 */
#include "main.h"

#define Debug 1 // 控制Debug的一些相关函数

#define START_TASK_PRIO 1			 // 任务优先级
#define START_STK_SIZE 128			 // 任务堆栈大小
TaskHandle_t StartTask_Handler;		 // 任务句柄
void start_task(void *pvParameters); // 任务函数

#define LED0_TASK_PRIO 2			// 任务优先级
#define LED0_STK_SIZE 50			// 任务堆栈大小
TaskHandle_t LED0Task_Handler;		// 任务句柄
void led0_task(void *pvParameters); // 任务函数

#define LED1_TASK_PRIO 3			// 任务优先级
#define LED1_STK_SIZE 50			// 任务堆栈大小
TaskHandle_t LED1Task_Handler;		// 任务句柄
void led1_task(void *pvParameters); // 任务函数

#define ADC1_TASK_PRIO 4			// 任务优先级
#define ADC1_STK_SIZE 256			// 任务堆栈大小
TaskHandle_t ADC1Task_Handler;		// 任务句柄
void adc1_task(void *pvParameters); // 任务函数

#define Speech_TASK_PRIO 4			  // 任务优先级
#define Speech_STK_SIZE 256			  // 任务堆栈大小
TaskHandle_t SpeechTask_Handler;	  // 任务句柄
void Speech_task(void *pvParameters); // 任务函数

/**
 * @brief   主函数,程序入口
 * @param   none
 * @arg		  none
 * @note    ADC,OLED
 * @retval  int
 */
int main(void)
{
	if (HAL_Init()) // 初始化HAL库
	{
		Error_Handler();
	}
	Stm32_Clock_Init(168U, 4U, 2U, 4U); // 初始化时钟
	delay_init(168);					// 初始化延时函数
	uart_init(115200);					// 初始化串口
	LED_Init();							// 初始化LED
	MX_DMA_Init();						// 要先初始化DMA
	// MX_I2C1_Init();                     // 初始化i2c接口
	xfs5152Drv_Init();		 // 初始化xfs5152
	MX_SPI1_Init();			 // 初始化MDA后再初始话SPI
	if (W25QXX_Init() == -1) // 初始化w25qxx，里面初始化了spi3
		printf(" W25QXX_Init error!\r\n");
	else
	{
		printf(" W25QXX_Init ok!\r\n");
		printf("W25QXX_TYPE:%x\r\n", W25QXX_TYPE);
		printf("W25QXX_SIZE:%x\r\n", W25QXX_SIZE);
		for (u8 i = 0; i < 8; i++)
		{
			printf("W25QXX_UID[%d]:%x\r\n", i, W25QXX_UID[i]);
		}
	}
	// MX_ADC1_Init();                     // 初始化ADC1
	OLED_Init(); // 初始化OLED
	// TIM3_Init(202 - 1, 840 - 1);
	TIM4_Init(10000 - 1, 8400 - 1); // 定时器3初始化，周期1s

	printf(" Init OK!\r\n");

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
	// 创建LED0任务
	xTaskCreate((TaskFunction_t)led0_task,
				(const char *)"led0_task",
				(uint16_t)LED0_STK_SIZE,
				(void *)NULL,
				(UBaseType_t)LED0_TASK_PRIO,
				(TaskHandle_t *)&LED0Task_Handler);
	// 创建LED1任务
	xTaskCreate((TaskFunction_t)led1_task,
				(const char *)"led1_task",
				(uint16_t)LED1_STK_SIZE,
				(void *)NULL,
				(UBaseType_t)LED1_TASK_PRIO,
				(TaskHandle_t *)&LED1Task_Handler);
	// 创建ADC1任务
	// xTaskCreate((TaskFunction_t)adc1_task,
	//             (const char *)"adc1_task",
	//             (uint16_t)ADC1_STK_SIZE,
	//             (void *)NULL,
	//             (UBaseType_t)ADC1_TASK_PRIO,
	//             (TaskHandle_t *)&ADC1Task_Handler);
	// 创建Speech任务
	// xTaskCreate((TaskFunction_t)Speech_task,
	//             (const char *)"Speech_task",
	//             (uint16_t)Speech_STK_SIZE,
	//             (void *)NULL,
	//             (UBaseType_t)Speech_TASK_PRIO,
	//             (TaskHandle_t *)&SpeechTask_Handler);
	vTaskDelete(StartTask_Handler); // 删除开始任务
	taskEXIT_CRITICAL();			// 退出临界区
}

// LED0任务函数
void led0_task(void *pvParameters)
{
	while (1)
	{
		LED0_Reverse();
		vTaskDelay(1000);
	}
}

// LED1任务函数
void led1_task(void *pvParameters)
{
	while (1)
	{
		LED1_Reverse();
		vTaskDelay(50);
	}
}

// ADC1任务函数
void adc1_task(void *pvParameters)
{
	u8 temp[10] = {0};
	// sprintf(temp, "%s", "你好");
	// OLED_ShowString(64, 0, temp, 16, 1);

	// HAL_ADC_Start_DMA(&hadc1, (u32 *)&adcx, 1); // 启动ADC+DMA

	while (1)
	{
		// printf("value=%d,%f\n", adcx, value);
		// sprintf(temp, "%f", value);
		// OLED_ShowString(64, 0, temp, 16, 1);
		vTaskDelay(5);
	}
}

// Speech任务函数
void Speech_task(void *pvParameters)
{
	u8 i = 0;

	while (1)
	{
		speech_text("[v10]", 0);
		vTaskDelay(500);

		if (i)
		{
			speech_text("[m3]", 0);
			vTaskDelay(500);
			speech_text_utf8("切换女声");
			vTaskDelay(3000);
		}
		else
		{
			speech_text("[m51]", 0);
			vTaskDelay(500);
			speech_text_utf8("切换男声");
			vTaskDelay(3000);
		}

		i = !i;

		speech_text_utf8("大声哒咩哒咩哒咩");
		vTaskDelay(3000);
		speech_text("123ABC", 0);
		vTaskDelay(3000);
		speech_text("[v3]", 0);
		vTaskDelay(500);
		speech_text_utf8("小声哒咩哒咩哒咩");
		vTaskDelay(3000);
		speech_text("123ABC", 0);
		vTaskDelay(3000);
	}
}

#if Debug == 1
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
