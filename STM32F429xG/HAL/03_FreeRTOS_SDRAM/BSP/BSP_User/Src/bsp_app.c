#include "bsp_app.h"
#include "key.h"
#include "lcd.h"
#include "led.h"
#include "sdram.h"


void bsp_init(void)
{
    Stm32_Clock_Init(SYS_CLOCK, 12, RCC_PLLP_DIV2, 8); // 设置时钟
    delay_init(SYS_CLOCK);                             // 延时初始化
    LED_Init();
    KEY_UP_Init();
    // KEY_BOOT_Init();
    SDRAM_Init();
    lcd_disp_config();
}
