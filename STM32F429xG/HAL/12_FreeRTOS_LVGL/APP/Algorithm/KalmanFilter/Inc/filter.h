#ifndef INC_FILTER_H_
#define INC_FILTER_H_

#include "main.h"

typedef struct
{
    float Last_P; // 上次估算协方差 不可以为0 ! ! ! ! !
    float Now_P;  // 当前估算协方差
    float out;    // 卡尔曼滤波器输出
    float Kg;     // 卡尔曼增益
    float Q;      // 过程噪声协方差
    float R;      // 观测噪声协方差
} Kalman;

extern Kalman kfp_x;
extern Kalman kfp_y;

void Kalman_Init(void);
float KalmanFilter(Kalman *kfp, float input);
float Kalman_Filter_x(float Accel, float Gyro);
float Complementary_Filter_x(float angle_m, float gyro_m);
float Kalman_Filter_y(float Accel, float Gyro);
float Complementary_Filter_y(float angle_m, float gyro_m);

#endif /* INC_FILTER_H_ */
