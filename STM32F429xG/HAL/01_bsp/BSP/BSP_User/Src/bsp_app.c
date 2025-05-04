#include "bsp_app.h"

void bsp_init(void)
{
    Stm32_Clock_Init(SYS_CLOCK, 12, RCC_PLLP_DIV2, 8); // 设置时钟
    delay_init(SYS_CLOCK);                             // 延时初始化
    LED_Init();
}
