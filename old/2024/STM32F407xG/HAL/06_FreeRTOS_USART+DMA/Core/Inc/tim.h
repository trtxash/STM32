#ifndef _TIM_H
#define _TIM_H

#include "sys.h"

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim11;
extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim14;

// extern volatile unsigned long long FreeRTOSRunTimeTicks;

void MX_TIM4_Init(u16 arr, u16 psc);
void MX_TIM5_Init(u32 arr, u16 psc);
void MX_TIM7_Init(u16 arr, u16 psc);
void MX_TIM11_Init(u16 arr, u16 psc);
void MX_TIM13_Init(u16 arr, u16 psc);
void MX_TIM14_Init(u16 arr, u16 psc);

void ConfigureTimerForTimeStats(void);
void ConfigureTimerForTask(void);

#endif