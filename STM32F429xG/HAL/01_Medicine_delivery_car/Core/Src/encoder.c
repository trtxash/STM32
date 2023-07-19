#include "encoder.h"

short Encoder[2] = {0}; // 记录编码器的值
int Location_sum = 0;

/**
 * @brief  配置TIMx编码器模式
 * @param  无
 * @retval 无
 */
void Tim_Encoder_Init(void)
{
    MX_TIM2_Init(ENCODER_TIM_PERIOD_0, ENCODER_TIM_PRESCALER_0, ENCODER_FILTER_VALUE_0);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL); // 使能编码器接口

    MX_TIM3_Init(ENCODER_TIM_PERIOD_1, ENCODER_TIM_PRESCALER_1, ENCODER_FILTER_VALUE_1);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL); // 使能编码器接口

    // MX_TIM4_Init(ENCODER_TIM_PERIOD_2, ENCODER_TIM_PRESCALER_2, ENCODER_FILTER_VALUE_2);
    // HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL); // 使能编码器接口

    // MX_TIM5_Init(ENCODER_TIM_PERIOD_3, ENCODER_TIM_PRESCALER_3, ENCODER_FILTER_VALUE_3);
    // HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL); // 使能编码器接口
}

/**
 * @brief	读取编码器数值函数
 * @param 	none
 * @arg		none
 * @note  	读取相关编码器的数值
 * @retval	short
 */
short Read_Encoder(TIM_HandleTypeDef *htim)
{
    short Encoder_TIM = 0;
    Encoder_TIM = (short)(__HAL_TIM_GET_COUNTER(htim)); // 获取计数器值
    __HAL_TIM_SET_COUNTER(htim, 0);                     // 计数器计数清零

    return Encoder_TIM;
}

/**
 * @brief	计算具体速度函数
 * @param 	none
 * @arg		none
 * @note  	根据编码器读数和轮直径计算具体速度
 * @retval	short
 */
double Calculate_Velocity(short encoder_value)
{
    double velocity = 0;
    /* 转速为周长*编码器读数*当前频率/电机减速比/电机总分辨率 */
    velocity = ONE_WHEEL_CIRCUMFERENCE * (double)encoder_value * READ_ENCODER_FREQU / REDUCTION_RATIO / ENCODER_TOTAL_RESOLUTION;
    return velocity;
}

/**
 * @brief	计算具体路程函数
 * @param 	none
 * @arg		none
 * @note  	根据编码器读数和轮直径计算具体路程
 * @retval	int
 */
int Location_integral(short encoder_value)
{
    /* 路程为周长*编码器读数/电机减速比/电机总分辨率,做个积分 */
    Location_sum += encoder_value * 0.01;

    return Location_sum;
}