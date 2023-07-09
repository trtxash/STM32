#ifndef _ULTRASONIC_H
#define _ULTRASONIC_H
#endif

#include "tim.h"
#include "delay.h"

#define Trig_Port GPIOA // 触发
#define Trig_Port_Clk_Enable() __HAL_RCC_GPIOA_CLK_ENABLE()
#define Trig_Pin GPIO_PIN_2

#define Trig_Clr() HAL_GPIO_WritePin(Trig_Port, Trig_Pin, GPIO_PIN_RESET)
#define Trig_Set() HAL_GPIO_WritePin(Trig_Port, Trig_Pin, GPIO_PIN_SET)

#define ULTRASONIC_TIM MX_TIM5_Init
#define sys_clock 240 // 240M

extern u8 ULTRASONIC_CAPTURE_STA; // 输入捕获状态
extern u32 ULTRASONIC_CAPTURE_VAL;	  // 输入捕获值 (TIM2/TIM5是 32位 )

void ultrasonic_init(void);
u32 get_capture_value(void);
float get_ultrasonic_value(void);
float ultrasonic_task(void);