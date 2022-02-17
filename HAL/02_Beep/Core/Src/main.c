/**
 * @file	02_Beep
 * @brief 	蜂鸣器响300ms，同时黄灯亮，蜂鸣器不响300ms同时红灯亮
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2022年2月17号22点30分
 */

#include "stm32f1xx.h"

void Beep_Init(void);
void LED_Init(void);

/**
 * @brief	利用HAL库函数进行Beep初始化
 * @param 	none
 * @arg		none
 * @note  	先开启GPIO时钟，再利用HAL_GPIO_Init();函数进行管脚初始化，再设置默认初始化后不响
 * @retval	none
 */
void Beep_Init(void)
{
	GPIO_InitTypeDef GPIO_InitTure;

	__HAL_RCC_GPIOB_CLK_ENABLE(); // 开启GPIOB时钟
	__HAL_RCC_GPIOE_CLK_ENABLE(); // 开启GPIOE时钟
	/*ARM的芯片都是这样，外设通常都是给了时钟后，才能设置它的寄存器,这么做的目的是为了省电，使用了所谓时钟门控的技术。*/

	/*进行结构体内的参数配置，先找到下面HAL_GPIO_Init();的定义处，再对定义处的函数详细找参数*/
	GPIO_InitTure.Mode = GPIO_MODE_OUTPUT_PP;	// 推挽输出
	GPIO_InitTure.Pull = GPIO_PULLUP;			// 上拉
	GPIO_InitTure.Speed = GPIO_SPEED_FREQ_HIGH; // 高速
	GPIO_InitTure.Pin = GPIO_PIN_8;				// 设置GPIOx的5口

	HAL_GPIO_Init(GPIOB, &GPIO_InitTure); // 先在上面四行设置GPIO的模式，上下拉，速度，再对GPIOB管脚初始化

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); // PB8置0，默认初始化后不响
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

	__HAL_RCC_GPIOB_CLK_ENABLE(); // 开启GPIOB时钟
	__HAL_RCC_GPIOE_CLK_ENABLE(); // 开启GPIOE时钟
	/*ARM的芯片都是这样，外设通常都是给了时钟后，才能设置它的寄存器,这么做的目的是为了省电，使用了所谓时钟门控的技术。*/

	/*进行结构体内的参数配置，先找到下面HAL_GPIO_Init();的定义处，再对定义处的函数详细找参数*/
	GPIO_InitTure.Mode = GPIO_MODE_OUTPUT_PP;	// 推挽输出
	GPIO_InitTure.Pull = GPIO_PULLUP;			// 上拉
	GPIO_InitTure.Speed = GPIO_SPEED_FREQ_HIGH; // 高速
	GPIO_InitTure.Pin = GPIO_PIN_5;				// 设置GPIOx的5口

	HAL_GPIO_Init(GPIOB, &GPIO_InitTure); // 先在上面四行设置GPIO的模式，上下拉，速度，再对GPIOB管脚初始化
	HAL_GPIO_Init(GPIOE, &GPIO_InitTure); // GPIOE管脚初始化

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); // PB5置1，默认初始化后灯灭
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET); // PE5置1，默认初始化后灯灭
}

/**
 * @brief	对函数简要描述
 * @param 	参数说明，以’:’作为参数结束标志；
 * @arg		参数里面可选择参量列举，对于可数情况可进行参量列举，同样以’:’作为参数结束标志；
 * @note  	注释，配合brief一起使用可以达到很好的注释效果；
 * @retval	返回值说明。
 */
int main(void)
{
	HAL_Init();	 // 初始化HAL库
	Beep_Init(); // 初始化Beep
	LED_Init();	 // 初始化LED

	while (1)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);	  // PB8置1，响
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); // PB5置0，灯亮
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);	  // PE5置1，灯灭
		HAL_Delay(299);										  // 延迟300ms
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); // PB8置0，不响
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);	  // PB5置1，灯灭
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET); // PE5置0，灯亮
		HAL_Delay(299);										  // 延迟300ms
	}
}