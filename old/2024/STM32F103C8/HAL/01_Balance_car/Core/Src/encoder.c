#include "encoder.h"

int Encoder_val[2];

/**
 * @brief  配置TIMx编码器模式
 * @param  无
 * @retval 无
 */
void Tim_Encoder_Init(void)
{
    MX_TIM2_Init(ENCODER_TIM_PERIOD_0, ENCODER_TIM_PRESCALER_0, ENCODER_FILTER_VALUE_0);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL); // 使能编码器接口

    // MX_TIM3_Init(ENCODER_TIM_PERIOD_1, ENCODER_TIM_PRESCALER_1, ENCODER_FILTER_VALUE_1);
    // HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL); // 使能编码器接口

    MX_TIM4_Init(ENCODER_TIM_PERIOD_1, ENCODER_TIM_PRESCALER_1, ENCODER_FILTER_VALUE_1);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL); // 使能编码器接口

    // MX_TIM5_Init(ENCODER_TIM_PERIOD_3, ENCODER_TIM_PRESCALER_3, ENCODER_FILTER_VALUE_3);
    // HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL); // 使能编码器接口
}

/**
 * @brief	读取编码器数值函数
 * @param 	none
 * @arg		none
 * @note  	读取相关编码器的数值，现有TIM1~TIM5
 * @retval	int
 */
int Read_Encoder(u8 tim)
{
    int Encoder_TIM = 0;
    switch (tim) // 读取相关定时器编码器的数值
    {
    case 1:
        Encoder_TIM = TIM1->CNT;
        break;
    case 2:
        Encoder_TIM = TIM2->CNT;
        break;
    case 3:
        Encoder_TIM = TIM3->CNT;
        break;
    case 4:
        Encoder_TIM = TIM4->CNT;
        break;
    default:
        break;
    }
    Encoder_TIM = (short)Encoder_TIM; // 将数值转换为short类型,实现正反转对应正负值
    switch (tim)                      // 读完清除相关定时器编码器的数值
    {
    case 1:
        TIM1->CNT = 0;
        break;
    case 2:
        TIM2->CNT = 0;
        break;
    case 3:
        TIM3->CNT = 0;
        break;
    case 4:
        TIM4->CNT = 0;
        break;
    default:
        break;
    }
    return Encoder_TIM;
}

/**
 * @brief	计算具体速度函数
 * @param 	none
 * @arg		none
 * @note  	根据编码器读数和轮直径计算具体速度
 * @retval	int
 */
double Calculate_Velocity(int encoder_value)
{
    double velocity = 0;
    /* 转速为周长*编码器读数*当前频率/电机减速比/电机总分辨率 */
    velocity = ONE_WHEEL_CIRCUMFERENCE * (double)encoder_value * READ_ENCODER_FREQU / REDUCTION_RATIO / ENCODER_TOTAL_RESOLUTION;
    return velocity;
}