/**
 * @file	03_Timer
 * @brief 	定时器中断
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2022年7月12号19点42分
 */

#include "delay.h"
#include "sys.h"
#include "led.h"
#include "timer.h"
#include "stm32f4xx.h"

static u32 time_frequence = 0;
static u32 time_arr = 0;
static u32 time_psc = 0;

/**
 * @brief	LED灯交替闪烁
 * @param 	none
 * @arg		none
 * @note  	初始化函数后利用L定时器中断以达到LED灯交替闪烁
 * @retval	int
 */
int main(void)
{
	HAL_Init(); //初始化HAL库
	Stm32_Clock_Init(168, 25, 2, 4);
	delay_init(84);			 //初始化延时函数
	LED_Init();				 //初始化LED
	TIM3_Init(50000 - 1, 1 - 1); //定时器3初始化，周期为500ms

	while (1)
	{
	}
}