#ifndef __GPIOTEST_H
#define __GPIOTEST_H

#include "sys.h"
#include "delay.h"

#define GPIO_BASE_PWM_FREQ 10000

void GPIOTest_Init(void);
void GPIOpwmout(void);

#endif