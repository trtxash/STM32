/**
 * @file	02_520直流电机
 * @brief 	LED灯交替闪烁
 * 			锻炼使用HAL库，对工程模板有更好的理解
 * 			为学寄存器版本打基础
 * 			建立一个高效的学习模板
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2022年7月11号19点1分
 */

#include "delay.h"
#include "sys.h"
#include "stm32f4xx.h"

void LED_Init(void);
void LED_Fleg(void);
void TIM3_PWM_Init(void)
{
	HAL_TIM_Base_Start_IT(&htim);
}

/**
 * @brief	LED灯交替闪烁
 * @param 	none
 * @arg		none
 * @note  	初始化函数后利用HAL_GPIO_WritePin和HAL_Delay进行控制LED的亮灭，指示灯，取反
 * @retval	int
 */
int main(void)
{
	HAL_Init(); //初始化HAL库
	Stm32_Clock_Init(25, 168, 2, 4);
	delay_init(84); //初始化延时函数
	LED_Init();		//初始化LED

	while (1)
	{
		LED_Fleg();
		delay_ms(500); //延时500ms
	}
}

/**
 * @brief	利用HAL库函数进行LED初始化
 * @param 	none
 * @arg		none
 * @note  	先开启GPIO时钟，再利用HAL_GPIO_Init();函数进行管脚初始化，再设置默认初始化后灯灭
 * @retval	none
 */
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitTure;

	__HAL_RCC_GPIOC_CLK_ENABLE(); // 开启GPIOB时钟
	/*ARM的芯片都是这样，外设通常都是给了时钟后，才能设置它的寄存器,这么做的目的是为了省电，使用了所谓时钟门控的技术。*/

	/*进行结构体内的参数配置，先找到下面HAL_GPIO_Init();的定义处，再对定义处的函数详细找参数*/
	GPIO_InitTure.Mode = GPIO_MODE_OUTPUT_PP;	// 推挽输出
	GPIO_InitTure.Pull = GPIO_PULLUP;			// 上拉
	GPIO_InitTure.Speed = GPIO_SPEED_FREQ_HIGH; // 高速
	GPIO_InitTure.Pin = GPIO_PIN_13;			// 设置GPIOx的5口

	HAL_GPIO_Init(GPIOC, &GPIO_InitTure); // 先在上面四行设置GPIO的模式，上下拉，速度，再对GPIOB管脚初始化

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // PB5置1，默认初始化后灯灭
}

/**
 * @brief	LED灯指示
 * @param 	none
 * @arg		none
 * @note  	初始化函数后利用HAL_GPIO_WritePin和HAL_Delay进行控制LED的亮灭，指示灯，取反
 * @retval	int
 */
void LED_Fleg(void)
{

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)); // GPIOB13取反
}