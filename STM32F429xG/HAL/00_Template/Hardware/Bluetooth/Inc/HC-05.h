#ifndef _HC_05_H
#define _HC_05_H

#include "sys.h"
#include "delay.h"

#define HC_05_STATE_Port GPIOG
#define HC_05_STATE_Port_Clk_Enable() __HAL_RCC_GPIOG_CLK_ENABLE()
#define HC_05_STATE_Pin GPIO_PIN_13

#define HC_05_EN_Port GPIOG
#define HC_05_EN_Port_Clk_Enable() __HAL_RCC_GPIOG_CLK_ENABLE()
#define HC_05_EN_Pin GPIO_PIN_12

#define HC_05_EN_Clr() HC_05_EN_Port->BSRR = (uint32_t)HC_05_EN_Pin << 16U
#define HC_05_EN_Set() HC_05_EN_Port->BSRR = HC_05_EN_Pin

#define HC_05_READ_STATE() HC_05_STATE_Port->IDR &HC_05_STATE_Pin // 0 or 1

void HC_05_init(void);

#endif