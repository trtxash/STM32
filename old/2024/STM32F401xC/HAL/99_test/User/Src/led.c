#include "led.h"

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
 * @brief	LED灯交替闪烁
 * @param 	none
 * @arg		none
 * @note  	初始化函数后利用HAL_GPIO_xxxx以达到LED灯状态取反
 * @retval	void
 */
void LED_Reverse(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)); // GPIOB13取反
}