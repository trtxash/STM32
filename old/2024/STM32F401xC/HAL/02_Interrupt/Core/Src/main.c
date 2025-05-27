/**
 * @file	00_Template
 * @brief 	模板
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2022年7月12号14点04分
 */

#include "delay.h"
#include "sys.h"
#include "led.h"
// #include "key.h"
#include "exti.h"
#include "stm32f4xx.h"

/**
 * @brief	LED灯交替闪烁
 * @param 	none
 * @arg		none
 * @note  	初始化函数后利用LED_Reverse以达到LED灯交替闪烁
 * @retval	int
 */
int main(void)
{
	HAL_Init(); //初始化HAL库
	Stm32_Clock_Init(168, 25, 2, 4);
	delay_init(84); //初始化延时函数
	LED_Init();		//初始化LED
	// KEY_Init();		//初始化按键
	EXTI_Init(); //初始化外部中断

	while (1)
	{
		// if (KEY_Scan(0))
		// {
		// 	LED_Reverse();
		// }
	}
}