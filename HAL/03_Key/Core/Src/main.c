/**
 * @file	03_Key
 * @brief 	检查按键按下，key0按下则红灯亮，key1按下则黄灯亮，weakup按下则蜂鸣器响，按键要检查是否长按
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2022年2月17号23点55分
 */

#include "stm32f1xx.h"

void Beep_Init(void);
void LED_Init(void);
void Key_Init(void);
uint8_t Key_Scan(uint8_t MODE);

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
 * @brief	利用HAL库函数进行Key初始化
 * @param 	none
 * @arg		none
 * @note  	先开启GPIO时钟，再利用HAL_GPIO_Init();函数进行管脚初始化,注意上下拉
 * @retval	none
 */
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitTure;

	__HAL_RCC_GPIOA_CLK_ENABLE(); // 开启GPIOA时钟
	__HAL_RCC_GPIOE_CLK_ENABLE(); // 开启GPIOE时钟
	/*ARM的芯片都是这样，外设通常都是给了时钟后，才能设置它的寄存器,这么做的目的是为了省电，使用了所谓时钟门控的技术。*/

	/*进行结构体内的参数配置，先找到下面HAL_GPIO_Init();的定义处，再对定义处的函数详细找参数*/
	GPIO_InitTure.Mode = GPIO_MODE_INPUT;		// 输入
	GPIO_InitTure.Pull = GPIO_PULLDOWN;			// 下拉
	GPIO_InitTure.Speed = GPIO_SPEED_FREQ_HIGH; // 高速
	GPIO_InitTure.Pin = GPIO_PIN_0;				// 设置GPIOx的0口

	HAL_GPIO_Init(GPIOA, &GPIO_InitTure);		 // GPIOA管脚初始化
	GPIO_InitTure.Pull = GPIO_PULLUP;			 // 上拉
	GPIO_InitTure.Pin = GPIO_PIN_3 | GPIO_PIN_4; // 设置GPIOx的3口,设置GPIOx的4口
	HAL_GPIO_Init(GPIOE, &GPIO_InitTure);		 // GPIOE管脚初始化
}

/**
 * @brief	按键扫描喊出
 * @param 	MODE;KEY;
 * @arg		MODE = 0为单次扫描， MODE = 1为连续扫描；Keyx为扫描x按键
 * @note
 * @retval	Key_up,按键是否松开，0否，1是
 */
uint8_t Key_Scan(uint8_t MODE)
{
	static uint8_t Key_up = 1; // 默认赋值为松开,static变量只初始化一次
	uint16_t Key_Number;	   // 检查按下几个按键

	Key_Number = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) * 4 + !HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) * 2 + !HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4); // 记录哪些键按下
	if (MODE == 1)																															// 支持连按
	{
		Key_up = 1;
	}
	if (Key_Number && Key_up != 0) // 按下检测和连按检测
	{
		Key_up = 0;			// 记录按下
		switch (Key_Number) // 按位发送哪些按键按下
		{
		case 0B0001u:
			return 1;
		case 0B0010u:
			return 2;
		case 0B0011u:
			return 3;
		case 0B0100u:
			return 4;
		case 0B0101u:
			return 5;
		case 0B0110u:
			return 6;
		case 0B0111u:
			return 7;
		default:
			break;
		}
	}
	else
		return 0; // 无按键按下
}

/**
 * @brief	检查按键按下，key0按下则红灯亮，key1按下则黄灯亮，weakup按下则蜂鸣器响，按键要检查是否长按
 * @param 	none
 * @arg		none
 * @note  	none
 * @retval	int
 */
int main(void)
{
	HAL_Init();	 // 初始化HAL库
	Beep_Init(); // 初始化Beep
	LED_Init();	 // 初始化LED
	Key_Init();	 // 初始化Key

	while (1)
	{
		HAL_Delay(9);		 // 延迟10ms按键消抖
		switch (Key_Scan(1)) // 按键扫描
		{
		case 0:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);	  // PB5置1，灯灭
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);	  // PE5置1，灯灭
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); // PB8置0，不响
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); // PB5置0，灯亮
			HAL_Delay(9);										  // 延迟10ms按键消抖
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET); // PE5置0，灯亮
			HAL_Delay(9);										  // 延迟10ms按键消抖
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); // PB5置0，灯亮
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET); // PE5置0，灯亮
			HAL_Delay(9);										  // 延迟10ms按键消抖
			break;
		case 4:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET); // PB8置1，响
			HAL_Delay(9);										// 延迟10ms按键消抖
			break;
		case 5:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); // PB5置0，灯亮
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);	  // PB8置1，响
			HAL_Delay(9);										  // 延迟10ms按键消抖
			break;
		case 6:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET); // PE5置0，灯亮
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);	  // PB8置1，响
			HAL_Delay(9);										  // 延迟10ms按键消抖
			break;
		case 7:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); // PB5置0，灯亮
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET); // PE5置0，灯亮
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);	  // PB8置1，响
			HAL_Delay(9);										  // 延迟10ms按键消抖
			break;
		}
	}
}