/**
 * @file	timer.h
 * @brief 	定时器设置
 *          适用STM32F401CCU6，F4一般都行，但具体定时器需要配置
 *          顺序为，句柄，定时器初始化，定时器PWM初始化，定时器底层驱动，定时器PWM底层驱动，定时器PWM占空比设置，中断服务，中断回调函数
 *          中断服务函数名可在 “startup_stm32f40_41xxx.s”文件中找到
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2022年7月16号15点32分
 */

#ifndef _TIMER_H
#define _TIMER_H

#include "sys.h"
#include "stdio.h"
#include "usart.h"
#include "oled.h"
#include "bspencoder.h"
#include "pid.h"
#include "infraredtobe.h"

extern u32 TIME_N5ms;
extern char infraredtobe_flag;
extern u8 infraredtobe_state;
extern char car_state; // -2:左转，-1:停止，1:直走，2:右转
extern u8 beep;
extern u8 beep_flag;
extern u8 move;
extern u8 bluetooth;
extern int pwmval_1;    // 定时器5PWM占空比设置
extern int pwmval_2;    // 定时器5PWM占空比设置
extern int pwmval_3;    // 定时器5PWM占空比设置
extern int pwmval_4;    // 定时器5PWM占空比设置
extern int Now_pos;     // 外部变量，当前位置
extern int Now_pos_num; // 外部变量，当前位置数字
extern int Target_pos;  // 外部变量，目标位置
extern short Encoder_target;
extern short Encoder_target_1;
extern short Encoder_target_2;
extern short Encoder_target_3;
extern short Encoder_target_4;
extern double TargetSpeed_1; // 目标速度
extern double TargetSpeed_2; // 目标速度
extern double TargetSpeed;   // 目标和速度
extern double Angle_Target;  // 目标角度
extern long rxIndex;         // 接收索引
extern unsigned int err;

void TIM1_Init(u16 arr, u16 psc);
void TIM2_Init(u16 arr, u16 psc);
void TIM3_Init(u16 arr, u16 psc);
void TIM4_Init(u16 arr, u16 psc);
void TIM4_PWM_Init(u16 arr, u16 psc, u8 ways);
void TIM5_PWM_Init(u16 arr, u16 psc, u8 ways);
void TIM_SetTIM4Compare_n(u32 compare, u8 n);
void TIM_SetTIM5Compare_n(u32 compare, u8 n);
void TIM_SetTIM4_DutyCycle_n(u8 DutyCycle, u8 n);
void TIM_SetTIM5_DutyCycle_n(u8 DutyCycle, u8 n);

#endif
