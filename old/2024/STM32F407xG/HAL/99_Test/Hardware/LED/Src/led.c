#include "led.h"

/**
 * @brief	利用HAL库函数进行LED初始化
 * @param 	none
 * @arg		none
 * @note  	先开启GPIO时钟，再利用HAL_GPIO_Init();函数进行管脚初始化，再设置默认初始化后灯灭
 * @retval	none
 */
void LED0_Init(void)
{
	GPIO_InitTypeDef GPIO_InitTure;

	LED0_GPIO_CLK_ENABLE(); // 开启GPIO时钟
	/*ARM的芯片都是这样，外设通常都是给了时钟后，才能设置它的寄存器,这么做的目的是为了省电，使用了所谓时钟门控的技术。*/

	/*进行结构体内的参数配置，先找到下面HAL_GPIO_Init();的定义处，再对定义处的函数详细找参数*/
	GPIO_InitTure.Mode = GPIO_MODE_OUTPUT_PP;	// 推挽输出
	GPIO_InitTure.Pull = GPIO_PULLUP;			// 上拉
	GPIO_InitTure.Speed = GPIO_SPEED_FREQ_HIGH; // 高速
	GPIO_InitTure.Pin = LED0_PIN;				// 设置GPIOx的5口
	HAL_GPIO_Init(LED0_GPIO, &GPIO_InitTure);	// 先在上面四行设置GPIO的模式，上下拉，速度，再对GPIOB管脚初始化

	HAL_GPIO_WritePin(LED0_GPIO, LED0_PIN, GPIO_PIN_SET); // PB5置1，默认初始化后灯灭
}

/**
 * @brief	利用HAL库函数进行LED初始化
 * @param 	none
 * @arg		none
 * @note  	先开启GPIO时钟，再利用HAL_GPIO_Init();函数进行管脚初始化，再设置默认初始化后灯灭
 * @retval	none
 */
void LED1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitTure;

	LED1_GPIO_CLK_ENABLE();

	GPIO_InitTure.Mode = GPIO_MODE_OUTPUT_PP;	// 推挽输出
	GPIO_InitTure.Pull = GPIO_PULLUP;			// 上拉
	GPIO_InitTure.Speed = GPIO_SPEED_FREQ_HIGH; // 高速
	GPIO_InitTure.Pin = LED1_PIN;				// 设置GPIOx的5口
	HAL_GPIO_Init(LED1_GPIO, &GPIO_InitTure);	// 先在上面四行设置GPIO的模式，上下拉，速度，再对GPIOB管脚初始化

	HAL_GPIO_WritePin(LED1_GPIO, LED1_PIN, GPIO_PIN_SET); // PB5置1，默认初始化后灯灭
}

/**
 * @brief	LED0灯交替闪烁
 * @param 	none
 * @arg		none
 * @note  	初始化函数后利用HAL_GPIO_xxxx以达到LED灯状态取反
 * @retval	void
 */
void LED0_Reverse(void)
{
	static u8 i = 0;
	if (i)
		LED0_Clr();
	else
		LED0_Set();
	i = !i;
}

/**
 * @brief	LED1灯交替闪烁
 * @param 	none
 * @arg		none
 * @note  	初始化函数后利用HAL_GPIO_xxxx以达到LED灯状态取反
 * @retval	void
 */
void LED1_Reverse(void)
{
	static u8 i = 0;
	if (i)
		LED1_Clr();
	else
		LED1_Set();
	i = !i;
}