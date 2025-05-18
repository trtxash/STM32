#include "key.h"

#if SYSTEM_SUPPORT_OS == 1
struct Button button_up;

static uint8_t read_button_GPIO(uint8_t button_id)
{
    // you can share the GPIO read function with multiple Buttons
    switch (button_id)
    {
    case KEY_ID_UP:
        return HAL_GPIO_ReadPin(KEY_UP_Port, KEY_UP_PIN);
        break;
    default:
        return 0;
        break;
    }
}
#endif

// 按键初始化函数
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure = {0};

    KEY_UP_Port_CLK_ENABLE();

    GPIO_Initure.Pin = KEY_UP_PIN;
    GPIO_Initure.Mode = GPIO_MODE_INPUT; // 输入
    GPIO_Initure.Pull = GPIO_NOPULL;
    GPIO_Initure.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(KEY_UP_Port, &GPIO_Initure);

#if SYSTEM_SUPPORT_OS == 1
    button_init(&button_up, read_button_GPIO, 1, KEY_ID_UP);
#else
#endif
}
