/**
 * @file	00_Template
 * @brief 	用于工程模板
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2022年2月17号18点42分
 */

#include "sys.h"
#include "delay.h"
#include "usart.h"

void Delay(__IO uint32_t nCount);

/**
 * @brief	对函数简要描述
 * @param 	参数说明，以’:’作为参数结束标志；
 * @arg		参数里面可选择参量列举，对于可数情况可进行参量列举，同样以’:’作为参数结束标志；
 * @note  	注释，配合brief一起使用可以达到很好的注释效果；
 * @retval	返回值说明。
 */
void Delay(__IO uint32_t nCount)
{
	while (nCount--)
	{
	}
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
	GPIO_InitTypeDef GPIO_Initure;

	HAL_Init();						//初始化HAL库
	Stm32_Clock_Init(RCC_PLL_MUL9); //设置时钟,72M

	__HAL_RCC_GPIOB_CLK_ENABLE(); //开启GPIOB时钟
	__HAL_RCC_GPIOE_CLK_ENABLE(); //开启GPIOE时钟

	GPIO_Initure.Pin = GPIO_PIN_5;			   // PB5
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;   //推挽输出
	GPIO_Initure.Pull = GPIO_PULLUP;		   //上拉
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; //高速
	HAL_GPIO_Init(GPIOB, &GPIO_Initure);

	GPIO_Initure.Pin = GPIO_PIN_5; // PE5
	HAL_GPIO_Init(GPIOE, &GPIO_Initure);

	while (1)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); // PB5置1
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET); // PE5置1
		Delay(0x7FFFFF);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); // PB5置0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET); // PE5置0
		Delay(0x7FFFFF);
	}
}