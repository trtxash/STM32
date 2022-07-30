/**
 * @file	07_Bluetooth_HC_05
 * @brief 	蓝牙HC-05模块驱动，蓝牙左右电机驱动
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2022年7月21号18点53分
 */

#include "main.h"

float tem;

/**
 * @brief	主函数,程序入口
 * @param 	none
 * @arg		none
 * @note  	初始化函数后利用定时器5PWM发生器实现4路PWM发生器
 * @retval	int
 */
int main(void)
{
	u16 arr = 500 - 1;
	u16 psc = 84 - 1;

	HAL_Init();
	Stm32_Clock_Init(168, 25, 2, 4);
	uart_init(115200); // 串口1初始化，作为从机，数据采集端
	delay_init(84);	   //初始化延时函数
	// Beep_Init();					 //初始化蜂鸣器
	// Infraredtobe_Init();			 //初始化红外检测模块
	OLED_Init();				 // OLED初始化
	SMBus_Init();				 // SMBus初始化
	TIM1_Init(5000 - 1, 84 - 1); // 200HZ,5ms
								 // TIM2_Init(10000 - 1, 84 - 1);	 // 100Hz刷新OLED
	/* TIM4，ch1右上反转，ch2右上正转，ch3左上反转，ch4左上正转 */
	TIM4_PWM_Init(arr, psc, 0B1111); // 2kHz，50%，4路,84M/84=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ
	/* TIM5，ch1左下正转，ch2左下反转，ch3右下反转，ch4右下正转 */
	TIM5_PWM_Init(arr, psc, 0B1111); // 2kHz，50%，4路,84M/84=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ
	// TIM_SetTIM4Compare_n(300, 2);
	// TIM_SetTIM4Compare_n(300, 4);
	// TIM_SetTIM5Compare_n(300, 1);
	// TIM_SetTIM5Compare_n(300, 3);
	// Encoder_Init(); // 初始化电机编码器
	// OLED_Display(); //显示初始化信息
	while (1)
	{
		tem = SMBus_ReadTemp();
		main_plan();
	}
}