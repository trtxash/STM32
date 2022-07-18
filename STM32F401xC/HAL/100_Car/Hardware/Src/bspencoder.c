#include "bspencoder.h"

/**
 * @brief  配置TIMx编码器模式
 * @param  无
 * @retval 无
 */
static void TIM_Encoder_Init(void)
{
    TIM_Encoder_InitTypeDef Encoder_ConfigStructure;
    TIM_HandleTypeDef TIM_EncoderHandle;
    TIM_Encoder_InitTypeDef Encoder_ConfigStructure_2;
    TIM_HandleTypeDef TIM_EncoderHandle_2;

    ENCODER_TIM_CLK_ENABLE();                                      /* 使能编码器接口时钟 */
    TIM_EncoderHandle.Instance = ENCODER_TIM;                      /* 定时器初始化设置 */
    TIM_EncoderHandle.Init.Prescaler = 0;                          //捕获预分频器的值为0时代表每捕获一个边沿便执行捕获
    TIM_EncoderHandle.Init.CounterMode = TIM_COUNTERMODE_UP;       //向上计
    TIM_EncoderHandle.Init.Period = ENCODER_TIM_PERIOD;            //捕获一个周期总次数为最大值65535
    TIM_EncoderHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //不分频
    TIM_EncoderHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    Encoder_ConfigStructure.EncoderMode = ENCODER_MODE;              /* 设置编码器倍频数 */
    Encoder_ConfigStructure.IC1Polarity = TIM_ICPOLARITY_RISING;     /* 编码器接口通道1设置 上升沿开始*/
    Encoder_ConfigStructure.IC1Selection = TIM_ICSELECTION_DIRECTTI; //编码器极性设置，没有反方向就不用管
    Encoder_ConfigStructure.IC1Prescaler = TIM_ICPSC_DIV1;           //不分频
    Encoder_ConfigStructure.IC1Filter = 10;                          //滤波器设置
    Encoder_ConfigStructure.IC2Polarity = TIM_ICPOLARITY_RISING;     /* 编码器接口通道2设置 */
    Encoder_ConfigStructure.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    Encoder_ConfigStructure.IC2Prescaler = TIM_ICPSC_DIV1;
    Encoder_ConfigStructure.IC2Filter = 10;                             //滤波器设置
    HAL_TIM_Encoder_Init(&TIM_EncoderHandle, &Encoder_ConfigStructure); /* 初始化编码器接口 */
    __HAL_TIM_SET_COUNTER(&TIM_EncoderHandle, 0);                       /* 清零计数器 */
    __HAL_TIM_CLEAR_IT(&TIM_EncoderHandle, TIM_IT_UPDATE);              /* 清零中断标志位 */
    // __HAL_TIM_ENABLE_IT(&TIM_EncoderHandle, TIM_IT_UPDATE);             /* 使能定时器的更新事件中断 */
    // __HAL_TIM_URS_ENABLE(&TIM_EncoderHandle);                           /* 设置更新事件请求源为：计数器溢出 */
    HAL_NVIC_SetPriority(ENCODER_TIM_IRQn, 1, 2);               /* 设置中断优先级 */
    HAL_NVIC_EnableIRQ(ENCODER_TIM_IRQn);                       /* 使能定时器中断 */
    HAL_TIM_Encoder_Start(&TIM_EncoderHandle, TIM_CHANNEL_ALL); /* 使能编码器接口 */

    ENCODER_TIM_CLK_ENABLE();                                        /* 使能编码器接口时钟 */
    TIM_EncoderHandle_2.Instance = ENCODER_TIM_2;                    /* 定时器初始化设置 */
    TIM_EncoderHandle_2.Init.Prescaler = 0;                          //捕获预分频器的值为0时代表每捕获一个边沿便执行捕获
    TIM_EncoderHandle_2.Init.CounterMode = TIM_COUNTERMODE_UP;       //向上计
    TIM_EncoderHandle_2.Init.Period = 65535;                         //捕获一个周期总次数为最大值65535
    TIM_EncoderHandle_2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //不分频
    TIM_EncoderHandle_2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    Encoder_ConfigStructure_2.EncoderMode = ENCODER_MODE_2;            /* 设置编码器倍频数 */
    Encoder_ConfigStructure_2.IC1Polarity = TIM_ICPOLARITY_RISING;     /* 编码器接口通道1设置 上升沿开始*/
    Encoder_ConfigStructure_2.IC1Selection = TIM_ICSELECTION_DIRECTTI; //编码器极性设置，没有反方向就不用管
    Encoder_ConfigStructure_2.IC1Prescaler = TIM_ICPSC_DIV1;           //不分频
    Encoder_ConfigStructure_2.IC1Filter = 10;                          //滤波器设置
    Encoder_ConfigStructure_2.IC2Polarity = TIM_ICPOLARITY_RISING;     /* 编码器接口通道2设置 */
    Encoder_ConfigStructure_2.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    Encoder_ConfigStructure_2.IC2Prescaler = TIM_ICPSC_DIV1;
    Encoder_ConfigStructure_2.IC2Filter = 10;                               //滤波器设置
    HAL_TIM_Encoder_Init(&TIM_EncoderHandle_2, &Encoder_ConfigStructure_2); /* 初始化编码器接口 */
    __HAL_TIM_SET_COUNTER(&TIM_EncoderHandle_2, 0);                         /* 清零计数器 */
    __HAL_TIM_CLEAR_IT(&TIM_EncoderHandle_2, TIM_IT_UPDATE);                /* 清零中断标志位 */
    // __HAL_TIM_ENABLE_IT(&TIM_EncoderHandle_2, TIM_IT_UPDATE);               /* 使能定时器的更新事件中断 */
    // __HAL_TIM_URS_ENABLE(&TIM_EncoderHandle_2);                             /* 设置更新事件请求源为：计数器溢出 */
    HAL_NVIC_SetPriority(ENCODER_TIM_IRQn_2, 1, 2);               /* 设置中断优先级 */
    HAL_NVIC_EnableIRQ(ENCODER_TIM_IRQn_2);                       /* 使能定时器中断 */
    HAL_TIM_Encoder_Start(&TIM_EncoderHandle_2, TIM_CHANNEL_ALL); /* 使能编码器接口 */
}

/**
 * @brief  编码器接口引脚初始化
 * @param  无
 * @retval 无
 */
static void Encoder_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitTypeDef GPIO_InitStruct_2 = {0};

    ENCODER_TIM_CH1_GPIO_CLK_ENABLE(); /* 定时器通道引脚端口时钟使能 */
    ENCODER_TIM_CH2_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;                     /* 设置输入类型 */
    GPIO_InitStruct.Pull = GPIO_NOPULL;                         /* 设置上拉 */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;               /* 设置引脚速率 */
    GPIO_InitStruct.Pin = ENCODER_TIM_CH1_PIN;                  /* 选择要控制的GPIO引脚 */
    GPIO_InitStruct.Alternate = ENCODER_TIM_CH1_GPIO_AF;        /* 设置复用 */
    HAL_GPIO_Init(ENCODER_TIM_CH1_GPIO_PORT, &GPIO_InitStruct); /* 调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO */
    GPIO_InitStruct.Pin = ENCODER_TIM_CH2_PIN;                  /* 选择要控制的GPIO引脚 */
    GPIO_InitStruct.Alternate = ENCODER_TIM_CH2_GPIO_AF;        /* 设置复用 */
    HAL_GPIO_Init(ENCODER_TIM_CH2_GPIO_PORT, &GPIO_InitStruct); /* 调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO */

    ENCODER_TIM_CH1_GPIO_CLK_ENABLE_2(); /* 定时器通道引脚端口时钟使能 */
    ENCODER_TIM_CH2_GPIO_CLK_ENABLE_2();
    GPIO_InitStruct_2.Mode = GPIO_MODE_AF_OD;                       /* 设置输入类型 */
    GPIO_InitStruct_2.Pull = GPIO_NOPULL;                           /* 设置上拉 */
    GPIO_InitStruct_2.Speed = GPIO_SPEED_FREQ_HIGH;                 /* 设置引脚速率 */
    GPIO_InitStruct_2.Pin = ENCODER_TIM_CH1_PIN_2;                  /* 选择要控制的GPIO引脚 */
    GPIO_InitStruct_2.Alternate = ENCODER_TIM_CH1_GPIO_AF_2;        /* 设置复用 */
    HAL_GPIO_Init(ENCODER_TIM_CH1_GPIO_PORT_2, &GPIO_InitStruct_2); /* 调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO */
    GPIO_InitStruct_2.Pin = ENCODER_TIM_CH2_PIN_2;                  /* 选择要控制的GPIO引脚 */
    GPIO_InitStruct_2.Alternate = ENCODER_TIM_CH2_GPIO_AF_2;        /* 设置复用 */
    HAL_GPIO_Init(ENCODER_TIM_CH2_GPIO_PORT_2, &GPIO_InitStruct_2); /* 调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO */
}

/**
 * @brief  编码器接口初始化
 * @param  无
 * @retval 无
 */
void Encoder_Init(void)
{
    Encoder_GPIO_Init(); /* 引脚初始化 */
    TIM_Encoder_Init();  /* 配置编码器接口 */
}

/**
 * @brief	读取编码器数值函数
 * @param 	none
 * @arg		none
 * @note  	读取相关编码器的数值，现有TIM1~TIM5
 * @retval	int
 */
int Read_Encoder(u8 tim)
{
    int Encoder_TIM = 0;
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
    if (Encoder_TIM > 0XEFFF)
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