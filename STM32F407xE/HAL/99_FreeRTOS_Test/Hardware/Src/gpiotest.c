#include "gpiotest.h"

u32 delay_time = 1000000 * (1.0 / (double)GPIO_BASE_PWM_FREQ) / 2.0;

// 启动所有GPIO
void GPIOTest_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_All;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ~(GPIO_PIN_13 | GPIO_PIN_14);
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

// 对所有引出GPIO进行方波输出,判断GPIO是否虚焊
void GPIOpwmout(void)
{
    delay_us(delay_time);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_All, !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_All));
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_All, !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_All));
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_All, !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_All));
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_All, !HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_All));
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_All, !HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_All));
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_All, !HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_All));
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_All, !HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_All));
}