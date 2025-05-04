#ifndef __TB6612_H_
#define __TB6612_H_

#include "sys.h"
#include "tim.h"

#define TB6612_AIN1_Port GPIOC
#define TB6612_AIN1_Port_Clk_Enable() __HAL_RCC_GPIOC_CLK_ENABLE()
#define TB6612_AIN1_Pin GPIO_PIN_1

#define TB6612_AIN2_Port GPIOC
#define TB6612_AIN2_Port_Clk_Enable() __HAL_RCC_GPIOC_CLK_ENABLE()
#define TB6612_AIN2_Pin GPIO_PIN_13

#define TB6612_BIN1_Port GPIOC
#define TB6612_BIN1_Port_Clk_Enable() __HAL_RCC_GPIOC_CLK_ENABLE()
#define TB6612_BIN1_Pin GPIO_PIN_5

#define TB6612_BIN2_Port GPIOD
#define TB6612_BIN2_Port_Clk_Enable() __HAL_RCC_GPIOD_CLK_ENABLE()
#define TB6612_BIN2_Pin GPIO_PIN_6

#define TB6612_AIN1_Clr() TB6612_AIN1_Port->BSRR = (uint32_t)TB6612_AIN1_Pin << 16U
#define TB6612_AIN1_Set() TB6612_AIN1_Port->BSRR = TB6612_AIN1_Pin

#define TB6612_AIN2_Clr() TB6612_AIN2_Port->BSRR = (uint32_t)TB6612_AIN2_Pin << 16U
#define TB6612_AIN2_Set() TB6612_AIN2_Port->BSRR = TB6612_AIN2_Pin

#define TB6612_BIN1_Clr() TB6612_BIN1_Port->BSRR = (uint32_t)TB6612_BIN1_Pin << 16U
#define TB6612_BIN1_Set() TB6612_BIN1_Port->BSRR = TB6612_BIN1_Pin

#define TB6612_BIN2_Clr() TB6612_BIN2_Port->BSRR = (uint32_t)TB6612_BIN2_Pin << 16U
#define TB6612_BIN2_Set() TB6612_BIN2_Port->BSRR = TB6612_BIN2_Pin

#define TB6612_TIM htim5

void TB6612_init(void);
void TB6612_control_speed(int arr_r, int arr_l);

#endif