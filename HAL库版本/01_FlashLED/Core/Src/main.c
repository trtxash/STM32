/**
 * @file	01_FlashLED
 * @brief 	LED灯交替闪烁
 * 			锻炼使用HAL库，对工程模板有更好的理解
 * 			为学寄存器版本打基础
 * 			建立一个高效的学习模板
 * @author 	TRTX-gamer
 * @version 1.0
 * @date 	2022年2月17号18点50分
 */

#include "delay.h"
#include "sys.h"

void LED_Init(void);

/**
 * @brief	对函数简要描述
 * @param 	参数说明，以’:’作为参数结束标志；
 * @arg		参数里面可选择参量列举，对于可数情况可进行参量列举，同样以’:’作为参数结束标志；
 * @note  	注释，配合brief一起使用可以达到很好的注释效果；
 * @retval	返回值说明。
 */
void LED_Init(void)
{

	__HAL_RCC_GPIOB_CLK_ENABLE(); // 开启GPIOB时钟
	__HAL_RCC_GPIOE_CLK_ENABLE(); // 开启GPIOE时钟
	/*ARM的芯片都是这样，外设通常都是给了时钟后，才能设置它的寄存器,这么做的目的是为了省电，使用了所谓时钟门控的技术。*/

	HAL_GPIO_Init();
}

/**
 * @brief	对函数简要描述
 * @param 	参数说明，以’:’作为参数结束标志；
 * @arg		参数里面可选择参量列举，对于可数情况可进行参量列举，同样以’:’作为参数结束标志；
 * @note  	注释，配合brief一起使用可以达到很好的注释效果；
 * @retval	返回值说明。
 */
void main(void)
{
	HAL_Init();						//初始化HAL库
	Stm32_Clock_Init(RCC_PLL_MUL9); //设置时钟,72M，因为几乎都要用时钟，最先考虑设置时钟,后面再详细学习时钟相关HAL库函数，先用
	LED_Init();						//初始化LED
}

/*----------------------------下面是正点原子的程序源码--------------------------*/
/*
#include "sys.h"
#include "delay.h"
#include "usart.h"

void LED_Init(void);

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOB_CLK_ENABLE();           	//开启GPIOB时钟
	__HAL_RCC_GPIOE_CLK_ENABLE();           	//开启GPIOE时钟

	GPIO_Initure.Pin=GPIO_PIN_5; 				//PB5
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);

	GPIO_Initure.Pin=GPIO_PIN_5; 				//PE5
	HAL_GPIO_Init(GPIOE,&GPIO_Initure);

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);	//PB5置1，默认初始化后灯灭
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);	//PE5置1，默认初始化后灯灭
}

int main(void) // 使用HAL库函数实现控制IO口输出
{
	HAL_Init();                    	 	//初始化HAL库
	Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);               		//初始化延时函数
	LED_Init();							//初始化LED

	while(1)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET); 	//LED0对应引脚PB5拉低，亮，等同于LED0(0)
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);   	//LED1对应引脚PE5拉高，灭，等同于LED1(1)
		delay_ms(500);											//延时500ms
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);   	//LED0对应引脚PB5拉高，灭，等同于LED0(1)
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET); 	//LED1对应引脚PE5拉低，亮，等同于LED1(0)
		delay_ms(500);                                      	//延时500ms
	}
}

int main(void) // 使用位带操作实现
{
	HAL_Init();                    	 	//初始化HAL库
	Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);               			//初始化延时函数
	LED_Init();							//初始化LED

	while(1)
	{
		 LED0=0;			     	//LED0亮
		 LED1=1;				 	//LED1灭
		 delay_ms(500);
		 LED0=1;					//LED0灭
		 LED1=0;					//LED1亮
		 delay_ms(500);
	 }
}

int main(void) // 使用直接操作结存器方式实现跑马灯
{
	HAL_Init();                    	 	//初始化HAL库
	Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);               		//初始化延时函数
	LED_Init();							//初始化LED

	while(1)
	{
	  GPIOB->BSRR=GPIO_PIN_5;     	//LED0亮
	  GPIOE->BSRR=GPIO_PIN_5<<16; 	//LED1灭
	  delay_ms(500);
	  GPIOB->BSRR=GPIO_PIN_5<<16; 	//LED0灭
	  GPIOE->BSRR=GPIO_PIN_5;     	//LED1亮
	  delay_ms(500);
	 }
}
*/