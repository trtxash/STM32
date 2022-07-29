/**
 * @file	07_Bluetooth_HC_05
 * @brief 	蓝牙HC-05模块驱动，蓝牙左右电机驱动
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2022年7月21号18点53分
 */
#include "main.h"

u8 power = 93;	 //电池电量
long pin_a3 = 0; // A3引脚电压
long pin_a4 = 0; // A4引脚电压
long pin_b5 = 0; // B5引脚电压
long pin_b6 = 0; // B6引脚电压
long pin_sum = 0;

/**
 * @brief	主函数,程序入口
 * @param 	none
 * @arg		none
 * @note  	初始化函数后利用定时器5PWM发生器实现4路PWM发生器
 * @retval	int
 */
int main(void)
{
	// u16 arr = 500 - 1;
	// u16 psc = 84 - 1;

	HAL_Init();
	Stm32_Clock_Init(168, 25, 2, 4);
	uart_init(115200); // 串口1初始化，作为从机，数据采集端
	delay_init(84);	   //初始化延时函数
	// Beep_Init();					 //初始化蜂鸣器
	Infraredtobe_Init(); //初始化红外检测模块
	// OLED_Init();					 // OLED初始化
	TIM1_Init(5000 - 1, 84 - 1); // 5ms
	// TIM2_Init(10000 - 1, 84 - 1);	 // 100Hz刷新OLED
	// TIM5_PWM_Init(arr, psc, 0B1111); // 2kHz，50%，4路,84M/84=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ
	Encoder_Init(); // 初始化电机编码器
	// OLED_Display();					 //显示初始化信息
	while (1)
	{
		// pin_sum++;
		// if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3))
		// {
		// 	pin_a3++;
		// }
		// else
		// {
		// 	pin_a3--;
		// }
		// if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4))
		// {
		// 	pin_a4++;
		// }
		// else
		// {
		// 	pin_a4--;
		// }
		// if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5))
		// {
		// 	pin_b5++;
		// }
		// else
		// {
		// 	pin_b5--;
		// }
		// if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6))
		// {
		// 	pin_b6++;
		// }
		// else
		// {
		// 	pin_b6--;
		// }
	}
}