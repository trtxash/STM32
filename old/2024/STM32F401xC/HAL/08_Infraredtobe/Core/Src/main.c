/**
 * @file	08_Inheritance
 * @brief 	在遥控小车的基础上加入红外传感，实现循迹
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2022年7月25号13点35分
 */

#include "main.h"

/**
 * @brief	主函数,程序入口
 * @param 	none
 * @arg		none
 * @note  	超多，不写
 * @retval	int
 */
int main(void)
{
	u16 arr = 500 - 1;
	u16 psc = 84 - 1;

	HAL_Init();
	Stm32_Clock_Init(168, 25, 2, 4);
	uart6_init(115200);
	delay_init(84);					 //初始化延时函数
	Beep_Init();					 //初始化蜂鸣器
	Infraredtobe_Init();			 //初始化红外检测模块
	OLED_Init();					 // OLED初始化
	TIM2_Init(10000 - 1, 84 - 1);	 // 100Hz刷新OLED
	TIM5_PWM_Init(arr, psc, 0B1111); // 2kHz，50%，4路,84M/84=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ
	Encoder_Init();					 // 初始化电机编码器
	OLED_Display();					 //显示初始化信息
	while (1)
	{
		if (beep)
		{
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, beep_flag);
			beep_flag = !beep_flag;
			delay_us(400);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, 1);
		}
	}
}