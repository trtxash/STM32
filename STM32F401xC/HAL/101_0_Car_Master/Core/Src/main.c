/**
 * @file	07_Bluetooth_HC_05
 * @brief 	蓝牙HC-05模块驱动，蓝牙左右电机驱动
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2022年7月21号18点53分
 */

#include "main.h"

#define USART_REC_LEN_MAIN 12 //定义最大接收字节数 12

u8 usart_tr[USART_REC_LEN_MAIN] = {0XA5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0X5A};
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
	OLED_Init();					 // OLED初始化
	SMBus_Init();					 // SMBus初始化
									 // TIM1_Init(10000 - 1, 84 - 1); // 100Hz
									 // TIM2_Init(10000 - 1, 84 - 1);	 // 100Hz刷新OLED
	TIM4_PWM_Init(arr, psc, 0B1111); // 2kHz，50%，4路,84M/84=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ
	TIM5_PWM_Init(arr, psc, 0B1111); // 2kHz，50%，4路,84M/84=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ
	TIM_SetTIM4Compare_n(250, 2);	 
	// Encoder_Init(); // 初始化电机编码器
	// OLED_Display(); //显示初始化信息
	while (1)
	{
		tem = SMBus_ReadTemp();
	}
}