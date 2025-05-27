#include "filter.h"

Kalman kfp_x;
Kalman kfp_y;

void Kalman_Init()
{
    kfp_x.Last_P = 0.1;
    kfp_x.Now_P = 0;
    kfp_x.out = 0;
    kfp_x.Kg = 0;
    kfp_x.Q = 2.0;
    kfp_x.R = 0.05;

    kfp_y.Last_P = 0.1;
    kfp_y.Now_P = 0;
    kfp_y.out = 0;
    kfp_y.Kg = 0;
    kfp_y.Q = 2.0;
    kfp_y.R = 0.05;
}

/**
 *卡尔曼滤波器
 *@param 	Kalman *kfp 卡尔曼结构体参数
 *   			float input 需要滤波的参数的测量值（即传感器的采集值）
 *@return 滤波后的参数（最优值）
 */
float KalmanFilter(Kalman *kfp, float input)
{
    // 预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
    kfp->Now_P = kfp->Last_P + kfp->Q;
    // 卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
    kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
    // 更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
    kfp->out = kfp->out + kfp->Kg * (input - kfp->out); // 因为这一次的预测值就是上一次的输出值
    // 更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
    kfp->Last_P = (1 - kfp->Kg) * kfp->Now_P;
    return kfp->out;
}

/**************************************************************************
Function: Simple Kalman filter
Input   : acceleration、angular velocity
Output  : none
函数功能：获取x轴角度简易卡尔曼滤波
入口参数：加速度获取的角度、角速度
返回  值：x轴角速度
**************************************************************************/
float dt = 0.005; // 每1ms进行一次滤波
float Kalman_Filter_x(float Accel, float Gyro)
{
    static float angle_dot;
    static float angle;
    float Q_angle = 0.001; // 过程噪声的协方差
    float Q_gyro = 0.003;  // 0.003 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵
    float R_angle = 0.5;   // 测量噪声的协方差 既测量偏差
    char C_0 = 1;
    static float Q_bias, Angle_err;
    static float PCt_0, PCt_1, E;
    static float K_0, K_1, t_0, t_1;
    static float Pdot[4] = {0, 0, 0, 0};
    static float PP[2][2] = {{1, 0}, {0, 1}};
    angle += (Gyro - Q_bias) * dt;           // 先验估计
    Pdot[0] = Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分

    Pdot[1] = -PP[1][1];
    Pdot[2] = -PP[1][1];
    Pdot[3] = Q_gyro;
    PP[0][0] += Pdot[0] * dt; // Pk-先验估计误差协方差微分的积分
    PP[0][1] += Pdot[1] * dt; // =先验估计误差协方差
    PP[1][0] += Pdot[2] * dt;
    PP[1][1] += Pdot[3] * dt;

    Angle_err = Accel - angle; // zk-先验估计

    PCt_0 = C_0 * PP[0][0];
    PCt_1 = C_0 * PP[1][0];

    E = R_angle + C_0 * PCt_0;

    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;

    t_0 = PCt_0;
    t_1 = C_0 * PP[0][1];

    PP[0][0] -= K_0 * t_0; // 后验估计误差协方差
    PP[0][1] -= K_0 * t_1;
    PP[1][0] -= K_1 * t_0;
    PP[1][1] -= K_1 * t_1;

    angle += K_0 * Angle_err;  // 后验估计
    Q_bias += K_1 * Angle_err; // 后验估计
    angle_dot = Gyro - Q_bias; // 输出值(后验估计)的微分=角速度
    return angle;
}

/**************************************************************************
Function: First order complementary filtering
Input   : acceleration、angular velocity
Output  : none
函数功能：一阶互补滤波
入口参数：加速度获取的角度、角速度
返回  值：x轴角速度
**************************************************************************/
float Complementary_Filter_x(float angle_m, float gyro_m)
{
    static float angle;
    float K1 = 0.02;
    angle = K1 * angle_m + (1 - K1) * (angle + gyro_m * dt);
    return angle;
}

/**************************************************************************
Function: Simple Kalman filter
Input   : acceleration、angular velocity
Output  : none
函数功能：获取y轴角度简易卡尔曼滤波
入口参数：加速度获取的角度、角速度
返回  值：y轴角速度
**************************************************************************/
float Kalman_Filter_y(float Accel, float Gyro)
{
    static float angle_dot;
    static float angle;
    float Q_angle = 0.001; // 过程噪声的协方差
    float Q_gyro = 0.003;  // 0.003 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵
    float R_angle = 0.5;   // 测量噪声的协方差 既测量偏差
    char C_0 = 1;
    static float Q_bias, Angle_err;
    static float PCt_0, PCt_1, E;
    static float K_0, K_1, t_0, t_1;
    static float Pdot[4] = {0, 0, 0, 0};
    static float PP[2][2] = {{1, 0}, {0, 1}};
    angle += (Gyro - Q_bias) * dt;           // 先验估计
    Pdot[0] = Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分
    Pdot[1] = -PP[1][1];
    Pdot[2] = -PP[1][1];
    Pdot[3] = Q_gyro;
    PP[0][0] += Pdot[0] * dt; // Pk-先验估计误差协方差微分的积分
    PP[0][1] += Pdot[1] * dt; // =先验估计误差协方差
    PP[1][0] += Pdot[2] * dt;
    PP[1][1] += Pdot[3] * dt;
    Angle_err = Accel - angle; // zk-先验估计

    PCt_0 = C_0 * PP[0][0];
    PCt_1 = C_0 * PP[1][0];

    E = R_angle + C_0 * PCt_0;

    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;

    t_0 = PCt_0;
    t_1 = C_0 * PP[0][1];

    PP[0][0] -= K_0 * t_0; // 后验估计误差协方差
    PP[0][1] -= K_0 * t_1;
    PP[1][0] -= K_1 * t_0;
    PP[1][1] -= K_1 * t_1;

    angle += K_0 * Angle_err;  // 后验估计
    Q_bias += K_1 * Angle_err; // 后验估计
    angle_dot = Gyro - Q_bias; // 输出值(后验估计)的微分=角速度
    return angle;
}

/**************************************************************************
Function: First order complementary filtering
Input   : acceleration、angular velocity
Output  : none
函数功能：一阶互补滤波
入口参数：加速度获取的角度、角速度
返回  值：y轴角速度
**************************************************************************/
float Complementary_Filter_y(float angle_m, float gyro_m)
{
    static float angle;
    float K1 = 0.02;
    angle = K1 * angle_m + (1 - K1) * (angle + gyro_m * dt);
    return angle;
}
