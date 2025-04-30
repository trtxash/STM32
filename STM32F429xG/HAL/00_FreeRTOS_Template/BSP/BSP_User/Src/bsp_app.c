#include "bsp_app.h"

void bsp_init(void)
{
    SDRAM_Init();
    LED_Init();
}
