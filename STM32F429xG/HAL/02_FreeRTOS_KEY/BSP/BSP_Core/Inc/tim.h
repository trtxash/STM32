#ifndef _TIM_H
#define _TIM_H

#include "sys.h"

extern TIM_HandleTypeDef htim14;

#if SYSTEM_SUPPORT_OS
extern volatile uint32_t FreeRTOSRunTimeTicks;
#endif

void TIM14_Init(uint16_t arr, uint16_t psc);
void ConfigureTimerForTimeStats(void);

#endif
