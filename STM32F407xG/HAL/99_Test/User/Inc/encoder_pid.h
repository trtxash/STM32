#ifndef _ENCODER_PID_H
#define _ENCODER_PID_H

#include "sys.h"
#include "encoder.h"

#define Encoder_NUM 4 // 编码器数量

extern short Encoder[Encoder_NUM]; // 当前编码器速度

short Velocity_FeedbackControl_1(float TargetVelocity, float CurrentVelocity);
short Velocity_FeedbackControl_2(float TargetVelocity, float CurrentVelocity);
short Velocity_FeedbackControl_3(float TargetVelocity, float CurrentVelocity);
short Velocity_FeedbackControl_4(float TargetVelocity, float CurrentVelocity);
short X_FeedbackControl_1(float TargetVelocity, float CurrentVelocity);
short X_FeedbackControl_2(float TargetVelocity, float CurrentVelocity);

#endif // PID_H_INCLUDED