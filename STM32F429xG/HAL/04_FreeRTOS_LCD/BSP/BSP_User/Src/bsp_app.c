#include "bsp_app.h"

/**
 * @brief	板级初始化函数
 * @param 	无
 * @arg
 * @note  	争对板子的初始化，包括时钟配置，外设初始化，中断配置等。
 * @retval	无
 */
void bsp_init(void)
{
    Stm32_Clock_Init(SYS_CLOCK, 12, RCC_PLLP_DIV2, 8); // 设置时钟
    delay_init(SYS_CLOCK);                             // 延时初始化
    LED_Init();
    KEY_Init();
    SDRAM_Init();
    ADC1_Init();
    DMA_Init();
    LCD_Init(); // 一般放在最后,等待LCD硬件上电完成
}
