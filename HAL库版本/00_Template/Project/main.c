/*00_模板
 *
 *功能
 *
 *目的
 *
 */

/* 包含头文件 -----------------------------------------------------------------*/
#include "sys.h"
#include "delay.h"
#include "usart.h"

/* 函数定义 -------------------------------------------------------------------*/
void Delay(__IO uint32_t nCount);

/*******************************************************************************
 * 函 数 名      : main
 * 函数功能      : 主函数
 * 输    入      :
 * 输    出      :
 ******************************************************************************/
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

/*******************************************************************************
 * 函 数 名      : Delay
 * 函数功能      : 
 * 输    入      :
 * 输    出      :
 ******************************************************************************/
void Delay(__IO uint32_t nCount)
{
	while (nCount--)
	{
	}
}

// 2022年2月14号3点19分