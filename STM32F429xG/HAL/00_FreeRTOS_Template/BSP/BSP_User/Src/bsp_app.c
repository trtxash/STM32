#include "bsp_app.h"
// #include "fmc.h"
// #include "log_rtt.h"
// #include "tim.h"
#include "led.h"
#include "sdram.h"

void bsp_init(void)
{
    Stm32_Clock_Init(SYS_CLOCK, 12, RCC_PLLP_DIV2, 8); // 设置时钟
    delay_init(SYS_CLOCK);                             // 延时初始化
    LED_Init();
    SDRAM_Init();
}
