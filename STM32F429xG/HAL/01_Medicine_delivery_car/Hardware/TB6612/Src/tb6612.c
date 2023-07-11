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
    GPIO_InitStructure.Pin = TB6612_AIN1_Pin;
    HAL_GPIO_Init(TB6612_AIN1_Port, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = TB6612_AIN2_Pin;
    HAL_GPIO_Init(TB6612_AIN2_Port, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = TB6612_BIN1_Pin;
    HAL_GPIO_Init(TB6612_BIN1_Port, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = TB6612_BIN2_Pin;
    HAL_GPIO_Init(TB6612_BIN2_Port, &GPIO_InitStructure);

    MX_TIM5_Init(7199, 0); // 10kHz PWM,两通道c1，c4

    HAL_TIM_PWM_Start(&TB6612_TIM, TIM_CHANNEL_2); // 开启定时器1通道1的PWM输出，左轮电机使用
    HAL_TIM_PWM_Start(&TB6612_TIM, TIM_CHANNEL_3); // 开启定时器1通道4的PWM输出，右轮电机使用
}

// 控制左右轮速度,arr范围7199 ~ -7199
void TB6612_control_speed(int arr_r, int arr_l)
{
    if (arr_r >= 0) // 正转
    {
        TB6612_AIN1_Clr(); // 右轮正转
        TB6612_AIN2_Set();
        __HAL_TIM_SET_COMPARE(&TB6612_TIM, TIM_CHANNEL_2, arr_r); // tim1，ch1大，左轮
    }
    else
    {
        TB6612_AIN1_Set(); // 右轮反转
        TB6612_AIN2_Clr();
        __HAL_TIM_SET_COMPARE(&TB6612_TIM, TIM_CHANNEL_2, -arr_r); // tim1，ch1大，左轮
    }

    if (arr_l >= 0) // 正转
    {
        TB6612_BIN1_Set(); // 左轮正转
        TB6612_BIN2_Clr();
        __HAL_TIM_SET_COMPARE(&TB6612_TIM, TIM_CHANNEL_3, arr_l); // tim1，ch1大，左轮
    }
    else
    {
        TB6612_BIN1_Clr(); // 左轮反转
        TB6612_BIN2_Set();
        __HAL_TIM_SET_COMPARE(&TB6612_TIM, TIM_CHANNEL_3, -arr_l); // tim1，ch1大，左轮
    }
}