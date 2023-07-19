#ifndef _CONTROL_H
#define _CONTROL_H

#include "encoder.h"
#include "grayscale.h"
#include "key.h"
#include "led.h"
#include "main.h"
#include "mpu6050.h"
#include "positional_pid.h"
#include "sys.h"
#include "usart.h"

extern positional_pid_params_t motor1_velocity;
extern positional_pid_params_t motor2_velocity;
extern positional_pid_params_t motor12_location;
extern positional_pid_params_t motor_turn;

extern u8 SUM[4];           // 存的是字符
extern int TARGET_LOCATION; // 目标位置
extern short TARGET_V;
extern float TARGET_ANGLE;
extern float Yaw_Start;
extern u8 TASK;
extern u8 GET_ROOM_FLAG;
extern u8 FIND_ROOM_FLAG;
extern u8 LOAD_FLAG;
extern u8 AIM_SUM;
extern u8 AIM_PLACE;
extern u8 Do_count;
extern u8 STOP_FLAG, LINE_FLAG, TURN_SUCCEED_FLAG; // 初始车停下，不巡线
extern u8 LOR;

void MIAN_TASK(void);
u8 GET_SET_AIM_ROOM(void);
void LOADORNOT(void);
u8 GET_NUM(void);
void Car_GO(short v, int targetl);
void Car_TURN(float turn_angle);
void Car_STOP(void);
u8 J_turn(float angle);
u8 J_sum(u8 sum, u8 sec);

#endif