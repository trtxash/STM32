#include "mecanum_wheel.h"

extern uint32_t SystemCoreClock;

/**
 * @brief	配置定时器1，8输出PMW波控制电机
 * @param 	none
 * @arg		none
 * @note  	PWM 100KHz,1000份可调占空比
 * @retval	void
 */
void Tim_Motor_Init()
{
    u8 i;

    MX_TIM1_Init(600 - 1, (u16)(SystemCoreClock / 60000000 - 1)); // 100KHz,1000分辨率占空比
    MX_TIM8_Init(600 - 1, (u16)(SystemCoreClock / 60000000 - 1));
    MX_TIM13_Init(600 - 1, (u16)(SystemCoreClock / 60000000 - 1));

    mecanum_wheel_stop();

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

/**
 * @brief	计算，设置PWM占空比
 * @param 	none
 * @arg		none
 * @note
 * @retval	void
 */
void mecanum_wheel_pwm_set()
{
    short *p = Velocity_FeedbackControl(Encoder_target, Encoder); // PID计算，p为控制值指针
    if (p[0] >= 0)                                                // 正转
    {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 600 - p[0]); // tim1，ch1大，正转
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 600 - 1);    // tim1，ch2大，反转
    }
    else // 反转
    {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 600 + p[0]);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 600 - 1);
    }
    if (p[1] >= 0) // 正转
    {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 600 - 1);    // tim1，ch3大，反转
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 600 - p[1]); // tim1，ch4大，正转
    }
    else
    {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 600 + p[1]);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 600 - 1);
    }
    if (p[2] >= 0) // 正转
    {
        __HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, 600 - 1);   // tim13，ch1大，反转
        __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, 600 - p[2]); // tim8，ch2大，正转
    }
    else
    {
        __HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, 600 + p[2]);
        __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, 600 - 1);
    }
    if (p[3] >= 0) // 正转
    {
        __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 600 - p[3]); // tim8，ch3大，正转
        __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, 600 - 1);    // tim8，ch4大，反转
    }
    else
    {
        __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 600 - 1);
        __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, 600 + p[3]);
    }
}

/**
 * @brief	计算，设置xyz速度
 * @param 	none
 * @arg		none
 * @note    输入位置方向速度Vx，Vy,Vz。得到电机方向速度。
 * @retval	void
 */
void mecanum_wheel_xyz_set()
{
    Encoder_target[0] = (short)((rxvaluepack.shorts[0] + rxvaluepack.shorts[1]) / (float)1.414213562373);
    Encoder_target[2] = (short)((rxvaluepack.shorts[0] + rxvaluepack.shorts[1]) / (float)1.414213562373);

    Encoder_target[1] = (short)((-rxvaluepack.shorts[0] + rxvaluepack.shorts[1]) / (float)1.414213562373);
    Encoder_target[3] = (short)((-rxvaluepack.shorts[0] + rxvaluepack.shorts[1]) / (float)1.414213562373);

    Encoder_target[0] -= rxvaluepack.shorts[2];
    Encoder_target[3] -= rxvaluepack.shorts[2];

    Encoder_target[1] += rxvaluepack.shorts[2];
    Encoder_target[2] += rxvaluepack.shorts[2];

    for (u8 i = 0; i < 4; i++)
    {
        if (Encoder_target[i] > 22000)
            Encoder_target[i] = 22000;
        else if (Encoder_target[i] < -22000)
            Encoder_target[i] = -22000;
    }
}

/**
 * @brief	速度归零
 * @param 	none
 * @arg		none
 * @note
 * @retval	void
 */
void mecanum_wheel_stop()
{
    for (u8 i = 0; i < 4; i++)
    {
        Encoder_target[i] = 0;
    }

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 600 - 1); // 默认0占空比
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 600 - 1); // 默认0占空比
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 600 - 1); // 默认0占空比
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 600 - 1); // 默认0占空比
    // __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 600 - 1);
    __HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, 600 - 1);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, 600 - 1);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 600 - 1);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, 600 - 1);
}