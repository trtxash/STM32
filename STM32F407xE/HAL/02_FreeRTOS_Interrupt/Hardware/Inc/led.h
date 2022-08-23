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
#define LED0_GPIO GPIOF
#define LED1_GPIO GPIOF

/* GPIO CLK */
#define LED0_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define LED1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()

/* GPIO PIN */
#define LED0_PIN GPIO_PIN_9
#define LED1_PIN GPIO_PIN_10

void LED_Init(void);
void LED0_Reverse(void);
void LED1_Reverse(void);

#endif