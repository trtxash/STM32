#ifndef _BSP_FREERTOS_H
#define _BSP_FREERTOS_H

#include "delay.h"
#include "sys.h"

#include "FreeRTOS.h"
#include "task.h"

#include "led.h"

void freertos_main(void);

#endif