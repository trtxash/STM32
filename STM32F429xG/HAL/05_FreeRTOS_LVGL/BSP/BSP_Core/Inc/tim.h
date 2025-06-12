#ifndef _TIM_H
#define _TIM_H

#include "sys.h"

extern TIM_HandleTypeDef htim14;

#if SYSTEM_SUPPORT_OS
extern TIM_HandleTypeDef htim7;
extern volatile uint32_t FreeRTOSRunTimeTicks;
#endif

void TIM14_Init(uint16_t arr, uint16_t psc);

#if SYSTEM_SUPPORT_OS
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority);
void HAL_SuspendTick(void);
void HAL_ResumeTick(void);
void ConfigureTimerForTimeStats(void);
#endif

#endif
