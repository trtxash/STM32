#include "timer.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
// ALIENTEK STM32F407开发板
//定时器中断驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/4/7
//版本：V1.0
//版权所有，盗版必究。
// Copyright(C) 广州市星翼电子科技有限公司 2014-2024
// All rights reserved
//////////////////////////////////////////////////////////////////////////////////
//修改说明
// V1.1 20170407
//新增TIM5_PWM_Init函数,用于PWM输出
//////////////////////////////////////////////////////////////////////////////////

TIM_HandleTypeDef TIM3_Handler;      //定时器句柄
TIM_HandleTypeDef TIM5_Handler;      //定时器句柄
TIM_OC_InitTypeDef TIM5_CH1Handler;  //定时器5通道1句柄
TIM_HandleTypeDef TIM11_Handler;     //定时器句柄
TIM_OC_InitTypeDef TIM11_CH1Handler; //定时器11通道1句柄

//通用定时器3中断初始化
// arr：自动重装值。
// psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
// Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)(F407)
// F401 Timer clock is  APB1 clock.
void TIM3_Init(u16 arr, u16 psc)
{
    TIM3_Handler.Instance = TIM3;                             //通用定时器3
    TIM3_Handler.Init.Prescaler = psc;                        //分频系数
    TIM3_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       //向上计数器
    TIM3_Handler.Init.Period = arr;                           //自动装载值
    TIM3_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //时钟分频因子
    HAL_TIM_Base_Init(&TIM3_Handler);

    HAL_TIM_Base_Start_IT(&TIM3_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE
}

// TIM5 PWM部分初始化
// arr：自动重装值。
// psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
// Ft=定时器工作频率,单位:Mhz
void TIM5_PWM_Init(u16 arr, u16 psc)
{
    TIM5_Handler.Instance = TIM5;                             //定时器5
    TIM5_Handler.Init.Prescaler = psc;                        //定时器分频
    TIM5_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       //向上计数模式
    TIM5_Handler.Init.Period = arr;                           //自动重装载值
    TIM5_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //数字分频因子,数字分频因子为1时，TIMxCLK = PCLKx,其他值时，TIMxCLK = PCLKx/2^(数字分频因子)
    HAL_TIM_PWM_Init(&TIM5_Handler);                          //初始化PWM

    TIM5_CH1Handler.OCMode = TIM_OCMODE_PWM1;                                  //模式选择PWM1
    TIM5_CH1Handler.Pulse = arr / 2;                                           //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM5_CH1Handler.OCPolarity = TIM_OCPOLARITY_LOW;                           //输出比较极性为低
    HAL_TIM_PWM_ConfigChannel(&TIM5_Handler, &TIM5_CH1Handler, TIM_CHANNEL_1); //配置TIM5通道1
    HAL_TIM_PWM_ConfigChannel(&TIM5_Handler, &TIM5_CH1Handler, TIM_CHANNEL_3); //配置TIM5通3
    HAL_TIM_PWM_Start(&TIM5_Handler, TIM_CHANNEL_1);                           //开启PWM通道1
    HAL_TIM_PWM_Start(&TIM5_Handler, TIM_CHANNEL_3);                           //开启PWM通道3
}

// TIM11 PWM部分初始化
// arr：自动重装值。
// psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
// Ft=定时器工作频率,单位:Mhz
void TIM11_PWM_Init(u16 arr, u16 psc)
{
    TIM5_Handler.Instance = TIM11;                            //定时器11
    TIM5_Handler.Init.Prescaler = psc;                        //定时器分频
    TIM5_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       //向上计数模式
    TIM5_Handler.Init.Period = arr;                           //自动重装载值
    TIM5_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // 数字分频因子,数字分频因子为1时，TIMxCLK = PCLKx,其他值时，TIMxCLK = PCLKx/2^(数字分频因子)
    HAL_TIM_PWM_Init(&TIM11_Handler);                         //初始化PWM

    TIM5_CH1Handler.OCMode = TIM_OCMODE_PWM1;                                    //模式选择PWM1
    TIM5_CH1Handler.Pulse = arr / 2;                                             //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM5_CH1Handler.OCPolarity = TIM_OCPOLARITY_LOW;                             //输出比较极性为低
    HAL_TIM_PWM_ConfigChannel(&TIM11_Handler, &TIM11_CH1Handler, TIM_CHANNEL_1); //配置TIM11通道1

    HAL_TIM_PWM_Start(&TIM11_Handler, TIM_CHANNEL_1); //开启PWM通道1
}

//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE();           //使能TIM3时钟
        HAL_NVIC_SetPriority(TIM3_IRQn, 1, 3); //设置中断优先级，抢占优先级1，子优先级3
        HAL_NVIC_EnableIRQ(TIM3_IRQn);         //开启ITM3中断
    }
}

//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
// htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM5_CLK_ENABLE();  //使能定时器5
    __HAL_RCC_TIM11_CLK_ENABLE(); //使能定时器11
    __HAL_RCC_GPIOA_CLK_ENABLE(); //开启GPIOB时钟
    __HAL_RCC_GPIOB_CLK_ENABLE(); //开启GPIOB时钟

    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_2; // PA0
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;        //复用推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;            //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;       //高速
    GPIO_Initure.Alternate = GPIO_AF2_TIM5;     // PA0,2复用为AF2_TIM5
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);
}

//设置TIM通道1的占空比
// compare:比较值
void TIM_SetTIM5Compare_1(u32 compare)
{
    TIM5->CCR1 = compare;
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}

//回调函数，定时器中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&TIM3_Handler))
    {
        LED_Reverse();
    }
}
