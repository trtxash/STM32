#ifndef _KEY_H
#define _KEY_H
#include "sys.h"

/* KEY GPIO */
#define KEY0_Port GPIOE
#define KEY1_Port GPIOE

/* KEY CLK */
#define KEY0_Port_CLK_ENABLE() __HAL_RCC_GPIOE_CLK_ENABLE()
#define KEY1_Port_CLK_ENABLE() __HAL_RCC_GPIOE_CLK_ENABLE()

/* KEY PIN */
#define KEY0_PIN GPIO_PIN_2
#define KEY1_PIN GPIO_PIN_3

/* KEY 操作 */
#define KEY0_READ() KEY0_Port->IDR &KEY0_PIN // 0 or 1
#define KEY1_READ() KEY1_Port->IDR &KEY1_PIN // 0 or 1

void KEY0_Init(void);
void KEY1_Init(void);

#endif
