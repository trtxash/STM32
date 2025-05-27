#ifndef _BSP_FREERTOS_H
#define _BSP_FREERTOS_H

// #include "bsp_app.h"

#include "bsp_app.h"

extern QueueHandle_t xQueue_KEY;
extern QueueHandle_t xSemaphore_ADC;

void freertos_main(void);

#endif
