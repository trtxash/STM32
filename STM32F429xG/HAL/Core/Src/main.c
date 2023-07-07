#include "main.h"

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