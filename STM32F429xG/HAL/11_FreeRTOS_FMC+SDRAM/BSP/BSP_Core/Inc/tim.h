#ifndef _TIM_H
#define _TIM_H

#include "sys.h"

extern TIM_HandleTypeDef htim14;

#if SYSTEM_SUPPORT_OS
extern volatile unsigned long long FreeRTOSRunTimeTicks;
#endif

void TIM14_Init(uint16_t arr, uint16_t psc);
void ConfigureTimerForTimeStats(void);

#endif