/**
 * @file	99_Test
 * @brief 	FreeRTOS，Test
 * @note  	Printf函数对应UART1，Vlauepack对应UART6
 * 			USMART用了TIM6，定时任务用了TIM7,FreeRTOS任务计时用了Tim14
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
	u8 temp[21] = {0};

	if (HAL_Init()) // 初始化HAL库
	{
		Error_Handler();
	}
	Stm32_Clock_Init(240, 4U, 2U, 4U);	   // 初始化时钟
	MX_DMA_Init();						   // 要先初始化DMA
	delay_init(SystemCoreClock / 1000000); // 初始化延时函数
	uart_init(BOUND);					   // 初始化串口

	printf("\r\n初始化中...\r\n");
	// MX_I2C1_Init();
	usmart_dev.init(SystemCoreClock / 1000000); // 初始化USMART，用了tim6,100ms定时，0.1ms计数时间
	initValuePack(BOUND);						// Valuepack初始化，用了uart6+DMA
	Tim_Encoder_Init();
	Tim_Motor_Init();
	LED0_Init();
	LED1_Init();
	KEY0_Init();
	OLED_Init();
	HC_05_init();
	Tim_ConfigureTimerForTask(); // 定时任务，tim7初始化，周期1ms,最后初始化
	printf("\r\n初始化完成\r\n");
	sprintf(temp, "OK!");
	OLED_ShowString(0, 0, temp, 8, 1, WHITE);

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
	u8 i = 0;
	while (1)
	{
		i++;
		LED0_Reverse();
		if (i >= 2)
		{
			i = 0;
			LED1_Reverse();
		}
		vTaskDelay(500);
	}
}

// 测试任务函数
void test_task(void *pvParameters)
{
	u8 s = 0;
	u8 j;
	u16 i = 600;
	u8 temp[21] = {0};

	while (1)
	{
		if (HC_05_READ_STATE())
			OLED_ShowString(64, 0, "Bl OK!", 8, 1, WHITE);
		else
			OLED_ShowString(64, 0, "Bl NO!", 8, 1, WHITE);

		for (j = 0; j < 4; j++)
		{
			if (Encoder[j] < 0)
				sprintf(temp, "-%5d", -Encoder[j]);
			else
				sprintf(temp, "+%5d", Encoder[j]);
			OLED_ShowString(0, 8 * (j + 1), temp, 8, 1, WHITE);

			sprintf(temp, "%5d", Encoder_target[j]);
			OLED_ShowString(64, 8 * (j + 1), temp, 8, 1, WHITE);
		}

		if (rxvaluepack.bools[0])
		{
			mecanum_wheel_xyz_set(); // 5ms计算一次麦轮运动值
			for (j = 0; j < 4; j++)
			{
				PID_V[0][j] = rxvaluepack.floats[0];
				PID_V[1][j] = rxvaluepack.floats[1];
				PID_V[2][j] = rxvaluepack.floats[2];
			}
		}
		else
		{
			mecanum_wheel_stop();
		}

		if (KEY0_READ())
		{

			// switch (s)
			// {
			// case 0:
			// 	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, i - 1);
			// 	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, i - 1);
			// 	// __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, i - 1);
			// 	__HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, i - 1);
			// 	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, i - 1);
			// 	i--;
			// 	break;
			// case 1:
			// 	++i;
			// 	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, i - 1);
			// 	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, i - 1);
			// 	// __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, i - 1);
			// 	__HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, i - 1);
			// 	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, i - 1);
			// 	break;
			// case 2:
			// 	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, i - 1);
			// 	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, i - 1);
			// 	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, i - 1);
			// 	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, i - 1);
			// 	i--;
			// 	break;
			// case 3:
			// 	++i;
			// 	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, i - 1);
			// 	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, i - 1);
			// 	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, i - 1);
			// 	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, i - 1);
			// 	break;
			// default:
			// 	s = 0;
			// 	break;
			// }

			// if (i == 0 || i == 600)
			// {
			// 	s++;
			// 	if (s > 3)
			// 		s = 0;
			// }
		}
		else
		{
			// s = 0;
			// i = 600;
			// __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1 - 1);
			// __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 600 - 1);
			// __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 1 - 1);
			// __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 600 - 1);
			// // __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 1 - 1);
			// __HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, 1 - 1);
			// __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, 600 - 1);
			// __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 1 - 1);
			// __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, 600 - 1);
		}

		vTaskDelay(5);
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