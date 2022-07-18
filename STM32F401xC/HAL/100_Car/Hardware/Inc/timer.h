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
#include "oled.h"
#include "bspencoder.h"

void TIM3_Init(u16 arr, u16 psc);
void TIM4_Init(u16 arr, u16 psc);
void TIM5_PWM_Init(u16 arr, u16 psc, u8 ways);
void TIM_SetTIM5Compare_n(u32 compare, u8 n);

#endif
