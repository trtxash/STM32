#include "infraredtobe.h"

//按键初始化函数
void Infraredtobe_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOB_CLK_ENABLE(); //开启GPIOB时钟

    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_10; // PB0 PB1 PB2 PB10
    GPIO_Initure.Mode = GPIO_MODE_INPUT;                                   //输入
    GPIO_Initure.Pull = GPIO_PULLUP;                                       //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;                                  //高速
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);
}

int Read_Infraredtobe(void)
{
    int temp;
    temp = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) + 10 * HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) + 100 * HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) + 1000 * HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10);
    return temp;
}