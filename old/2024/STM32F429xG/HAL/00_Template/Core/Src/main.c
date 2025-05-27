/**
 * @file	00_Template
 * @brief
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2023年7月9号13点22分
 */

#include "main.h"

/**
 * @brief	对函数简要描述
 * @param 	参数说明，以’:’作为参数结束标志；
 * @arg		参数里面可选择参量列举，对于可数情况可进行参量列举，同样以’:’作为参数结束标志；
 * @note  	注释，配合brief一起使用可以达到很好的注释效果；
 * @retval	返回值说明。
 */
int main(void)
{
    if (HAL_Init()) // 初始化HAL库
    {
        Error_Handler();
    }
    Stm32_Clock_Init(240, 12, RCC_PLLP_DIV2, 8); // 设置时钟
    delay_init(240);                             // 初始化延时函数
    LED_Init();

    while (1)
    {
        delay_ms(50);
        LED0_Reverse();
        delay_ms(50);
        LED1_Reverse();
        delay_ms(50);
        LED2_Reverse();
        delay_ms(50);
        LED3_Reverse();
    }
}