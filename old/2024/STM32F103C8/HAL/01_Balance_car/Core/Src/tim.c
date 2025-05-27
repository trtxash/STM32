/**
 * @file	tim.c
 * @brief 	定时器设置
 *          适用STM32F4，但具体定时器需要配置
 *          顺序为，句柄，定时器初始化，定时器PWM初始化，定时器底层驱动，定时器PWM底层驱动，定时器PWM占空比设置，中断服务，中断回调函数
 *          中断服务函数名可在 “startup_stm32f40_41xxx.s”文件中找到
 *          中断定义在stm32f4xx_it.c中
 * 			USMART用了TIM6，定时任务用了TIM17,FreeRTOS任务计时用了Tim14
 * @author 	TRTX-gamer
 * @version 1.05
 * @date 	2023年1月19号23点42分
 */
#include "tim.h"

TIM_HandleTypeDef htim1; // 4通道PWM
TIM_HandleTypeDef htim2; // 编码器0
TIM_HandleTypeDef htim4; // 编码器1

volatile unsigned long long FreeRTOSRunTimeTicks; // 易变量，FreeRTOS运行计时

extern uint32_t SystemCoreClock; // 系统时钟频率，初始化后发生变化

/**
 * @brief	TIM1 Initialization Function
 * @note	定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
 * @param 	arr-Period
 * 			psc-Prescaler
 * @retval 	None
 */
void MX_TIM1_Init(u16 arr, u16 psc)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

    htim1.Instance = TIM1;
    htim1.Init.Prescaler = psc;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = arr;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
    {
        Error_Handler();
    }
    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime = 0;
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
    {
        Error_Handler();
    }
    HAL_TIM_MspPostInit(&htim1);
}

/**
 * @brief	TIM2 Initialization Function
 * @note	定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
 * @param 	arr-Period
 * 			psc-Prescaler
 * 			fil-IC1Filter and IC2Filter
 * @retval 	None
 */
void MX_TIM2_Init(u32 arr, u16 psc, u16 fil)
{

    /* USER CODE BEGIN TIM2_Init 0 */

    /* USER CODE END TIM2_Init 0 */

    TIM_Encoder_InitTypeDef sConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM2_Init 1 */

    /* USER CODE END TIM2_Init 1 */
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = psc; // 捕获预分频器的值为0时代表每捕获一个边沿便执行捕获
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = arr; // 捕获一个周期总次数为最大值65535
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    sConfig.EncoderMode = TIM_ENCODERMODE_TI12; // 设置编码器倍频数
    sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC1Filter = fil; // 滤波器1设置
    sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC2Filter = fil; // 滤波器2设置
    if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM2_Init 2 */

    /* USER CODE END TIM2_Init 2 */
}

/**
 * @brief	TIM3 Initialization Function
 * @note	定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
 * @param 	arr-Period
 * 			psc-Prescaler
 * 			fil-IC1Filter and IC2Filter
 * @retval 	None
 */
void MX_TIM3_Init(u16 arr, u16 psc, u16 fil)
{

    /* USER CODE BEGIN TIM3_Init 0 */

    /* USER CODE END TIM3_Init 0 */

    TIM_Encoder_InitTypeDef sConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM3_Init 1 */

    /* USER CODE END TIM3_Init 1 */
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = psc; // 捕获预分频器的值为0时代表每捕获一个边沿便执行捕获
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = arr; // 捕获一个周期总次数为最大值65535
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    sConfig.EncoderMode = TIM_ENCODERMODE_TI12; // 设置编码器倍频数
    sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC1Filter = fil; // 滤波器1设置
    sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC2Filter = fil; // 滤波器2设置
    if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM3_Init 2 */

    /* USER CODE END TIM3_Init 2 */
}

/**
 * @brief	TIM4 Initialization Function
 * @note	定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
 * @param 	arr-Period
 * 			psc-Prescaler
 * 			fil-IC1Filter and IC2Filter
 * @retval 	None
 */
void MX_TIM4_Init(u16 arr, u16 psc, u16 fil)
{

    /* USER CODE BEGIN TIM4_Init 0 */

    /* USER CODE END TIM4_Init 0 */

    TIM_Encoder_InitTypeDef sConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM4_Init 1 */

    /* USER CODE END TIM4_Init 1 */
    htim4.Instance = TIM4;
    htim4.Init.Prescaler = psc; // 捕获预分频器的值为0时代表每捕获一个边沿便执行捕获
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim4.Init.Period = arr; // 捕获一个周期总次数为最大值65535
    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    sConfig.EncoderMode = TIM_ENCODERMODE_TI12; // 设置编码器倍频数
    sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC1Filter = fil; // 滤波器1设置
    sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC2Filter = fil; // 滤波器2设置
    if (HAL_TIM_Encoder_Init(&htim4, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM4_Init 2 */

    /* USER CODE END TIM4_Init 2 */
}

/**
 * @brief TIM_Base MSP Initialization
 * This function configures the hardware resources used in this example
 * @param tim_baseHandle: TIM_Base handle pointer
 * @retval None
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *tim_baseHandle)
{
    if (tim_baseHandle->Instance == TIM1)
    {
        __HAL_RCC_TIM1_CLK_ENABLE();
    }
    else if (tim_baseHandle->Instance == TIM2)
    {
        __HAL_RCC_TIM2_CLK_ENABLE();
    }
    else if (tim_baseHandle->Instance == TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE();
    }
    else if (tim_baseHandle->Instance == TIM4)
    {
        __HAL_RCC_TIM4_CLK_ENABLE();
    }
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *tim_pwmHandle)
{

    if (tim_pwmHandle->Instance == TIM1)
    {
        /* USER CODE BEGIN TIM1_MspInit 0 */

        /* USER CODE END TIM1_MspInit 0 */
        /* TIM1 clock enable */
        __HAL_RCC_TIM1_CLK_ENABLE();
        /* USER CODE BEGIN TIM1_MspInit 1 */

        /* USER CODE END TIM1_MspInit 1 */
    }
}

/**
 * @brief  Initializes the TIM Encoder Interface MSP.
 * @param  htim TIM Encoder Interface handle
 * @retval None
 */
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *tim_encoderHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (tim_encoderHandle->Instance == TIM2)
    {
        /* USER CODE BEGIN TIM2_MspInit 0 */

        /* USER CODE END TIM2_MspInit 0 */
        /* TIM2 clock enable */
        __HAL_RCC_TIM2_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**TIM2 GPIO Configuration
        PA0-WKUP     ------> TIM2_CH1
        PA1     ------> TIM2_CH2
        */
        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM2_MspInit 1 */

        /* USER CODE END TIM2_MspInit 1 */
    }
    else if (tim_encoderHandle->Instance == TIM4)
    {
        /* USER CODE BEGIN TIM4_MspInit 0 */

        /* USER CODE END TIM4_MspInit 0 */
        /* TIM4 clock enable */
        __HAL_RCC_TIM4_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**TIM4 GPIO Configuration
        PB6     ------> TIM4_CH1
        PB7     ------> TIM4_CH2
        */
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM4_MspInit 1 */

        /* USER CODE END TIM4_MspInit 1 */
    }
}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *timHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (timHandle->Instance == TIM1)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**TIM1 GPIO Configuration
        PA8     ------> TIM1_CH1
        PA11     ------> TIM1_CH4
        */
        GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *tim_baseHandle)
{

    if (tim_baseHandle->Instance == TIM1)
    {
        /* USER CODE BEGIN TIM1_MspDeInit 0 */

        /* USER CODE END TIM1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM1_CLK_DISABLE();
        /* USER CODE BEGIN TIM1_MspDeInit 1 */

        /* USER CODE END TIM1_MspDeInit 1 */
    }
}

void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef *tim_encoderHandle)
{
    if (tim_encoderHandle->Instance == TIM2)
    {
        /* USER CODE BEGIN TIM2_MspDeInit 0 */

        /* USER CODE END TIM2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM2_CLK_DISABLE();

        /**TIM2 GPIO Configuration
        PA15     ------> TIM2_CH1
        PB3     ------> TIM2_CH2
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_15);

        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3);

        /* USER CODE BEGIN TIM2_MspDeInit 1 */

        /* USER CODE END TIM2_MspDeInit 1 */
    }
    else if (tim_encoderHandle->Instance == TIM3)
    {
        /* USER CODE BEGIN TIM3_MspDeInit 0 */

        /* USER CODE END TIM3_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM3_CLK_DISABLE();

        /**TIM3 GPIO Configuration
        PB4     ------> TIM3_CH1
        PB5     ------> TIM3_CH2
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_4 | GPIO_PIN_5);

        /* USER CODE BEGIN TIM3_MspDeInit 1 */

        /* USER CODE END TIM3_MspDeInit 1 */
    }
    else if (tim_encoderHandle->Instance == TIM4)
    {
        /* USER CODE BEGIN TIM4_MspDeInit 0 */

        /* USER CODE END TIM4_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM4_CLK_DISABLE();

        /**TIM4 GPIO Configuration
        PD12     ------> TIM4_CH1
        PD13     ------> TIM4_CH2
        */
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_12 | GPIO_PIN_13);

        /* USER CODE BEGIN TIM4_MspDeInit 1 */

        /* USER CODE END TIM4_MspDeInit 1 */
    }
}

// /**
//  * @brief   FreeRTOS计数初始化
//  * @note    用了Tim11，20KHz
//  * @param   None
//  * @retval  None
//  */
// void ConfigureTimerForTimeStats(void)
// {
//     FreeRTOSRunTimeTicks = 0;
//     MX_TIM14_Init(50 - 1, (u16)(SystemCoreClock / 1000000 / 2 - 1)); // 定时器14初始化，120分频，计数50，为20KHz
// }

// /**
//  * @brief   定时任务计数初始化
//  * @note    用了Tim14，1ms
//  * @param   None
//  * @retval  None
//  */
// void Tim_ConfigureTimerForTask(void)
// {
//     MX_TIM7_Init(100 - 1, (u16)(SystemCoreClock / 100000 / 2 - 1)); // 定时器7初始化，周期1ms
// }
