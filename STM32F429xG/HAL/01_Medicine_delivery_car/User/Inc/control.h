#ifndef _CONTROL_H
#define _CONTROL_H

#include "sys.h"
#include "usart.h"
#include "positional_pid.h"

extern positional_pid_params_t motor1_velocity;
extern positional_pid_params_t motor2_velocity;
extern positional_pid_params_t motor_turn;

extern short TARGET_V[2];

#endif