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
#define LED0_GPIO GPIOE
#define LED1_GPIO GPIOD
#define LED2_GPIO GPIOG
#define LED3_GPIO GPIOA

/* GPIO CLK */
#define LED0_GPIO_CLK_ENABLE() __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED2_GPIO_CLK_ENABLE() __HAL_RCC_GPIOG_CLK_ENABLE()
#define LED3_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

/* GPIO PIN */
#define LED0_PIN GPIO_PIN_3
#define LED1_PIN GPIO_PIN_7
#define LED2_PIN GPIO_PIN_3
#define LED3_PIN GPIO_PIN_5

/* GPIO操作 */
#define LED0_Clr() LED0_GPIO->BSRR = (uint32_t)LED0_PIN << 16U
#define LED0_Set() LED0_GPIO->BSRR = LED0_PIN
#define LED1_Clr() LED1_GPIO->BSRR = (uint32_t)LED1_PIN << 16U
#define LED1_Set() LED1_GPIO->BSRR = LED1_PIN
#define LED2_Clr() LED2_GPIO->BSRR = (uint32_t)LED2_PIN << 16U
#define LED2_Set() LED2_GPIO->BSRR = LED2_PIN
#define LED3_Clr() LED3_GPIO->BSRR = (uint32_t)LED3_PIN << 16U
#define LED3_Set() LED3_GPIO->BSRR = LED3_PIN

void LED_Init(void);
void LED0_Init(void);
void LED1_Init(void);
void LED2_Init(void);
void LED3_Init(void);
void LED0_Reverse(void);
void LED1_Reverse(void);
void LED2_Reverse(void);
void LED3_Reverse(void);


#endif
