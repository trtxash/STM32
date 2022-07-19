#include "pid.h"

double P = 1.5000, I = 0.0050, D = 0.0; // PID constants

u16 pwmval_1;                // 定时器PWM占空比设置
u16 pwmval_2;                // 定时器PWM占空比设置
double Encoder_1;            // 当前1速度
double Encoder_2;            // 当前2速度
double TargetSpeed_1 = 0.25; // 目标速度
double TargetSpeed_2 = 0.25; // 目标速度

/**************************************************************************
函数功能：速度闭环PID控制(实际为PI控制)
入口参数：目标速度 当前速度
返回  值：速度控制值
根据增量式离散PID公式
ControlVelocity+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差
e(k-1)代表上一次的偏差  以此类推
ControlVelocity代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
ControlVelocity+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Velocity_FeedbackControl_1(double TargetVelocity, double CurrentVelocity)
{
    double Rate;                                                                          // 定义相关变量
    static double ControlVelocity;                                                        // 定义控制输出
    static double PID_OUT = 0;                                                            // PID输出
    static double P_OUT = 0, I_OUT = 0, D_OUT = 0;                                        //比例输出，积分输出，微分输出
    static double Current_Error = 0, Last_Error = 0;                                      //当前误差  最后误差
    static double Sum_Error = 0;                                                          //误差积分
    static double PID_I_MAX = 5.0, PID_I_MIN = -5.0, V_DATA_MAX = 20, V_DATA_MIN = -20; // PID积分上限，PID积分下限

    Current_Error = TargetVelocity - CurrentVelocity; //求速度偏差
    P_OUT = P * Current_Error;                        //比列项

    Sum_Error += Current_Error; //误差积分
    I_OUT = I * Sum_Error;      //积分项
    if (I_OUT > PID_I_MAX)      //积分限幅处理,不能超过最大值不能低于最小值
    {
        I_OUT = PID_I_MAX;
    }
    if (I_OUT < PID_I_MIN)
    {
        I_OUT = PID_I_MIN;
    }

    Rate = Current_Error - Last_Error; //变化速率计算
    Last_Error = Current_Error;        //存储误差分析
    D_OUT = D * Rate;                  // 微分输出

    PID_OUT = P_OUT + I_OUT + D_OUT; // PID输出
    if (PID_OUT >= V_DATA_MAX)       // 如果输出大于最大值，则输出最大值
    {
        PID_OUT = V_DATA_MAX;
    }
    if (PID_OUT <= V_DATA_MIN) // 如果输出小于最小值，则输出最小值
    {
        PID_OUT = V_DATA_MIN;
    }

    ControlVelocity += PID_OUT; // PID输出

    return ControlVelocity; //返回速度控制值
}

/**************************************************************************
函数功能：速度闭环PID控制(实际为PI控制)
入口参数：目标速度 当前速度
返回  值：速度控制值
根据增量式离散PID公式
ControlVelocity+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差
e(k-1)代表上一次的偏差  以此类推
ControlVelocity代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
ControlVelocity+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Velocity_FeedbackControl_2(double TargetVelocity, double CurrentVelocity)
{
    double Rate;                                                                          // 定义相关变量
    static double ControlVelocity;                                                        // 定义控制输出
    static double PID_OUT = 0;                                                            // PID输出
    static double P_OUT = 0, I_OUT = 0, D_OUT = 0;                                        //比例输出，积分输出，微分输出
    static double Current_Error = 0, Last_Error = 0;                                      //当前误差  最后误差
    static double Sum_Error = 0;                                                          //误差积分
    static double PID_I_MAX = 5.0, PID_I_MIN = -5.0, V_DATA_MAX = 20, V_DATA_MIN = -20; // PID积分上限，PID积分下限

    Current_Error = TargetVelocity - CurrentVelocity; //求速度偏差
    P_OUT = P * Current_Error;                        //比列项

    Sum_Error += Current_Error; //误差积分
    I_OUT = I * Sum_Error;      //积分项
    if (I_OUT > PID_I_MAX)      //积分限幅处理,不能超过最大值不能低于最小值
    {
        I_OUT = PID_I_MAX;
    }
    if (I_OUT < PID_I_MIN)
    {
        I_OUT = PID_I_MIN;
    }

    Rate = Current_Error - Last_Error; //变化速率计算
    Last_Error = Current_Error;        //存储误差分析
    D_OUT = D * Rate;                  // 微分输出

    PID_OUT = P_OUT + I_OUT + D_OUT; // PID输出
    if (PID_OUT >= V_DATA_MAX)       // 如果输出大于最大值，则输出最大值
    {
        PID_OUT = V_DATA_MAX;
    }
    if (PID_OUT <= V_DATA_MIN) // 如果输出小于最小值，则输出最小值
    {
        PID_OUT = V_DATA_MIN;
    }

    ControlVelocity += PID_OUT; // PID输出

    return ControlVelocity; //返回速度控制值
}