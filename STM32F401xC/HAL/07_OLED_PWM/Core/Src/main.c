/**
 * @file	07_OLED_PWM
 * @brief 	OLED显示管脚的PWM频率
 *          GND    电源地
 *          VCC  接5V或3.3v电源
 *          D0   接（SCL）,可在oled.h里更改
 *          D1   接（SDA）,可在oled.h里更改
 *          RES  PD2（SPI模块改成IIC模块需要接此引脚，IIC模块用户请忽略）
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2022年7月14号15点23分
 */

#include "delay.h"
#include "sys.h"
#include "stm32f4xx.h"
#include "timer.h"
#include "oled.h"
#include "led.h"
#include "key.h"
#include "motorencoder.h"

/**
 * @brief	主函数,程序入口
 * @param 	none
 * @arg		none
 * @note  	初始化函数后利用定时器5PWM发生器实现4路PWM发生器
 * @retval	int
 */
int main(void)
{
	u16 pwmval = 500;
	u16 arr = pwmval - 1;
	u16 psc = 84 - 1;
	u32 freq = 84 / (psc + 1) / (arr + 1) * 1000;

	HAL_Init();
	Stm32_Clock_Init(168, 25, 2, 4);
	delay_init(84);					 //初始化延时函数
	LED_Init();						 //初始化LED
	KEY_Init();						 //初始化按键
	TIM5_PWM_Init(arr, psc, 0B1111); // 2kHz，50%，4路,84M/84=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ
	OLED_Init();
	OLED_DisplayInit(); // OLED初始化显示
	while (1)
	{
		if (KEY_Scan(0))
		{
			delay_ms(20); // 消抖
			LED_Reverse();
			if (pwmval < 50)
			{
				pwmval = 50; // pwmval不能小于50
			}
			pwmval -= 50;				  // pwmval递减
			TIM_SetTIM5Compare_3(pwmval); //修改比较值，修改占空比
		}
		Encoder = Read_Encoder(3); //读取编码器的值
		OLED_ShowNum(32, 32, Encoder, 4, 16, 1);
	}
}