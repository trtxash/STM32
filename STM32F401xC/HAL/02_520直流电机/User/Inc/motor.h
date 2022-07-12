#ifndef _MOTOR_H
#define _MOTOR_H
#include <sys.h>
#include "delay.h"
// #include "main.h"

//定义定时器寄存器,便于程序的拓展和修改
#define LF1_CCR TIM2->CCR1
#define LF2_CCR TIM2->CCR2

#define RF1_CCR TIM2->CCR3
#define RF2_CCR TIM2->CCR4

#define LR1_CCR TIM3->CCR1
#define LR2_CCR TIM3->CCR2

#define RR1_CCR TIM3->CCR3
#define RR2_CCR TIM3->CCR4

//定义复用引脚功能
#define LF_GPIO_AF GPIO_AF1_TIM2
#define RF_GPIO_AF GPIO_AF1_TIM2
#define LR_GPIO_AF GPIO_AF2_TIM3
#define RR_GPIO_AF GPIO_AF2_TIM3

//定义定时器
#define LF_TIM htim2
#define RF_TIM htim2
#define LR_TIM htim3
#define RR_TIM htim3

//定义定时器通道
#define LF1_TIM_CHANNEL TIM_CHANNEL_1
#define LF2_TIM_CHANNEL TIM_CHANNEL_2
#define RF1_TIM_CHANNEL TIM_CHANNEL_3
#define RF2_TIM_CHANNEL TIM_CHANNEL_4

#define LR1_TIM_CHANNEL TIM_CHANNEL_1
#define LR2_TIM_CHANNEL TIM_CHANNEL_2
#define RR1_TIM_CHANNEL TIM_CHANNEL_3
#define RR2_TIM_CHANNEL TIM_CHANNEL_4
//定义引脚初始化操作
#define MOTOR_LF_F                                               \
    initGPIO_AF(MOTOR_LF1_GPIO_Port, MOTOR_LF1_Pin, LF_GPIO_AF); \
    initGPIO_OUTPUT(MOTOR_LF2_GPIO_Port, MOTOR_LF2_Pin);
#define MOTOR_LF_B                                       \
    initGPIO_OUTPUT(MOTOR_LF1_GPIO_Port, MOTOR_LF1_Pin); \
    initGPIO_AF(MOTOR_LF2_GPIO_Port, MOTOR_LF2_Pin, LF_GPIO_AF);
#define MOTOR_LF_S                                       \
    initGPIO_OUTPUT(MOTOR_LF1_GPIO_Port, MOTOR_LF1_Pin); \
    initGPIO_OUTPUT(MOTOR_LF2_GPIO_Port, MOTOR_LF2_Pin);

#define MOTOR_RF_F                                               \
    initGPIO_AF(MOTOR_RF1_GPIO_Port, MOTOR_RF1_Pin, RF_GPIO_AF); \
    initGPIO_OUTPUT(MOTOR_RF2_GPIO_Port, MOTOR_RF2_Pin);
#define MOTOR_RF_B                                       \
    initGPIO_OUTPUT(MOTOR_RF1_GPIO_Port, MOTOR_RF1_Pin); \
    initGPIO_AF(MOTOR_RF2_GPIO_Port, MOTOR_RF2_Pin, RF_GPIO_AF);
#define MOTOR_RF_S                                       \
    initGPIO_OUTPUT(MOTOR_RF1_GPIO_Port, MOTOR_RF1_Pin); \
    initGPIO_OUTPUT(MOTOR_RF2_GPIO_Port, MOTOR_RF2_Pin);

#define MOTOR_LR_F                                               \
    initGPIO_AF(MOTOR_LR1_GPIO_Port, MOTOR_LR1_Pin, LR_GPIO_AF); \
    initGPIO_OUTPUT(MOTOR_LR2_GPIO_Port, MOTOR_LR2_Pin);
#define MOTOR_LR_B                                       \
    initGPIO_OUTPUT(MOTOR_LR1_GPIO_Port, MOTOR_LR1_Pin); \
    initGPIO_AF(MOTOR_LR2_GPIO_Port, MOTOR_LR2_Pin, LR_GPIO_AF);
#define MOTOR_LR_S                                       \
    initGPIO_OUTPUT(MOTOR_LR1_GPIO_Port, MOTOR_LR1_Pin); \
    initGPIO_OUTPUT(MOTOR_LR2_GPIO_Port, MOTOR_LR2_Pin);

#define MOTOR_RR_F                                               \
    initGPIO_AF(MOTOR_RR1_GPIO_Port, MOTOR_RR1_Pin, RR_GPIO_AF); \
    initGPIO_OUTPUT(MOTOR_RR2_GPIO_Port, MOTOR_RR2_Pin);
#define MOTOR_RR_B                                       \
    initGPIO_OUTPUT(MOTOR_RR1_GPIO_Port, MOTOR_RR1_Pin); \
    initGPIO_AF(MOTOR_RR2_GPIO_Port, MOTOR_RR2_Pin, RR_GPIO_AF);
#define MOTOR_RR_S                                       \
    initGPIO_OUTPUT(MOTOR_RR1_GPIO_Port, MOTOR_RR1_Pin); \
    initGPIO_OUTPUT(MOTOR_RR2_GPIO_Port, MOTOR_RR2_Pin);

//从main.c中引入定时器实例
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

/*********函数定义****************/

//停止所有PWM通道的输出
void Motor_TIM_PWM_Stop(void);
//设置小车前进方向为向前
void CarTurnForward(void);
//设置小车前进方向为向后
void CarTurnBackward(void);
//设置小车前进方向为向左
void CarTurnLeft(void);
//设置小车前进方向为向右
void CarTurnRight(void);
//设置小车前进方向为向左前
void CarTurnLF(void);
//设置小车前进方向为向右前
void CarTurnRF(void);
//设置小车前进方向为向左后
void CarTurnLR(void);
//设置小车前进方向为向右后
void CarTurnRR(void);
//小车停止
void CarStop(void);

//通过设置比较值设置小车的速度
void CarForward(u32 valLeftFront, u32 valRightFront, u32 valLeftRear, u32 valRightRear);
void CarBack(u32 valLeftFront, u32 valRightFront, u32 valLeftRear, u32 valRightRear);
void CarLeft(u32 valLeftFront, u32 valRightFront, u32 valLeftRear, u32 valRightRear);
void CarRight(u32 valLeftFront, u32 valRightFront, u32 valLeftRear, u32 valRightRear);
void CarLF(u32 valRF, u32 valLR);
void CarRF(u32 valLF, u32 valRR);
void CarLR(u32 valLF, u32 valRR);
void CarRR(u32 valRF, u32 valLR);

//引脚初始化
void initGPIO_OUTPUT(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
void initGPIO_AF(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint8_t GPIO_AF);

//测试函数
void CarTest(void);

#endif
