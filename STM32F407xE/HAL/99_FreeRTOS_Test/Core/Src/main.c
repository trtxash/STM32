/**
 * @file	12_FreeRTOS_WS2812
 * @brief 	移植FreeRTOS和WS2812
 * @author 	TRTX-gamer      https://github.com/TRTX-gamer；
 *          突然吐血    https://space.bilibili.com/12890038;
 * @version 1.00
 * @date 	2022年11月8号21点46分
 */
#include "main.h"

u16 Buf[NUM];

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
	ws2812init();					   // 初始化ws2812，用了tim4

	Buf[NUM - 1] = 0;

	for (i = 0; i < NUM; i++)
	{
		Buf[i] = 7 * (i + 1);
	}

	while (1)
	{
		HAL_TIM_PWM_Start_DMA(&WS2812_PWM_TIM_Hand, WS2812_PWM_TIM_Ch, (u32 *)Buf, NUM);
		HAL_Delay(10);
		HAL_TIM_PWM_Stop_DMA(&WS2812_PWM_TIM_Hand, WS2812_PWM_TIM_Ch);
		HAL_Delay(10);
	}
}
