#ifndef _BSP_FREERTOS_H
#define _BSP_FREERTOS_H

#include "bsp_app.h"

#include "FreeRTOS.h"
#include "task.h"

#include "led.h"
#include "sdram.h"

void freertos_main(void);

#endif
