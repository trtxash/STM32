#ifndef _PID_H
#define _PID_H

#include "sys.h"

int Velocity_FeedbackControl_1(double TargetVelocity, double CurrentVelocity);
int Velocity_FeedbackControl_2(double TargetVelocity, double CurrentVelocity);

#endif // PID_H_INCLUDED