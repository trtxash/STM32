#include "key.h"

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
}
