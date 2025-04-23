#ifndef __MECANUM_WHEEL_H
#define __MECANUM_WHEEL_H

#include "sys.h"
#include "tim.h"
#include "mecanum_wheel_encoder_pid.h"
#include "valuepack.h"

void Tim_Motor_Init(void);
void mecanum_wheel_pwm_set(void);
void mecanum_wheel_xyz_set(void);
void mecanum_wheel_stop(void);

#endif