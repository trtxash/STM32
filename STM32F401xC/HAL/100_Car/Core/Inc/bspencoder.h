#ifndef __BSP_ENCOEDER_H
#define __BSP_ENCOEDER_H

#include "stm32f4xx.h"
#include "sys.h"

/* 定时器选择 */
#define ENCODER_TIM TIM3
#define ENCODER_TIM_CLK_ENABLE() __HAL_RCC_TIM3_CLK_ENABLE()

#define ENCODER_TIM_2 TIM4
#define ENCODER_TIM_CLK_ENABLE_2() __HAL_RCC_TIM4_CLK_ENABLE()

/* 定时器溢出值 */
#define ENCODER_TIM_PERIOD 0XFFFF

#define ENCODER_TIM_PERIOD_2 65535
/* 定时器预分频值 */
#define ENCODER_TIM_PRESCALER 0

#define ENCODER_TIM_PRESCALER_2 0

/* 定时器中断 */
#define ENCODER_TIM_IRQn TIM3_IRQn
#define ENCODER_TIM_IRQHandler TIM3_IRQHandler

#define ENCODER_TIM_IRQn_2 TIM4_IRQn
#define ENCODER_TIM_IRQHandler_2 TIM4_IRQHandler

/* 编码器接口引脚 */
#define ENCODER_TIM_CH1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define ENCODER_TIM_CH1_GPIO_PORT GPIOA
#define ENCODER_TIM_CH1_PIN GPIO_PIN_6
#define ENCODER_TIM_CH1_GPIO_AF GPIO_AF2_TIM3
#define ENCODER_TIM_CH2_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define ENCODER_TIM_CH2_GPIO_PORT GPIOA
#define ENCODER_TIM_CH2_PIN GPIO_PIN_7
#define ENCODER_TIM_CH2_GPIO_AF GPIO_AF2_TIM3

#define ENCODER_TIM_CH1_GPIO_CLK_ENABLE_2() __HAL_RCC_GPIOB_CLK_ENABLE()
#define ENCODER_TIM_CH1_GPIO_PORT_2 GPIOB
#define ENCODER_TIM_CH1_PIN_2 GPIO_PIN_6
#define ENCODER_TIM_CH1_GPIO_AF_2 GPIO_AF2_TIM4
#define ENCODER_TIM_CH2_GPIO_CLK_ENABLE_2() __HAL_RCC_GPIOB_CLK_ENABLE()
#define ENCODER_TIM_CH2_GPIO_PORT_2 GPIOB
#define ENCODER_TIM_CH2_PIN_2 GPIO_PIN_7
#define ENCODER_TIM_CH2_GPIO_AF_2 GPIO_AF2_TIM4

/* 编码器接口倍频数 */
#define ENCODER_MODE TIM_ENCODERMODE_TI12

#define ENCODER_MODE_2 TIM_ENCODERMODE_TI12

/* 编码器接口输入捕获通道相位设置 */
#define ENCODER_IC1_POLARITY TIM_ICPOLARITY_RISING
#define ENCODER_IC2_POLARITY_2 TIM_ICPOLARITY_RISING

#define ENCODER_IC1_POLARITY TIM_ICPOLARITY_RISING
#define ENCODER_IC2_POLARITY_2 TIM_ICPOLARITY_RISING

/* 编码器物理分辨率 */
#define ENCODER_RESOLUTION 16

/* 经过倍频之后的总分辨率 */
#if (ENCODER_MODE == TIM_ENCODERMODE_TI12)
#define ENCODER_TOTAL_RESOLUTION (ENCODER_RESOLUTION * 4) /* 4倍频后的总分辨率 */
#else
#define ENCODER_TOTAL_RESOLUTION (ENCODER_RESOLUTION * 2) /* 2倍频后的总分辨率 */
#endif

/* 减速电机减速比 */
#define REDUCTION_RATIO 30

extern __IO int16_t Encoder_Overflow_Count;
// extern TIM_HandleTypeDef TIM_EncoderHandle;
// extern TIM_HandleTypeDef TIM_EncoderHandle_2;

/* extern表示该变量已在外部定义，所以要在外部定义它们 */
extern u8 MotorRun;    // 0: stop, 1: run
extern s16 Encoder_1;   // 外部变量，当前1速度
extern s16 Encoder_2;   // 外部变量，当前2速度
extern s16 TargetSpeed; // 外部变量，目标速度

extern float Kp; // 外部变量，PID参数
extern float Ki;
extern float Kd;

void Encoder_Init(void);
int Read_Encoder(u8 tim);

#endif /* __BSP_ENCODER_H */