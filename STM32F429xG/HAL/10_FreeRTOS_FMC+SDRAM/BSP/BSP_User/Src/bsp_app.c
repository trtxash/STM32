#include "bsp_app.h"

void bsp_init(void)
{
    LED_Init();
    IWDG_Init(IWDG_PRESCALER_64, 500); // 约0.94s
    WWDG_Init(0X7F, 0X5F, WWDG_PRESCALER_8);
}