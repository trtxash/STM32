#ifndef _ENCODER_PID_H
#define _ENCODER_PID_H

#include "sys.h"

int Velocity_FeedbackControl_1(double TargetVelocity, double CurrentVelocity);
int Velocity_FeedbackControl_2(double TargetVelocity, double CurrentVelocity);
int Velocity_FeedbackControl_3(double TargetVelocity, double CurrentVelocity);
int Velocity_FeedbackControl_4(double TargetVelocity, double CurrentVelocity);
int X_FeedbackControl_1(double TargetVelocity, double CurrentVelocity);
int X_FeedbackControl_2(double TargetVelocity, double CurrentVelocity);

#endif // PID_H_INCLUDED