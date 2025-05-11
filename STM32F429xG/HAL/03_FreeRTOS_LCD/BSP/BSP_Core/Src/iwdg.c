#include "iwdg.h"

IWDG_HandleTypeDef hiwdg;

// 初始化独立看门狗
// prer:分频数:IWDG_PRESCALER_4~IWDG_PRESCALER_256
// rlr:自动重装载值,0~0XFFF.
// 时间计算(大概):Tout=((4*2^prer)*rlr)/32 (ms).
void IWDG_Init(uint8_t prer, uint16_t rlr)
{
    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = prer; // 设置IWDG分频系数
    hiwdg.Init.Reload = rlr;     // 重装载值
    if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
    {
        Error_Handler();
    }
}

// 喂独立看门狗
void IWDG_Feed(void)
{
    HAL_IWDG_Refresh(&hiwdg); // 重装载
}