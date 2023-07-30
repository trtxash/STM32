#ifndef _TIM_H
#define _TIM_H

#include "sys.h"

extern TIM_HandleTypeDef htim2; // 编码器0
extern TIM_HandleTypeDef htim3; // 编码器1
extern TIM_HandleTypeDef htim5; // 两路PWM
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;

// extern volatile unsigned long long FreeRTOSRunTimeTicks;

void MX_TIM2_Init(u32 arr, u16 psc, u16 fil);
void MX_TIM3_Init(u16 arr, u16 psc, u16 fil);
void MX_TIM5_Init(u32 arr, u16 psc);
void MX_TIM6_Init(u16 arr, u16 psc);
void MX_TIM7_Init(u16 arr, u16 psc);

void ConfigureTimerForTimeStats(void);
void Tim_ConfigureTimerForTask(void);

#endif