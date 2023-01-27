#include "mecanum_wheel_encoder_pid.h"

/* PID constants */
float PID_V[3][Encoder_NUM] = {0.05, 0.05, 0.05, 0.05,
                               0.0005, 0.0005, 0.0005, 0.0005,
                               0.00, 0.00, 0.00, 0.00};

u8 move = 0;
u8 bluetooth = 0;
short Encoder[Encoder_NUM] = {0};          // 当前编码器值
short Encoder_target[Encoder_NUM] = {0};   // 目标编码器值
float EncoderVelocity[Encoder_NUM];        // 当前编码器速度
float EncoderVelocity_target[Encoder_NUM]; // 目标编码器速度

/**************************************************************************
函数功能：速度闭环PID控制
入口参数：目标速度 当前速度,数组
返回  值：电压控制值数组地址
根据增量式离散PID公式
ControlVelocity[i]+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差
e(k-1)代表上一次的偏差  以此类推
ControlVelocity[i]代表增量输出
**************************************************************************/
short *Velocity_FeedbackControl(short *TargetVelocity, short *CurrentVelocity)
{
    u8 i;
    float Rate;                                                                                // 定义相关变量
    static short ControlVelocity[Encoder_NUM] = {0};                                           // 定义控制输出
    static float PID_OUT[Encoder_NUM] = {0};                                                   // PID输出
    static float P_OUT[Encoder_NUM] = {0}, I_OUT[Encoder_NUM] = {0}, D_OUT[Encoder_NUM] = {0}; // 比例输出，积分输出，微分输出
    static float Current_Error[Encoder_NUM] = {0}, Last_Error[Encoder_NUM] = {0};              // 当前误差  最后误差
    static float Sum_Error[Encoder_NUM] = {0};                                                 // 误差积分
    static float PID_I_MAX[Encoder_NUM] = {600.0, 600.0, 600.0, 600.0}, PID_I_MIN[Encoder_NUM] = {-600.0, -600.0, -600.0, -600.0},
                 V_DATA_MAX[Encoder_NUM] = {600, 600, 600, 600}, V_DATA_MIN[Encoder_NUM] = {-600, -600, -600, -600}; // PID积分上限，PID积分下限

    for (i = 0; i < Encoder_NUM; i++)
    {
        Current_Error[i] = TargetVelocity[i] - CurrentVelocity[i]; // 求速度偏差
        P_OUT[i] = PID_V[0][i] * Current_Error[i];                 // 比列项

        Sum_Error[i] += Current_Error[i];      // 误差积分
        I_OUT[i] = PID_V[1][i] * Sum_Error[i]; // 积分项
        if (I_OUT[i] > PID_I_MAX[i])           // 积分限幅处理,不能超过最大值不能低于最小值
        {
            I_OUT[i] = PID_I_MAX[i];
        }
        if (I_OUT[i] < PID_I_MIN[i])
        {
            I_OUT[i] = PID_I_MIN[i];
        }

        Rate = Current_Error[i] - Last_Error[i]; // 变化速率计算
        Last_Error[i] = Current_Error[i];        // 存储误差分析
        D_OUT[i] = PID_V[2][i] * Rate;           // 微分输出

        PID_OUT[i] = P_OUT[i] + I_OUT[i] + D_OUT[i]; // PID输出

        ControlVelocity[i] += (short)PID_OUT[i];        // PID输出
        if (ControlVelocity[i] >= V_DATA_MAX[i]) // 如果输出大于最大值，则输出最大值
        {
            ControlVelocity[i] = V_DATA_MAX[i];
        }
        if (ControlVelocity[i] <= V_DATA_MIN[i]) // 如果输出小于最小值，则输出最小值
        {
            ControlVelocity[i] = V_DATA_MIN[i];
        }
    }

    return ControlVelocity; // 返回速度控制值
}
