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

void MIAN_TASK(void);
void GET_SET_AIM_ROOM(void);
void LOADORNOT(void);
u8 GET_NUM(void);
void Car_GO(short v);
void Car_TURN(float turn_angle);
void Car_STOP(void);
u8 J_turn(float angle);

#endif