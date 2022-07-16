#include "motorencoder.h"

/**
 * @brief	编码器初始化函数
 * @param 	none
 * @arg		none
 * @note  	初始化编码器的定时器
 * @retval	none
 */
void MotorEncoder_Init(void)
{
    GPIO_InitTypeDef GPIO_InitTure; // GPIO初始化结构体
    // TIM_HandleTypeDef TIM2_Handler;     //定时器2句柄
    // TIM_OC_InitTypeDef TIM2_CHxHandler; //定时器2通道x句柄
    TIM_HandleTypeDef TIM3_Handler;               //定时器3句柄
    TIM_OC_InitTypeDef TIM3_CHxHandler;           //定时器3通道x句柄
    TIM_Encoder_InitTypeDef TIM_ENCODERMODE_TIM3; // TIM3编码器模式句柄

    __HAL_RCC_GPIOA_CLK_ENABLE(); //使能GPIOA时钟
    // __HAL_RCC_GPIOB_CLK_ENABLE(); //使能GPIOB时钟

    GPIO_InitTure.Pin = GPIO_PIN_6 | GPIO_PIN_7; // PA6和PA7
    GPIO_InitTure.Mode = GPIO_MODE_INPUT;        // 浮空输入模式，不用配置上下拉电阻，因为编码器有自己的上下拉电阻
    GPIO_InitTure.Speed = GPIO_SPEED_FREQ_HIGH;  // 高速模式
    HAL_GPIO_Init(GPIOB, &GPIO_InitTure);        // 先在上面四行设置GPIO的模式，上下拉，速度，再对GPIOB管脚初始化

    TIM3_Handler.Instance = TIM3;                             //通用定时器3
    TIM3_Handler.Init.Prescaler = 0;                          //分频系数
    TIM3_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       //向上计数器
    TIM3_Handler.Init.Period = 0XFFFF;                        //自动装载值
    TIM3_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //时钟分频因子
    HAL_TIM_Base_Init(&TIM3_Handler);
    // HAL_TIM_Base_Start_IT(&TIM3_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE

    TIM_ENCODERMODE_TIM3.EncoderMode = TIM_ENCODERMODE_TI12;                // TIM3编码器模式3，TI12模式,TI1和TI2同时计数
    TIM_ENCODERMODE_TIM3.IC1Polarity = TIM_ICPOLARITY_RISING;               // IC1上升沿触发
    TIM_ENCODERMODE_TIM3.IC1Selection = TIM_ICSELECTION_DIRECTTI;           // IC1使用直接输入模式
    TIM_ENCODERMODE_TIM3.IC1Prescaler = TIM_ICPSC_DIV1;                     // IC1输入比较分频系数为1
    TIM_ENCODERMODE_TIM3.IC1Filter = 10;                                    // IC1输入比较滤波器为10，即10个时钟周期内滤波，即滤波器的时间为10个时钟周期
    TIM_ENCODERMODE_TIM3.IC2Polarity = TIM_ICPOLARITY_RISING;               // IC2上升沿触发
    TIM_ENCODERMODE_TIM3.IC2Selection = TIM_ICSELECTION_DIRECTTI;           // IC2使用直接输入模式
    TIM_ENCODERMODE_TIM3.IC2Prescaler = TIM_ICPSC_DIV1;                     // IC2输入比较分频系数为1
    TIM_ENCODERMODE_TIM3.IC2Filter = 10;                                    // IC2输入比较滤波器为10，即10个时钟周期内滤波，即滤波器的时间为10个时钟周期
    HAL_TIM_Encoder_Init(&TIM3_Handler, &TIM_ENCODERMODE_TIM3);             // 初始化TIM3编码器模式3
    HAL_TIM_Encoder_Start_IT(&TIM3_Handler, TIM_CHANNEL_1 | TIM_CHANNEL_2); // 使能TIM3编码器模式3的通道1和通道2的中断：TIM_IT_CC1 | TIM_IT_CC2
}

/**
 * @brief	读取编码器数值函数
 * @param 	none
 * @arg		none
 * @note  	读取相关编码器的数值，现有TIM1~TIM5
 * @retval	int
 */
u32 Read_Encoder(u8 tim)
{
    u32 Encoder_TIM = 0;
    switch (tim) // 读取相关定时器编码器的数值
    {
    case 1:
        Encoder_TIM = TIM1->CNT;
        break;
    case 2:
        Encoder_TIM = TIM2->CNT;
        break;
    case 3:
        Encoder_TIM = TIM3->CNT;
        break;
    case 4:
        Encoder_TIM = TIM4->CNT;
        break;
    case 5:
        Encoder_TIM = TIM5->CNT;
        break;
    default:
        break;
    }
    if (Encoder_TIM > 0XFFFF)
    {
        Encoder_TIM = Encoder_TIM - 0XFFFF; // 转化计数器为有方向的值，正数表示正转，负数表示反转
                                            // CTN范围为0~0XFFFF，初值为0
    }
    switch (tim) // 读完清除相关定时器编码器的数值
    {
    case 1:
        TIM1->CNT = 0;
        break;
    case 2:
        TIM2->CNT = 0;
        break;
    case 3:
        TIM3->CNT = 0;
        break;
    case 4:
        TIM4->CNT = 0;
        break;
    case 5:
        TIM5->CNT = 0;
        break;
    default:
        break;
    }
    return Encoder_TIM;
}

// //定时器3中断服务函数
// void TIM3_IRQHandler(void)
// {
//     if (TIM3->SR & 0X0001) //检测定时器3更新中断标志
//     {
//         TIM3->SR &= ~0X0001; //清除定时器3更新中断标志
//     }
// }