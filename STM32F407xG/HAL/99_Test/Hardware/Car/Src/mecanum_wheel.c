#include "mecanum_wheel.h"

extern uint32_t SystemCoreClock;

/**
 * @brief	配置定时器1，8输出PMW波控制电机
 * @param 	none
 * @arg		none
 * @note  	100KHz,1000分辨率占空比
 * @retval	void
 */
void Tim_Motor_Init()
{
    u8 i;

    MX_TIM1_Init(600 - 1, (u16)(SystemCoreClock / 60000000 - 1)); // 100KHz,1000分辨率占空比
    MX_TIM8_Init(600 - 1, (u16)(SystemCoreClock / 60000000 - 1));
    MX_TIM13_Init(600 - 1, (u16)(SystemCoreClock / 60000000 - 1));

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 600 - 1); // 默认0占空比
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 600 - 1); // 默认0占空比
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 600 - 1); // 默认0占空比
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 600 - 1); // 默认0占空比
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 600 - 1);
    __HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, 600 - 1);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, 600 - 1);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 600 - 1);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, 600 - 1);

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
    // HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
}