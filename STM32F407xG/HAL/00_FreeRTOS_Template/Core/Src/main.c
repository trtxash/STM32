/**
 * @file	12_FreeRTOS_WS2812
 * @brief 	移植FreeRTOS和WS2812
 * @author 	TRTX-gamer      https://github.com/TRTX-gamer；
 *          突然吐血    https://space.bilibili.com/12890038;
 * @version 1.00
 * @date 	2022年11月8号21点46分
 */
#include "main.h"

#define NUM 21
uint16_t send_Buf[NUM] = {0};

/**
 * @brief   主函数,程序入口
 * @param   none
 * @arg		none
 * @note    ADC,OLED
 * @retval  int
 */
int main(void)
{
	u8 i;

	if (HAL_Init()) // 初始化HAL库
	{
		Error_Handler();
	}
	Stm32_Clock_Init(168, 4U, 2U, 4U); // 初始化时钟
	delay_init(168);				   // 初始化延时函数
	MX_DMA_Init();					   // 要先初始化DMA
	MX_TIM4_Init();

	send_Buf[NUM - 1] = 0;

	for (i = 0; i < NUM; i++)
	{
		send_Buf[i] = 7 * (i + 1);
	}

	HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_1, (u32 *)send_Buf, NUM);
	while (1)
	{
	}
}
