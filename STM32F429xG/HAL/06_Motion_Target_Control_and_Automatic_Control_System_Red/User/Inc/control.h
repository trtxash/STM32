#ifndef _CONTROL_H
#define _CONTROL_H

#include "led.h"
#include "main.h"
#include "positional_pid.h"
#include "sys.h"
#include "usart.h"

extern positional_pid_params_t motor1_velocity;
extern positional_pid_params_t motor2_velocity;

extern u8 TASK, TASK_TEMP, Do_count;
extern u16 PWMA, PWMB;
extern u32 RED_XY[2], RED_XY_TEMP[2], STOP[2];
extern u32 TARGET_RED_XY[2], TARGET_GREEN_XY[2];
extern u16 XSET, YSET;
extern u8 finreset;
extern u16 HEIKUANG[8];
extern u16 HEIKUANG_TEMP[16];

void red_init(void);
void control_red(u8 kaiguan);
u8 redJabsl(u16 x, u16 y, u16 l);
void Set_angle(u16 pwm1, u16 pwm2);
u8 get_pi_xy(void);
void MAIN_TASK(void);
void M1M2RESET(void);
u8 time40sec(u8 sec);

#endif