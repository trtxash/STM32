#ifndef _KEY_H
#define _KEY_H

#include "main.h"

/* KEY GPIO */
#define KEY_UP_Port GPIOA
/* KEY CLK */
#define KEY_UP_Port_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
/* KEY PIN */
#define KEY_UP_PIN GPIO_PIN_0
/* KEY 操作 */
#define KEY_UP_READ() (KEY_UP_Port->IDR & KEY_UP_PIN) // 0 or 1

void KEY_Init(void);

#endif
