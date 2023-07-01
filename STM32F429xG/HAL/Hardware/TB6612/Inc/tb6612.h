#ifndef __TB6612_H_
#define __TB6612_H_

#include "sys.h"
#include "tim.h"

#define TB6612_AIN1_Port GPIOB
#define TB6612_AIN1_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define TB6612_AIN1_Pin GPIO_PIN_14

#define TB6612_AIN2_Port GPIOB
#define TB6612_AIN2_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define TB6612_AIN2_Pin GPIO_PIN_15

#define TB6612_BIN1_Port GPIOB
#define TB6612_BIN1_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define TB6612_BIN1_Pin GPIO_PIN_13

#define TB6612_BIN2_Port GPIOB
#define TB6612_BIN2_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define TB6612_BIN2_Pin GPIO_PIN_12

#define TB6612_AIN1_Clr() TB6612_AIN1_Port->BSRR = (uint32_t)TB6612_AIN1_Pin << 16U
#define TB6612_AIN1_Set() TB6612_AIN1_Port->BSRR = TB6612_AIN1_Pin

#define TB6612_AIN2_Clr() TB6612_AIN2_Port->BSRR = (uint32_t)TB6612_AIN2_Pin << 16U
#define TB6612_AIN2_Set() TB6612_AIN2_Port->BSRR = TB6612_AIN2_Pin

#define TB6612_BIN1_Clr() TB6612_BIN1_Port->BSRR = (uint32_t)TB6612_BIN1_Pin << 16U
#define TB6612_BIN1_Set() TB6612_BIN1_Port->BSRR = TB6612_BIN1_Pin

#define TB6612_BIN2_Clr() TB6612_BIN2_Port->BSRR = (uint32_t)TB6612_BIN2_Pin << 16U
#define TB6612_BIN2_Set() TB6612_BIN2_Port->BSRR = TB6612_BIN2_Pin

void TB6612_init(void);

#endif