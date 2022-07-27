#include "pid.h"

double P_V = 2.0000, I_V = 0.0050, D_V = 100.0; // PID constants
double P_X = 0.8000, I_X = 0.0050, D_X = 300.0; // PID constants

u8 move = 0;
u8 bluetooth = 0;
int pwmval_1;               // 定时器PWM占空比设置
int pwmval_2;               // 定时器PWM占空比设置
double Encoder_1;           // 当前1速度
double Encoder_2;           // 当前2速度
double Encoder_3;           // 当前3速度
double Encoder_4;           // 当前4速度
int Now_pos = 0;            // 当前位置
int Now_pos_num = 0;        // 当前位置数字
int Target_pos = 0;         // 目标位置
double TargetSpeed_1 = 0.0; // 目标速度
double TargetSpeed_2 = 0.0; // 目标速度
double TargetSpeed = 0.5;   // 目标和速度
double Angle_Target = 0.0;  // 目标角度

/**************************************************************************
函数功能：速度闭环PID控制
入口参数：目标速度 当前速度
返回  值：电压控制值
根据增量式离散PID公式
ControlVelocity+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差
e(k-1)代表上一次的偏差  以此类推
ControlVelocity代表增量输出
**************************************************************************/
int Velocity_FeedbackControl_1(double TargetVelocity, double CurrentVelocity)
{
    double Rate;                                                                          // 定义相关变量
    static double ControlVelocity;                                                        // 定义控制输出
    static double PID_OUT = 0;                                                            // PID输出
    static double P_OUT = 0, I_OUT = 0, D_OUT = 0;                                        //比例输出，积分输出，微分输出
    static double Current_Error = 0, Last_Error = 0;                                      //当前误差  最后误差
    static double Sum_Error = 0;                                                          //误差积分
    static double PID_I_MAX = 5.0, PID_I_MIN = -5.0, V_DATA_MAX = 100, V_DATA_MIN = -100; // PID积分上限，PID积分下限

    Current_Error = TargetVelocity - CurrentVelocity; //求速度偏差
    P_OUT = P_V * Current_Error;                      //比列项

    Sum_Error += Current_Error; //误差积分
    I_OUT = I_V * Sum_Error;    //积分项
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
    D_OUT = D_V * Rate;                // 微分输出

    PID_OUT = P_OUT + I_OUT + D_OUT; // PID输出

    ControlVelocity += PID_OUT;        // PID输出
    if (ControlVelocity >= V_DATA_MAX) // 如果输出大于最大值，则输出最大值
    {
        ControlVelocity = V_DATA_MAX;
    }
    if (ControlVelocity <= V_DATA_MIN) // 如果输出小于最小值，则输出最小值
    {
        ControlVelocity = V_DATA_MIN;
    }

    return ControlVelocity; //返回速度控制值
}

/**************************************************************************
函数功能：速度闭环PID控制
入口参数：目标速度 当前速度
返回  值：电压控制值
根据增量式离散PID公式
ControlVelocity+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差
e(k-1)代表上一次的偏差  以此类推
ControlVelocity代表增量输出
**************************************************************************/
int Velocity_FeedbackControl_2(double TargetVelocity, double CurrentVelocity)
{
    double Rate;                                                                          // 定义相关变量
    static double ControlVelocity;                                                        // 定义控制输出
    static double PID_OUT = 0;                                                            // PID输出
    static double P_OUT = 0, I_OUT = 0, D_OUT = 0;                                        //比例输出，积分输出，微分输出
    static double Current_Error = 0, Last_Error = 0;                                      //当前误差  最后误差
    static double Sum_Error = 0;                                                          //误差积分
    static double PID_I_MAX = 5.0, PID_I_MIN = -5.0, V_DATA_MAX = 100, V_DATA_MIN = -100; // PID积分上限，PID积分下限

    Current_Error = TargetVelocity - CurrentVelocity; //求速度偏差
    P_OUT = P_V * Current_Error;                      //比列项

    Sum_Error += Current_Error; //误差积分
    I_OUT = I_V * Sum_Error;    //积分项
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
    D_OUT = D_V * Rate;                // 微分输出

    PID_OUT = P_OUT + I_OUT + D_OUT; // PID输出

    ControlVelocity += PID_OUT;        // PID输出
    if (ControlVelocity >= V_DATA_MAX) // 如果输出大于最大值，则输出最大值
    {
        ControlVelocity = V_DATA_MAX;
    }
    if (ControlVelocity <= V_DATA_MIN) // 如果输出小于最小值，则输出最小值
    {
        ControlVelocity = V_DATA_MIN;
    }

    return ControlVelocity; //返回速度控制值
}

/**************************************************************************
函数功能：位置闭环PID控制
入口参数：目标位置 当前位置
返回  值：电压控制值
根据增量式离散PID公式
ControlVelocity+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差
e(k-1)代表上一次的偏差  以此类推
ControlVelocity代表增量输出
**************************************************************************/
int X_FeedbackControl_1(double TargetVelocity, double CurrentVelocity)
{
    double Rate;                                                                           // 定义相关变量
    static double ControlVelocity;                                                         // 定义控制输出
    static double PID_OUT = 0;                                                             // PID输出
    static double P_OUT = 0, I_OUT = 0, D_OUT = 0;                                         //比例输出，积分输出，微分输出
    static double Current_Error = 0, Last_Error = 0;                                       //当前误差  最后误差
    static double Sum_Error = 0;                                                           //误差积分
    static double PID_I_MAX = 15.0, PID_I_MIN = -15.0, V_DATA_MAX = 100, V_DATA_MIN = -50; // PID积分上限，PID积分下限

    Current_Error = TargetVelocity - CurrentVelocity; //求速度偏差
    P_OUT = P_X * Current_Error;                      //比列项

    Sum_Error += Current_Error; //误差积分
    I_OUT = I_X * Sum_Error;    //积分项
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
    D_OUT = D_X * Rate;                // 微分输出

    PID_OUT = P_OUT + I_OUT + D_OUT; // PID输出

    ControlVelocity += PID_OUT;        // PID输出
    if (ControlVelocity >= V_DATA_MAX) // 如果输出大于最大值，则输出最大值
    {
        ControlVelocity = V_DATA_MAX;
    }
    if (ControlVelocity <= V_DATA_MIN) // 如果输出小于最小值，则输出最小值
    {
        ControlVelocity = V_DATA_MIN;
    }

    return ControlVelocity; //返回速度控制值
}

/**************************************************************************
函数功能：位置闭环PID控制
入口参数：目标位置 当前位置
返回  值：电压控制值
根据增量式离散PID公式
ControlVelocity+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差
e(k-1)代表上一次的偏差  以此类推
ControlVelocity代表增量输出
**************************************************************************/
int X_FeedbackControl_2(double TargetVelocity, double CurrentVelocity)
{
    double Rate;                                                                           // 定义相关变量
    static double ControlVelocity;                                                         // 定义控制输出
    static double PID_OUT = 0;                                                             // PID输出
    static double P_OUT = 0, I_OUT = 0, D_OUT = 0;                                         //比例输出，积分输出，微分输出
    static double Current_Error = 0, Last_Error = 0;                                       //当前误差  最后误差
    static double Sum_Error = 0;                                                           //误差积分
    static double PID_I_MAX = 15.0, PID_I_MIN = -15.0, V_DATA_MAX = 100, V_DATA_MIN = -50; // PID积分上限，PID积分下限

    Current_Error = TargetVelocity - CurrentVelocity; //求速度偏差
    P_OUT = P_X * Current_Error;                      //比列项

    Sum_Error += Current_Error; //误差积分
    I_OUT = I_X * Sum_Error;    //积分项
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
    D_OUT = D_X * Rate;                // 微分输出

    PID_OUT = P_OUT + I_OUT + D_OUT; // PID输出

    ControlVelocity += PID_OUT;        // PID输出
    if (ControlVelocity >= V_DATA_MAX) // 如果输出大于最大值，则输出最大值
    {
        ControlVelocity = V_DATA_MAX;
    }
    if (ControlVelocity <= V_DATA_MIN) // 如果输出小于最小值，则输出最小值
    {
        ControlVelocity = V_DATA_MIN;
    }

    return ControlVelocity; //返回速度控制值
}