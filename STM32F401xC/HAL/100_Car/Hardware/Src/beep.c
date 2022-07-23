#include "beep.h"

u8 beep = 0;
u8 beep_flag = 0;

//按键初始化函数
void Beep_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOB_CLK_ENABLE(); //开启GPIOB时钟

    GPIO_Initure.Pin = GPIO_PIN_0;        // PB0
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  
    GPIO_Initure.Pull = GPIO_PULLUP;      //下拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH; //高速
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);
}