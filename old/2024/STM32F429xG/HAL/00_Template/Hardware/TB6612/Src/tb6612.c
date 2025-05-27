#include "tb6612.h"

/**
 * @brief   TB6612初始化函数
 * @param   none
 * @arg		none
 * @note
 * @retval  void
 */
void TB6612_init()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    /* GPIO时钟开启 */
    TB6612_AIN1_Port_Clk_Enable();
    TB6612_AIN2_Port_Clk_Enable();
    TB6612_BIN1_Port_Clk_Enable();
    TB6612_BIN2_Port_Clk_Enable();

    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStructure.Pin = TB6612_AIN1_Pin | TB6612_AIN2_Pin | TB6612_BIN1_Pin | TB6612_BIN2_Pin;
    HAL_GPIO_Init(TB6612_AIN1_Port, &GPIO_InitStructure);

    MX_TIM1_Init(7199, 0); // 10kHz PWM,两通道c1，c4

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // 开启定时器1通道1的PWM输出，左轮电机使用
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4); // 开启定时器1通道4的PWM输出，右轮电机使用
}