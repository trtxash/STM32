#include "mecanum_wheel.h"

extern uint32_t SystemCoreClock;

/**
 * @brief	配置定时器1，8输出PMW波控制电机
 * @param 	none
 * @arg		none
 * @note  	100KHz,10000分辨率占空比
 * @retval	void
 */
void Tim_Motor_Init()
{
    MX_TIM1_Init(10000 - 1, SystemCoreClock / 1000 - 1); // 100KHz,10000分辨率占空比
    MX_TIM8_Init(10000 - 1, SystemCoreClock / 1000 - 1);

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_ALL, 10000 - 1); // 默认0占空比
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_ALL, 10000 - 1);

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_ALL);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_ALL);
}