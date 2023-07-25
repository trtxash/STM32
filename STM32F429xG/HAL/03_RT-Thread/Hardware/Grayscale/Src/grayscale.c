#include "grayscale.h"

u8 Grayscale_Val[5] = {0};
u8 Grayscale_truesum = 0;

// 按键初始化函数
void Grayscale_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure = {0};

    GRAYSCALE0_Port_Clk_Enable();
    GRAYSCALE1_Port_Clk_Enable();
    GRAYSCALE2_Port_Clk_Enable();
    GRAYSCALE3_Port_Clk_Enable();
    GRAYSCALE4_Port_Clk_Enable();
    // GRAYSCALE5_Port_Clk_Enable();
    // GRAYSCALE6_Port_Clk_Enable();

    GPIO_Initure.Mode = GPIO_MODE_INPUT; // 输入
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_MEDIUM; // 高速

    GPIO_Initure.Pin = GRAYSCALE0_Pin;
    HAL_GPIO_Init(GRAYSCALE0_Port, &GPIO_Initure);

    GPIO_Initure.Pin = GRAYSCALE1_Pin;
    HAL_GPIO_Init(GRAYSCALE1_Port, &GPIO_Initure);

    GPIO_Initure.Pin = GRAYSCALE2_Pin;
    HAL_GPIO_Init(GRAYSCALE2_Port, &GPIO_Initure);

    GPIO_Initure.Pin = GRAYSCALE3_Pin;
    HAL_GPIO_Init(GRAYSCALE3_Port, &GPIO_Initure);

    GPIO_Initure.Pin = GRAYSCALE4_Pin;
    HAL_GPIO_Init(GRAYSCALE4_Port, &GPIO_Initure);

    // GPIO_Initure.Pin = GRAYSCALE5_Pin;
    // HAL_GPIO_Init(GRAYSCALE5_Port, &GPIO_Initure);

    // GPIO_Initure.Pin = GRAYSCALE6_Pin;
    // HAL_GPIO_Init(GRAYSCALE6_Port, &GPIO_Initure);
}

// 根据灰度信息计算出具体偏移位置
void Get_Grayscale_Val(void)
{
    static Grayscale_trueval_old = 0;

    Grayscale_truesum = 0;

    if (GRAYSCALE4_READ())
    {
        Grayscale_Val[0] = 1;
        Grayscale_truesum++;
    }
    else
        Grayscale_Val[0] = 0;

    if (GRAYSCALE3_READ())
    {
        Grayscale_Val[1] = 1;
        Grayscale_truesum++;
    }
    else
        Grayscale_Val[1] = 0;

    if (GRAYSCALE2_READ())
    {
        Grayscale_Val[2] = 1;
        Grayscale_truesum++;
    }
    else
        Grayscale_Val[2] = 0;

    if (GRAYSCALE1_READ())
    {
        Grayscale_Val[3] = 1;
        Grayscale_truesum++;
    }
    else
        Grayscale_Val[3] = 0;

    if (GRAYSCALE0_READ())
    {
        Grayscale_Val[4] = 1;
        Grayscale_truesum++;
    }
    else
        Grayscale_Val[4] = 0;
}
