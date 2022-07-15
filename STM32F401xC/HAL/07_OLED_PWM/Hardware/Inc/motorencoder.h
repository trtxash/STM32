#ifndef __MOTORENCODER_H
#define __MOTORENCODER_H

#include "sys.h"

extern u32 MotorRun;    // 0: stop, 1: run
extern u32 Encoder;     // 外部变量，当前速度
extern u32 TargetSpeed; // 外部变量，目标速度

extern float Kp; // 外部变量，PID参数
extern float Ki;
extern float Kd;

void MotorEncoder_Init(void);
u32 Read_Encoder(u8 tim);

#endif // __MOTORENCODER_H