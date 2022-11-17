#ifndef _TIM_H
#define _TIM_H

#include "sys.h"

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim14;

void MX_TIM4_Init(u16 arr, u16 psc);
void MX_TIM13_Init(u16 arr, u16 psc);
void MX_TIM14_Init(u16 arr, u16 psc);

#endif