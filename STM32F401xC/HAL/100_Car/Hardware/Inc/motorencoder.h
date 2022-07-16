#ifndef __MOTORENCODER_H
#define __MOTORENCODER_H

#include "sys.h"
#include "timer.h"

/* extern表示该变量已在外部定义，所以要在外部定义它们 */
extern int MotorRun;    // 0: stop, 1: run
extern int Encoder; // 外部变量，当前速度
extern int TargetSpeed; // 外部变量，目标速度

extern float Kp; // 外部变量，PID参数
extern float Ki;
extern float Kd;

void MotorEncoder_Init(void);
u32 Read_Encoder(u8 tim);

#endif // __MOTORENCODER_H