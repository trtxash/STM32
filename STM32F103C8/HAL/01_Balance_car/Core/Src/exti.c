#include "exti.h"
#include "delay.h"

// 外部中断初始化
void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE(); // 开启GPIOA时钟

    GPIO_Initure.Pin = GPIO_PIN_12;          // PA12
    GPIO_Initure.Mode = GPIO_MODE_IT_RISING; // 下降沿触发
    GPIO_Initure.Pull = GPIO_PULLUP;         // 上拉
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0); // 抢占优先级为0，子优先级为0
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);         // 使能中断线0
}
