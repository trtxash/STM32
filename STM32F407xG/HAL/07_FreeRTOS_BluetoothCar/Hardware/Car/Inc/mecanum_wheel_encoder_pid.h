#ifndef _MECANUM_WHELL_ENCODER_PID_H
#define _MECANUM_WHELL_ENCODER_PID_H

#include "sys.h"

#define Encoder_NUM 4 // 编码器数量

extern float PID_V[3][Encoder_NUM];
extern short Encoder[Encoder_NUM];                // 当前编码器值
extern short Encoder_target[Encoder_NUM];         // 目标编码器值

short *Velocity_FeedbackControl(short *TargetVelocity, short *CurrentVelocity);

#endif // PID_H_INCLUDED