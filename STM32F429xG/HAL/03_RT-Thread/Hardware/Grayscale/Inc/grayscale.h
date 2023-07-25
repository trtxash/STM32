#ifndef _GRAYSCALE_H
#define _GRAYSCALE_H

#include "sys.h"

#define GRAYSCALE0_Port GPIOC
#define GRAYSCALE0_Port_Clk_Enable() __HAL_RCC_GPIOC_CLK_ENABLE()
#define GRAYSCALE0_Pin GPIO_PIN_4
#define GRAYSCALE0_READ() GRAYSCALE0_Port->IDR &GRAYSCALE0_Pin // 0 or 1

#define GRAYSCALE1_Port GPIOB
#define GRAYSCALE1_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define GRAYSCALE1_Pin GPIO_PIN_0
#define GRAYSCALE1_READ() GRAYSCALE1_Port->IDR &GRAYSCALE1_Pin // 0 or 1

#define GRAYSCALE2_Port GPIOB
#define GRAYSCALE2_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define GRAYSCALE2_Pin GPIO_PIN_9
#define GRAYSCALE2_READ() GRAYSCALE2_Port->IDR &GRAYSCALE2_Pin // 0 or 1

#define GRAYSCALE3_Port GPIOA
#define GRAYSCALE3_Port_Clk_Enable() __HAL_RCC_GPIOA_CLK_ENABLE()
#define GRAYSCALE3_Pin GPIO_PIN_4
#define GRAYSCALE3_READ() GRAYSCALE3_Port->IDR &GRAYSCALE3_Pin // 0 or 1

#define GRAYSCALE4_Port GPIOD
#define GRAYSCALE4_Port_Clk_Enable() __HAL_RCC_GPIOD_CLK_ENABLE()
#define GRAYSCALE4_Pin GPIO_PIN_4
#define GRAYSCALE4_READ() GRAYSCALE4_Port->IDR &GRAYSCALE4_Pin // 0 or 1

#define GRAYSCALE5_Port GPIOG
#define GRAYSCALE5_Port_Clk_Enable() __HAL_RCC_GPIOG_CLK_ENABLE()
#define GRAYSCALE5_Pin GPIO_PIN_11
#define GRAYSCALE5_READ() GRAYSCALE5_Port->IDR &GRAYSCALE5_Pin // 0 or 1

#define GRAYSCALE6_Port GPIOB
#define GRAYSCALE6_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define GRAYSCALE6_Pin GPIO_PIN_9
#define GRAYSCALE6_READ() GRAYSCALE6_Port->IDR &GRAYSCALE6_Pin // 0 or 1

extern u8 Grayscale_Val[5];
extern u8 Grayscale_truesum;

void Grayscale_Init(void);
void Get_Grayscale_Val(void);

#endif