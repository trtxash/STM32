#ifndef _CONTROL_H
#define _CONTROL_H

#include "sys.h"
#include "usart.h"
#include "positional_pid.h"
#include "grayscale.h"
#include "mpu6050.h"
#include "encoder.h"
#include "led.h"

extern positional_pid_params_t motor1_velocity;
extern positional_pid_params_t motor2_velocity;
extern positional_pid_params_t motor_turn;

extern short TARGET_V[2];
extern float TARGET_ANGLE;

#endif