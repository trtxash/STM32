#ifndef _ENCOEDER_H
#define _ENCOEDER_H

#include "sys.h"
#include "tim.h"

/* 定时器溢出值 */
#define ENCODER_TIM_PERIOD_0 0XFFFF

#define ENCODER_TIM_PERIOD_1 0XFFFF

#define ENCODER_TIM_PERIOD_2 0XFFFF

#define ENCODER_TIM_PERIOD_3 0XFFFF

/* 定时器预分频值 */
#define ENCODER_TIM_PRESCALER_0 0

#define ENCODER_TIM_PRESCALER_1 0

#define ENCODER_TIM_PRESCALER_2 0

#define ENCODER_TIM_PRESCALER_3 0

/* 编码器软件滤波值 */
#define ENCODER_FILTER_VALUE_0 30

#define ENCODER_FILTER_VALUE_1 30

#define ENCODER_FILTER_VALUE_2 30

#define ENCODER_FILTER_VALUE_3 30

/* 读取编码器的频率 单位Hz */
#define READ_ENCODER_FREQU 100

/* 编码器物理分辨率 */
#define ENCODER_RESOLUTION 500

/* 经过倍频之后的总分辨率 */
#define ENCODER_TOTAL_RESOLUTION (ENCODER_RESOLUTION * 4) /* 4倍频后的总分辨率 */

/* 减速电机减速比 */
#define REDUCTION_RATIO 30

/* 轮胎直径 单位M */
#define TIRE_DIAMETER 0.065

/* 计算转一圈的距离 单位M */
#define ONE_WHEEL_CIRCUMFERENCE (3.14159265358979323846 * TIRE_DIAMETER)

void TIM_Encoder_Init(void);
int Read_Encoder(u8 tim);
double Calculate_Velocity(int encoder_value);

#endif /* __BSP_ENCODER_H */