/**
 * @file	04_Clock_Interrupt
 * @brief 	学习利用HAL库配置STM32
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2022年3月27号15点45分
 */

#include "stm32f1xx.h"

void Stm32_ClockInit(uint32_t PLL);

/**
 * @brief	Stm32f103时钟配置和初始化
 * @param 	RCC_PLL_MUL2~RCC_PLL_MUL16；
 * @arg		乘法因子，一般为RCC_PLL_MUL9
 * @note
 * @retval
 */
void Stm32_ClockInit(uint32_t PLL)
{
	HAL_StatusTypeDef ret = HAL_OK;
	RCC_OscInitTypeDef RCC_OscInitStructure; // 结构体命名为RCC_OscInitStructure
	RCC_ClkInitTypeDef RCC_ClkInitStructure; // 结构体命名为RCC_ClkInitStructure

	//选中HSE作为PLL时钟源并且配置HSE,PLL
	RCC_OscInitStructure.OscillatorType = RCC_OSCILLATORTYPE_HSE; // 选择配置HSE振荡器
	RCC_OscInitStructure.HSEState = RCC_HSE_ON;					  // 打开HSE
	RCC_OscInitStructure.HSEPredivValue = RCC_HSE_PREDIV_DIV1;	  // HSE预分频，Prediv1值
	RCC_OscInitStructure.PLL.PLLState = RCC_PLL_ON;				  // PLL状态打开
	RCC_OscInitStructure.PLL.PLLSource = RCC_PLLSOURCE_HSE;		  // PLL时钟源设置为HSE
	RCC_OscInitStructure.PLL.PLLMUL = PLL;						  // PLL VCO输入时钟的乘法因子，主PLL倍频因子

	ret = HAL_RCC_OscConfig(&RCC_OscInitStructure); //初始化

	if (ret != HAL_OK) // 判断是否初始化出错
		while (1)
		{
		}

	//选中PLL作为系统时钟源并且配置HCLK,PCLK1和PCLK2
	RCC_ClkInitStructure.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStructure.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // 时钟源为PLL
	RCC_ClkInitStructure.AHBCLKDivider = RCC_SYSCLK_DIV1;		 // AHB分频系数为1
	RCC_ClkInitStructure.APB1CLKDivider = RCC_HCLK_DIV2;		 // APB1分频系数为2
	RCC_ClkInitStructure.APB2CLKDivider = RCC_HCLK_DIV1;		 // APB2分频系数为1

	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStructure, FLASH_LATENCY_2); //同时设置FLASH延时周期为2WS，也就是3个CPU周期。

	if (ret != HAL_OK) // 判断是否初始化出错
		while (1)
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
	Stm32_ClockInit(RCC_PLL_MUL9); //设置时钟,72M

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
	}
}