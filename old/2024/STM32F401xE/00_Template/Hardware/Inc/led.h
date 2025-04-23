#ifndef __LED_H
#define __LED_H
#include "delay.h"
#include "sys.h"
/**
 * @file	led.h
 * @brief 	LED灯交替闪烁
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2022年7月12号12点45分
 */

/* LED GPIO */
#define LED0_GPIO GPIOA
#define LED1_GPIO GPIOA

/* GPIO CLK */
#define LED0_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

/* GPIO PIN */
#define LED0_PIN GPIO_PIN_1
#define LED1_PIN GPIO_PIN_0

void LED_Init(void);
void LED0_Reverse(void);
void LED1_Reverse(void);

#endif