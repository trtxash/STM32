#include "grayscale.h"

// 按键初始化函数
void Grayscale_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure = {0};

    GRAYSCALE0_Port_Clk_Enable();
    GRAYSCALE1_Port_Clk_Enable();
    GRAYSCALE2_Port_Clk_Enable();
    GRAYSCALE3_Port_Clk_Enable();
    GRAYSCALE4_Port_Clk_Enable();
    GRAYSCALE5_Port_Clk_Enable();
    GRAYSCALE6_Port_Clk_Enable();

    GPIO_Initure.Pin = GRAYSCALE0_Pin;
    GPIO_Initure.Mode = GPIO_MODE_INPUT; // 输入
    GPIO_Initure.Speed = GPIO_SPEED_LOW; // 高速
    HAL_GPIO_Init(GRAYSCALE0_Port, &GPIO_Initure);

    GPIO_Initure.Pin = GRAYSCALE1_Pin;
    GPIO_Initure.Mode = GPIO_MODE_INPUT; // 输入
    GPIO_Initure.Speed = GPIO_SPEED_LOW; // 高速
    HAL_GPIO_Init(GRAYSCALE1_Port, &GPIO_Initure);

    GPIO_Initure.Pin = GRAYSCALE2_Pin;
    GPIO_Initure.Mode = GPIO_MODE_INPUT; // 输入
    GPIO_Initure.Speed = GPIO_SPEED_LOW; // 高速
    HAL_GPIO_Init(GRAYSCALE2_Port, &GPIO_Initure);

    GPIO_Initure.Pin = GRAYSCALE3_Pin;
    GPIO_Initure.Mode = GPIO_MODE_INPUT; // 输入
    GPIO_Initure.Speed = GPIO_SPEED_LOW; // 高速
    HAL_GPIO_Init(GRAYSCALE3_Port, &GPIO_Initure);

    GPIO_Initure.Pin = GRAYSCALE4_Pin;
    GPIO_Initure.Mode = GPIO_MODE_INPUT; // 输入
    GPIO_Initure.Speed = GPIO_SPEED_LOW; // 高速
    HAL_GPIO_Init(GRAYSCALE4_Port, &GPIO_Initure);

    GPIO_Initure.Pin = GRAYSCALE5_Pin;
    GPIO_Initure.Mode = GPIO_MODE_INPUT; // 输入
    GPIO_Initure.Speed = GPIO_SPEED_LOW; // 高速
    HAL_GPIO_Init(GRAYSCALE5_Port, &GPIO_Initure);

    GPIO_Initure.Pin = GRAYSCALE6_Pin;
    GPIO_Initure.Mode = GPIO_MODE_INPUT; // 输入
    GPIO_Initure.Speed = GPIO_SPEED_LOW; // 高速
    HAL_GPIO_Init(GRAYSCALE6_Port, &GPIO_Initure);
}