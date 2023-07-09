#include "key.h"

// 按键初始化函数
void KEY0_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    KEY0_Port_CLK_ENABLE();

    GPIO_Initure.Pin = KEY0_PIN;
    GPIO_Initure.Mode = GPIO_MODE_INPUT; // 输入
    GPIO_Initure.Pull = GPIO_PULLUP;     // 上拉
    GPIO_Initure.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(KEY0_Port, &GPIO_Initure);
}

void KEY1_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    KEY1_Port_CLK_ENABLE();

    GPIO_Initure.Pin = KEY1_PIN;
    GPIO_Initure.Mode = GPIO_MODE_INPUT; // 输入
    GPIO_Initure.Pull = GPIO_PULLUP;     // 上拉
    GPIO_Initure.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(KEY1_Port, &GPIO_Initure);
}
