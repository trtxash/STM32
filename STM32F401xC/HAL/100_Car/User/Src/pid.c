#include "pid.h"

void PID_calc(void) // PID算法
{
    // float Rate;                           //误差变化率
    // Current_Error = Set_Temp - Encoder_1; //当前误差
    // Sum_Error += Current_Error;           //误差积分
    // Prev_Error = Last_Error;              //存储误差积分
    // Last_Error = Current_Error;           //存储误差分析
    // Rate = Current_Error - Last_Error;    //变化速率计算

    // if (Rate > 10) //不让ta大于5也不让ta小于5
    //     Rate = 10;
    // if (Rate < -10)
    //     Rate = -10;

    // P_OUT = P * Gain * Current_Error; //比列项
    // I_OUT = I * Gain * Sum_Error;     //积分项

    // //积分限幅处理
    // if (I_OUT > PID_I_MAX)
    //     I_OUT = PID_I_MAX; //不能超过最大值不能低于最小值
    // if (I_OUT < PID_I_MIN)
    //     I_OUT = PID_I_MIN;

    // //微分输出处理
    // D_OUT = D * Gain * Rate;
    // PID_OUT = P_OUT + I_OUT + D_OUT;
    // if (PID_OUT >= V_DATA_MAX)
    //     PID_OUT = V_DATA_MAX;
    // if (PID_OUT <= V_DATA_MIN)
    //     PID_OUT = V_DATA_MIN;
    // /*******************************************************/
    // float Rate1;                            //误差变化率
    // Current_Error1 = Set_Temp1 - Encoder_2; //当前误差
    // Sum_Error1 += Current_Error1;           //误差积分
    // Prev_Error1 = Last_Error1;              //存储误差积分
    // Last_Error1 = Current_Error1;           //存储误差分析
    // Rate1 = Current_Error1 - Last_Error1;   //变化速率计算

    // if (Rate1 > 10) //不让ta大于5也不让ta小于5
    //     Rate1 = 10;
    // if (Rate1 < -10)
    //     Rate1 = -10;

    // P_OUT1 = P * Gain1 * Current_Error1; //比列项
    // I_OUT1 = I * Gain1 * Sum_Error1;     //积分项

    // //积分限幅处理
    // if (I_OUT1 > PID_I_MAX1)
    //     I_OUT1 = PID_I_MAX1; //不能超过最大值不能低于最小值
    // if (I_OUT1 < PID_I_MIN1)
    //     I_OUT1 = PID_I_MIN1;

    // //微分输出处理
    // D_OUT1 = D * Gain * Rate1;
    // PID_OUT1 = P_OUT1 + I_OUT1 + D_OUT1;
    // if (PID_OUT1 >= V_DATA_MAX1)
    //     PID_OUT1 = V_DATA_MAX1;
    // if (PID_OUT1 <= V_DATA_MIN1)
    //     PID_OUT1 = V_DATA_MIN1;
    // /*******************************************************/
    // float Rate2;                          //误差变化率
    // Current_Error2 = Set_Temp2 - V_3;     //当前误差
    // Sum_Error2 += Current_Error2;         //误差积分
    // Prev_Error2 = Last_Error2;            //存储误差积分
    // Last_Error2 = Current_Error2;         //存储误差分析
    // Rate2 = Current_Error2 - Last_Error2; //变化速率计算

    // if (Rate2 > 10) //不让ta大于5也不让ta小于5
    //     Rate2 = 10;
    // if (Rate2 < -10)
    //     Rate2 = -10;

    // P_OUT2 = P * Gain2 * Current_Error2; //比列项
    // I_OUT2 = I * Gain2 * Sum_Error2;     //积分项

    // //积分限幅处理
    // if (I_OUT2 > PID_I_MAX2)
    //     I_OUT2 = PID_I_MAX2; //不能超过最大值不能低于最小值
    // if (I_OUT2 < PID_I_MIN2)
    //     I_OUT2 = PID_I_MIN2;

    // //微分输出处理
    // D_OUT2 = D * Gain2 * Rate2;
    // PID_OUT2 = P_OUT2 + I_OUT2 + D_OUT2;
    // if (PID_OUT2 >= V_DATA_MAX2)
    //     PID_OUT2 = V_DATA_MAX2;
    // if (PID_OUT2 <= V_DATA_MIN2)
    //     PID_OUT2 = V_DATA_MIN2;
    // /*******************************************************/
    // float Rate3;                          //误差变化率
    // Current_Error3 = Set_Temp3 - V_4;     //当前误差
    // Sum_Error3 += Current_Error3;         //误差积分
    // Prev_Error3 = Last_Error3;            //存储误差积分
    // Last_Error3 = Current_Error3;         //存储误差分析
    // Rate3 = Current_Error3 - Last_Error3; //变化速率计算

    // if (Rate3 > 10) //不让ta大于5也不让ta小于5
    //     Rate3 = 10;
    // if (Rate3 < -10)
    //     Rate3 = -10;

    // P_OUT3 = P * Gain3 * Current_Error3; //比列项
    // I_OUT3 = I * Gain3 * Sum_Error3;     //积分项

    // //积分限幅处理
    // if (I_OUT3 > PID_I_MAX3)
    //     I_OUT3 = PID_I_MAX3; //不能超过最大值不能低于最小值
    // if (I_OUT3 < PID_I_MIN3)
    //     I_OUT3 = PID_I_MIN3;

    // //微分输出处理
    // D_OUT3 = D * Gain3 * Rate3;
    // PID_OUT3 = P_OUT3 + I_OUT3 + D_OUT3;
    // if (PID_OUT3 >= V_DATA_MAX3)
    //     PID_OUT3 = V_DATA_MAX3;
    // if (PID_OUT3 <= V_DATA_MIN3)
    //     PID_OUT3 = V_DATA_MIN3;
}