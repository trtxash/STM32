#ifndef _TIM_H
#define _TIM_H

#include "sys.h"

extern TIM_HandleTypeDef htim1; // 4通道PWM
extern TIM_HandleTypeDef htim2; // 编码器0
extern TIM_HandleTypeDef htim3; // 编码器1
extern TIM_HandleTypeDef htim4; // 编码器2
extern TIM_HandleTypeDef htim5; // 编码器3
extern TIM_HandleTypeDef htim6; // USMART
extern TIM_HandleTypeDef htim7; // 定时任务
extern TIM_HandleTypeDef htim8; // 4通道PWM
extern TIM_HandleTypeDef htim11;
extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim14; // FreeRTOS任务计时

// extern volatile unsigned long long FreeRTOSRunTimeTicks;

void MX_TIM1_Init(u16 arr, u16 psc);
void MX_TIM2_Init(u32 arr, u16 psc, u16 fil);
void MX_TIM3_Init(u16 arr, u16 psc, u16 fil);
void MX_TIM4_Init(u16 arr, u16 psc, u16 fil);
void MX_TIM5_Init(u32 arr, u16 psc, u16 fil);
void MX_TIM6_Init(u16 arr, u16 psc);
void MX_TIM7_Init(u16 arr, u16 psc);
void MX_TIM8_Init(u16 arr, u16 psc);
void MX_TIM11_Init(u16 arr, u16 psc);
void MX_TIM13_Init(u16 arr, u16 psc);
void MX_TIM14_Init(u16 arr, u16 psc);

void ConfigureTimerForTimeStats(void);
void Tim_ConfigureTimerForTask(void);

#endif