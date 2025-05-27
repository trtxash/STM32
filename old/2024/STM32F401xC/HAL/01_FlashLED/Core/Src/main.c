/**
 * @file	01_FlashLED
 * @brief 	LED灯交替闪烁
 * 			锻炼使用HAL库，对工程模板有更好的理解
 * 			为学寄存器版本打基础
 * 			建立一个高效的学习模板
 * @author 	TRTX-gamer
 * @version 1.01
 * @date 	2022年2月17号18点50分
 */

#include "delay.h"
#include "sys.h"
#include "led.h"
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

	while (1)
	{
		LED_Reverse();
		delay_ms(500); //延时500ms
	}
}