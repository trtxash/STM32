#ifndef _FILTER_H_
#define _FILTER_H_

typedef struct
{
    float Last_P; // 上次估算协方差 不可以为0,经典值0.1;与滤波器的初始状态有关，通常在滤波器初始化时给定。P值越大，滤波器的初始状态越不确定，滤波器会更快地调整其估计值以适应实际数据。
    float Now_P;  // 当前估算协方差,经典值0
    float out;    // 卡尔曼滤波器输出,经典值0
    float Kg;     // 卡尔曼增益,经典值0;
    float Q;      // 过程噪声协方差,经典值0.5;表示对模型的信任程度
    float R;      // 观测噪声协方差,经典值0.05;表示对测量数据的信任程度
} Kalman;

#ifdef __cplusplus
extern "C"
{
#endif

    void Kalman_Init(float LP, float Q, float R, Kalman *kfp);
    float KalmanFilter(Kalman *kfp, float input);

#ifdef __cplusplus
}
#endif

#endif /* INC_FILTER_H_ */
