#include "main.h"

int main(void)
{
    if (HAL_Init()) // 初始化HAL库
    {
        Error_Handler();
    }
    Stm32_Clock_Init(150, 12, RCC_PLLP_DIV2, 4); // 设置时钟
    delay_init(150);                             // 初始化延时函数
    LED_Init();

    while (1)
    {
        delay_ms(250);
        LED0_Reverse();
        delay_ms(250);
        LED1_Reverse();
        delay_ms(250);
        LED2_Reverse();
        delay_ms(250);
        LED3_Reverse();
    }
}