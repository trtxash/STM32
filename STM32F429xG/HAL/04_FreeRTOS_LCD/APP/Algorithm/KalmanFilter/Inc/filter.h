#ifndef _FILTER_H_
#define _FILTER_H_

typedef struct
{
    float Last_P; // 上次估算协方差 不可以为0,经典值0.1
    float Now_P;  // 当前估算协方差,经典值0
    float out;    // 卡尔曼滤波器输出,经典值0
    float Kg;     // 卡尔曼增益,经典值0
    float Q;      // 过程噪声协方差,经典值2.0
    float R;      // 观测噪声协方差,经典值0.05
} Kalman;

#ifdef __cplusplus
extern "C"
{
#endif

    float KalmanFilter(Kalman *kfp, float input);

#ifdef __cplusplus
}
#endif

#endif /* INC_FILTER_H_ */
