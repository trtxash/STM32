/**
 * @file	05_MPU9250
 * @brief
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2023年7月26号17点6分
 */

// 未完，有时间再补

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
    u8 temp[21] = {0};

    if (HAL_Init()) // 初始化HAL库
    {
        Error_Handler();
    }
    Stm32_Clock_Init(240, 12, RCC_PLLP_DIV2, 8); // 设置时钟
    delay_init(240);                             // 初始化延时函数
    MX_DMA_Init();
    uart2_init(115200);
    MX_SPI6_Init();
    LED_Init();
    OLED_Init();
    MX_TIM6_Init((u16)(50000 - 1), (u16)(120 - 1)); // 定时器6初始化，周期50ms
    MX_TIM7_Init((u16)(1000 - 1), (u16)(120 - 1));  // 定时器7初始化，周期1ms
    SetAngle(1, 0, 500, 0);
    while (1)
    {
        static u32 i = 0;

        sprintf(temp, "%d ", i);
        OLED_ShowString(0, 0, temp, 8, 1, WHITE);
        i++;
        delay_ms(50);
    }
}