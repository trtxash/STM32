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
#define LED0_PIN GPIO_PIN_6
#define LED1_PIN GPIO_PIN_7

/* GPIO操作 */
#define LED0_Clr() LED0_GPIO->BSRR = (uint32_t)LED0_PIN << 16U
#define LED0_Set() LED0_GPIO->BSRR = LED0_PIN
#define LED1_Clr() LED1_GPIO->BSRR = (uint32_t)LED1_PIN << 16U
#define LED1_Set() LED1_GPIO->BSRR = LED1_PIN

void LED0_Init(void);
void LED1_Init(void);
void LED0_Reverse(void);
void LED1_Reverse(void);

#endif