/**
 * @file	09_ADC+OLED+DMA
 * @brief 	ADC采样，OLED显示，DMA发送
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2022年7月21号18点53分
 */

#include "main.h"

/**
 * ADC采样注意参考电压VERF+接口，如果悬空，则一直为4095
 */

/**
 * @brief	主函数,程序入口
 * @param 	none
 * @arg		none
 * @note
 * @retval	int
 */
int main(void)
{
	if (HAL_Init()) // 初始化HAL库
	{
		Error_Handler();
	}
	Stm32_Clock_Init(168U, 25U, 2U, 4U); // 初始化时钟
	delay_init(84);						 // 初始化延时函数
	LED_Init();							 // 初始化LED
	// MX_DMA_Init(); // 要先初始化DMA
	// MX_I2C1_Init();                     // 初始化i2c接口
	// MX_SPI1_Init();	   // 初始化MDA后再初始话SPI
	// MX_ADC1_Init();	   // 初始化ADC1
	// OLED_Init();	   // 初始化OLED
	// uart_init(115200); // 初始化串口
	// TIM3_Init(202 - 1, 840 - 1);
	// TIM4_Init(10000 - 1, 8400 - 1); // 定时器3初始化，周期1s

	// printf("Inie OK!\n");

	// u8 temp[10] = {0};
	// HAL_ADC_Start_DMA(&hadc1, (u32 *)&adcx, 1); // 启动ADC+DMA
	while (1)
	{
		// printf("value=%d,%f\n", adcx, value);
		// sprintf(temp, "%f", value);
		// OLED_ShowString(64, 0, temp, 16, 1);
		delay_ms(500);
		LED_Reverse();
	}
}