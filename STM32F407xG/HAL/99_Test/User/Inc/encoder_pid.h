#ifndef _ENCODER_PID_H
#define _ENCODER_PID_H

#include "sys.h"
#include "encoder.h"

extern short Encoder_1;            // 当前1速度
extern short Encoder_2;            // 当前2速度
extern short Encoder_3;            // 当前3速度
extern short Encoder_4;            // 当前4速度

short Velocity_FeedbackControl_1(float TargetVelocity, float CurrentVelocity);
short Velocity_FeedbackControl_2(float TargetVelocity, float CurrentVelocity);
short Velocity_FeedbackControl_3(float TargetVelocity, float CurrentVelocity);
short Velocity_FeedbackControl_4(float TargetVelocity, float CurrentVelocity);
short X_FeedbackControl_1(float TargetVelocity, float CurrentVelocity);
short X_FeedbackControl_2(float TargetVelocity, float CurrentVelocity);

#endif // PID_H_INCLUDED