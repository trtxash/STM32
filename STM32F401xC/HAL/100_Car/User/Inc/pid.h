#ifndef _PID_H
#define _PID_H

#include "sys.h"

// float P = 0.800, I = 0.15, D = 300.0; // PID constants

// float Set_Temp = 60;                     //初始化P,I,D 当前值，设置值 常数
// float PID_OUT = 0, PWM_Duty = 0;         // PID输出
// float P_OUT = 0, I_OUT = 0, D_OUT = 0;   //比例输出，积分输出，微分输出
// float Current_Error = 0, Last_Error = 0; //当前误差  最后误差
// float Sum_Error = 0, Prev_Error = 0;     //误差积分
// float Gain = 1.2, PID_I_MAX = 100.0, PID_I_MIN = -100.0, V_DATA_MAX = 100, V_DATA_MIN = 0;

// float Set_Temp1 = 60;                      //初始化P,I,D，当前值，设置值 常数
// float PID_OUT1 = 0, PWM_Duty1 = 0;         // PID输出
// float P_OUT1 = 0, I_OUT1 = 0, D_OUT1 = 0;  //比例输出，积分输出，微分输出
// float Current_Error1 = 0, Last_Error1 = 0; //当前误差  最后误差
// float Sum_Error1 = 0, Prev_Error1 = 0;     //误差积分
// float Gain1 = 1.2, PID_I_MAX1 = 100.0, PID_I_MIN1 = -100.0, V_DATA_MAX1 = 100, V_DATA_MIN1 = 0;

// float Set_Temp2 = 60;                      //初始化P,I,D，当前值，设置值 常数
// float PID_OUT2 = 0, PWM_Duty2 = 0;         // PID输出
// float P_OUT2 = 0, I_OUT2 = 0, D_OUT2 = 0;  //比例输出，积分输出，微分输出
// float Current_Error2 = 0, Last_Error2 = 0; //当前误差  最后误差
// float Sum_Error2 = 0, Prev_Error2 = 0;     //误差积分
// float Gain2 = 1.2, PID_I_MAX2 = 100.0, PID_I_MIN2 = -100.0, V_DATA_MAX2 = 100, V_DATA_MIN2 = 0;

// float Set_Temp3 = 60;                      //初始化P,I,D，当前值，设置值 常数
// float PID_OUT3 = 0, PWM_Duty3 = 0;         // PID输出
// float P_OUT3 = 0, I_OUT3 = 0, D_OUT3 = 0;  //比例输出，积分输出，微分输出
// float Current_Error3 = 0, Last_Error3 = 0; //当前误差  最后误差
// float Sum_Error3 = 0, Prev_Error3 = 0;     //误差积分
// float Gain3 = 1.2, PID_I_MAX3 = 100.0, PID_I_MIN3 = -100.0, V_DATA_MAX3 = 100, V_DATA_MIN3 = 0;

void PID_calc(void);
int Velocity_FeedbackControl(double TargetVelocity, double CurrentVelocity);

#endif // PID_H_INCLUDED