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
TIM_HandleTypeDef htim3; // 编码器1
TIM_HandleTypeDef htim4; // 编码器2
TIM_HandleTypeDef htim5; // 编码器3
TIM_HandleTypeDef htim6; // USMART
TIM_HandleTypeDef htim7; // 定时任务
TIM_HandleTypeDef htim8; // 4通道PWM
TIM_HandleTypeDef htim11;
TIM_HandleTypeDef htim13; // 补tim8 ch1 pwm，芯片管脚坏了
TIM_HandleTypeDef htim14; // FreeRTOS任务计时C

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

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = psc;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = arr;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
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
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
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
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */
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
 * @brief	TIM5 Initialization Function
 * @note	定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
 * @param 	arr-Period
 * 			psc-Prescaler
 * 			fil-IC1Filter and IC2Filter
 * @retval 	None
 */
void MX_TIM5_Init(u32 arr, u16 psc, u16 fil)
{

	/* USER CODE BEGIN TIM5_Init 0 */

	/* USER CODE END TIM5_Init 0 */

	TIM_Encoder_InitTypeDef sConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM5_Init 1 */

	/* USER CODE END TIM5_Init 1 */
	htim5.Instance = TIM5;
	htim5.Init.Prescaler = psc; // 捕获预分频器的值为0时代表每捕获一个边沿便执行捕获
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim5.Init.Period = arr; // 捕获一个周期总次数为最大值65535
	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	sConfig.EncoderMode = TIM_ENCODERMODE_TI12; // 设置编码器倍频数
	sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC1Filter = fil; // 滤波器1设置
	sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC2Filter = fil; // 滤波器2设置
	if (HAL_TIM_Encoder_Init(&htim5, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM5_Init 2 */

	/* USER CODE END TIM5_Init 2 */
}

// /**
//  * @brief TIM5 Initialization Function
//  * @note 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//  * @param None
//  * @retval None
//  */
// void MX_TIM5_Init(u32 arr, u16 psc)
// {

// 	/* USER CODE BEGIN TIM5_Init 0 */

// 	/* USER CODE END TIM5_Init 0 */

// 	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
// 	TIM_MasterConfigTypeDef sMasterConfig = {0};
// 	TIM_IC_InitTypeDef sConfigIC = {0};

// 	/* USER CODE BEGIN TIM5_Init 1 */

// 	/* USER CODE END TIM5_Init 1 */
// 	htim5.Instance = TIM5;
// 	htim5.Init.Prescaler = psc;
// 	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
// 	htim5.Init.Period = arr;
// 	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
// 	htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
// 	if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
// 	{
// 		Error_Handler();
// 	}
// 	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
// 	if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
// 	{
// 		Error_Handler();
// 	}
// 	if (HAL_TIM_IC_Init(&htim5) != HAL_OK)
// 	{
// 		Error_Handler();
// 	}
// 	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
// 	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
// 	if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
// 	{
// 		Error_Handler();
// 	}
// 	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
// 	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
// 	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
// 	sConfigIC.ICFilter = 0;
// 	if (HAL_TIM_IC_ConfigChannel(&htim5, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
// 	{
// 		Error_Handler();
// 	}
// 	/* USER CODE BEGIN TIM5_Init 2 */

// 	/* USER CODE END TIM5_Init 2 */
// 	HAL_TIM_MspPostInit(&htim5);

// 	if (HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1) != HAL_OK) // 开启 TIM5的捕获通道 1，并且开启捕获中断
// 	{
// 		Error_Handler();
// 	}

// 	__HAL_TIM_ENABLE_IT(&htim5, TIM_IT_UPDATE); // 使能更新中断
// }

/**
 * @brief   TIM6 Initialization Function
 * @note    !!!用作了USMART计时定时器!!!，
 *          定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
 * @param 	arr-Period
 * 			psc-Prescaler
 * @retval  None
 */
void MX_TIM6_Init(u16 arr, u16 psc)
{

	/* USER CODE BEGIN TIM6_Init 0 */

	/* USER CODE END TIM6_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM6_Init 1 */

	/* USER CODE END TIM6_Init 1 */
	htim6.Instance = TIM6;
	htim6.Init.Prescaler = psc;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period = arr;
	htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM6_Init 2 */
	if (HAL_TIM_Base_Start_IT(&htim6) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE END TIM6_Init 2 */
}

/**
 * @brief   TIM7 Initialization Function
 * @note    !!!用作了  !!!，
 *          定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
 * @param 	arr-Period
 * 			psc-Prescaler
 * @retval  None
 */
void MX_TIM7_Init(u16 arr, u16 psc)
{

	/* USER CODE BEGIN TIM7_Init 0 */

	/* USER CODE END TIM7_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM7_Init 1 */

	/* USER CODE END TIM7_Init 1 */
	htim7.Instance = TIM7;
	htim7.Init.Prescaler = psc;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = arr;
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM7_Init 2 */
	if (HAL_TIM_Base_Start_IT(&htim7) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE END TIM7_Init 2 */
}

// /**
//  * @brief   TIM7 Initialization Function
//  * @note    !!!用作了DAC触发器!!!，
//  *          定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//  * @param   None
//  * @retval  None
//  */
// void MX_TIM7_Init(u16 arr, u16 psc)
// {

// 	/* USER CODE BEGIN TIM7_Init 0 */

// 	/* USER CODE END TIM7_Init 0 */

// 	TIM_MasterConfigTypeDef sMasterConfig = {0};

// 	/* USER CODE BEGIN TIM7_Init 1 */

// 	/* USER CODE END TIM7_Init 1 */
// 	htim7.Instance = TIM7;
// 	htim7.Init.Prescaler = psc;
// 	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
// 	htim7.Init.Period = arr;
// 	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
// 	if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
// 	{
// 		Error_Handler();
// 	}
// 	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
// 	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
// 	if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
// 	{
// 		Error_Handler();
// 	}
// 	/* USER CODE BEGIN TIM7_Init 2 */

// 	/* USER CODE END TIM7_Init 2 */
// 	HAL_TIM_Base_Start(&htim7);
// }

/**
 * @brief	TIM8 Initialization Function
 * @note	定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
 * @param 	arr-Period
 * 			psc-Prescaler
 * @retval 	None
 */
void MX_TIM8_Init(u16 arr, u16 psc)
{

	/* USER CODE BEGIN TIM8_Init 0 */

	/* USER CODE END TIM8_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

	/* USER CODE BEGIN TIM8_Init 1 */

	/* USER CODE END TIM8_Init 1 */
	htim8.Instance = TIM8;
	htim8.Init.Prescaler = psc;
	htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim8.Init.Period = arr;
	htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim8.Init.RepetitionCounter = 0;
	htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim8) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
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
	if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
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
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim8, &sBreakDeadTimeConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM8_Init 2 */

	/* USER CODE END TIM8_Init 2 */
	HAL_TIM_MspPostInit(&htim8);
}

/**
 * @brief   TIM11 Initialization Function
 * @note    !!!用作了FreeRTOS获取时间定时!!!，
 *          定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
 * @param   None
 * @retval  None
 */
void MX_TIM11_Init(u16 arr, u16 psc)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	htim11.Instance = TIM11;
	htim11.Init.Prescaler = psc;
	htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim11.Init.Period = arr;
	htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_Base_Start_IT(&htim11) != HAL_OK) // 也可以用这个，使能定时器11和定时器11更新中断：TIM_IT_UPDATE
	{
		Error_Handler();
	}
}

/**
 * @brief   TIM13 Initialization Function
 * @note    !!!用作了USMART定时!!!，
 *          定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
 * @param   None
 * @retval  None
 */
void MX_TIM13_Init(u16 arr, u16 psc)
{
	/* USER CODE BEGIN TIM13_Init 0 */

	/* USER CODE END TIM13_Init 0 */

	TIM_OC_InitTypeDef sConfigOC = {0};

	/* USER CODE BEGIN TIM13_Init 1 */

	/* USER CODE END TIM13_Init 1 */
	htim13.Instance = TIM13;
	htim13.Init.Prescaler = psc;
	htim13.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim13.Init.Period = arr;
	htim13.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim13.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim13) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim13) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim13, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM13_Init 2 */

	/* USER CODE END TIM13_Init 2 */
	HAL_TIM_MspPostInit(&htim13);
}

/**
 * @brief   TIM14 Initialization Function
 * @note    !!!用作了任务ms计数定时!!!，
 *          定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
 * @param   None
 * @retval  None
 */
void MX_TIM14_Init(u16 arr, u16 psc)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	htim14.Instance = TIM14;
	htim14.Init.Prescaler = psc;
	htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim14.Init.Period = arr;
	htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_Base_Start_IT(&htim14) != HAL_OK) // 也可以用这个，使能定时器14和定时器14更新中断：TIM_IT_UPDATE
	{
		Error_Handler();
	}
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
		/* USER CODE BEGIN TIM1_MspInit 0 */

		/* USER CODE END TIM1_MspInit 0 */
		/* TIM1 clock enable */
		__HAL_RCC_TIM1_CLK_ENABLE();
		/* USER CODE BEGIN TIM1_MspInit 1 */

		/* USER CODE END TIM1_MspInit 1 */
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
	else if (tim_baseHandle->Instance == TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();
	}
	else if (tim_baseHandle->Instance == TIM6)
	{
		/* USER CODE BEGIN TIM6_MspInit 0 */

		/* USER CODE END TIM6_MspInit 0 */
		/* TIM6 clock enable */
		__HAL_RCC_TIM6_CLK_ENABLE();

		/* TIM6 interrupt Init */
		HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 3, 0);
		HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
		/* USER CODE BEGIN TIM6_MspInit 1 */

		/* USER CODE END TIM6_MspInit 1 */
	}
	else if (tim_baseHandle->Instance == TIM7)
	{
		/* USER CODE BEGIN TIM7_MspInit 0 */

		/* USER CODE END TIM7_MspInit 0 */
		/* TIM7 clock enable */
		__HAL_RCC_TIM7_CLK_ENABLE();

		/* TIM7 interrupt Init */
		HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM7_IRQn);
		/* USER CODE BEGIN TIM7_MspInit 1 */

		/* USER CODE END TIM7_MspInit 1 */
	}
	else if (tim_baseHandle->Instance == TIM8)
	{
		/* USER CODE BEGIN TIM8_MspInit 0 */

		/* USER CODE END TIM8_MspInit 0 */
		/* TIM8 clock enable */
		__HAL_RCC_TIM8_CLK_ENABLE();
		/* USER CODE BEGIN TIM8_MspInit 1 */

		/* USER CODE END TIM8_MspInit 1 */
	}
	else if (tim_baseHandle->Instance == TIM11)
	{
		/* USER CODE BEGIN TIM11_MspInit 0 */

		/* USER CODE END TIM11_MspInit 0 */
		/* TIM11 clock enable */
		__HAL_RCC_TIM11_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 4, 0);
		HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
		/* USER CODE BEGIN TIM11_MspInit 1 */

		/* USER CODE END TIM11_MspInit 1 */
	}
	else if (tim_baseHandle->Instance == TIM13)
	{
		/* USER CODE BEGIN TIM13_MspInit 0 */

		/* USER CODE END TIM13_MspInit 0 */
		/* TIM11 clock enable */
		__HAL_RCC_TIM13_CLK_ENABLE();
		// HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 4, 0);
		// HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
		/* USER CODE BEGIN TIM13_MspInit 1 */

		/* USER CODE END TIM13_MspInit 1 */
	}
	else if (tim_baseHandle->Instance == TIM14)
	{
		/* USER CODE BEGIN TIM14_MspInit 0 */

		/* USER CODE END TIM14_MspInit 0 */
		/* TIM11 clock enable */
		__HAL_RCC_TIM14_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, 4, 0);
		HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);
		/* USER CODE BEGIN TIM14_MspInit 1 */

		/* USER CODE END TIM14_MspInit 1 */
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
		__HAL_RCC_GPIOB_CLK_ENABLE();
		/**TIM2 GPIO Configuration
		PA15     ------> TIM2_CH1
		PB3     ------> TIM2_CH2
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_15;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* USER CODE BEGIN TIM2_MspInit 1 */

		/* USER CODE END TIM2_MspInit 1 */
	}
	else if (tim_encoderHandle->Instance == TIM3)
	{
		/* USER CODE BEGIN TIM3_MspInit 0 */

		/* USER CODE END TIM3_MspInit 0 */
		/* TIM3 clock enable */
		__HAL_RCC_TIM3_CLK_ENABLE();

		__HAL_RCC_GPIOB_CLK_ENABLE();
		/**TIM3 GPIO Configuration
		PB4     ------> TIM3_CH1
		PB5     ------> TIM3_CH2
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* USER CODE BEGIN TIM3_MspInit 1 */

		/* USER CODE END TIM3_MspInit 1 */
	}
	else if (tim_encoderHandle->Instance == TIM4)
	{
		/* USER CODE BEGIN TIM4_MspInit 0 */

		/* USER CODE END TIM4_MspInit 0 */
		/* TIM4 clock enable */
		__HAL_RCC_TIM4_CLK_ENABLE();

		__HAL_RCC_GPIOD_CLK_ENABLE();
		/**TIM4 GPIO Configuration
		PD12     ------> TIM4_CH1
		PD13     ------> TIM4_CH2
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

		/* USER CODE BEGIN TIM4_MspInit 1 */

		/* USER CODE END TIM4_MspInit 1 */
	}
	else if (tim_encoderHandle->Instance == TIM5)
	{
		/* USER CODE BEGIN TIM5_MspInit 0 */

		/* USER CODE END TIM5_MspInit 0 */
		/* TIM5 clock enable */
		__HAL_RCC_TIM5_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**TIM5 GPIO Configuration
		PA0-WKUP     ------> TIM5_CH1
		PA1     ------> TIM5_CH2
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* USER CODE BEGIN TIM5_MspInit 1 */

		/* USER CODE END TIM5_MspInit 1 */
	}
}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *timHandle)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if (timHandle->Instance == TIM1)
	{
		/* USER CODE BEGIN TIM1_MspPostInit 0 */

		/* USER CODE END TIM1_MspPostInit 0 */
		__HAL_RCC_GPIOE_CLK_ENABLE();
		/**TIM1 GPIO Configuration
		PE9     ------> TIM1_CH1
		PE11     ------> TIM1_CH2
		PE13     ------> TIM1_CH3
		PE14     ------> TIM1_CH4
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

		/* USER CODE BEGIN TIM1_MspPostInit 1 */

		/* USER CODE END TIM1_MspPostInit 1 */
	}
	else if (timHandle->Instance == TIM8)
	{
		/* USER CODE BEGIN TIM8_MspPostInit 0 */

		/* USER CODE END TIM8_MspPostInit 0 */

		__HAL_RCC_GPIOC_CLK_ENABLE();
		/**TIM8 GPIO Configuration
		PC6     ------> TIM8_CH1
		PC7     ------> TIM8_CH2
		PC8     ------> TIM8_CH3
		PC9     ------> TIM8_CH4
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		/* USER CODE BEGIN TIM8_MspPostInit 1 */

		/* USER CODE END TIM8_MspPostInit 1 */
	}
	else if (timHandle->Instance == TIM13)
	{
		/* USER CODE BEGIN TIM13_MspPostInit 0 */

		/* USER CODE END TIM13_MspPostInit 0 */

		__HAL_RCC_GPIOF_CLK_ENABLE();
		/**TIM13 GPIO Configuration
		PF8     ------> TIM13_CH1
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_8;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF9_TIM13;
		HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

		/* USER CODE BEGIN TIM13_MspPostInit 1 */

		/* USER CODE END TIM13_MspPostInit 1 */
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
	else if (tim_baseHandle->Instance == TIM6)
	{
		/* USER CODE BEGIN TIM6_MspDeInit 0 */

		/* USER CODE END TIM6_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_TIM6_CLK_DISABLE();

		/* TIM6 interrupt Deinit */
		HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
		/* USER CODE BEGIN TIM6_MspDeInit 1 */

		/* USER CODE END TIM6_MspDeInit 1 */
	}
	else if (tim_baseHandle->Instance == TIM7)
	{
		/* USER CODE BEGIN TIM7_MspDeInit 0 */

		/* USER CODE END TIM7_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_TIM7_CLK_DISABLE();

		/* TIM7 interrupt Deinit */
		HAL_NVIC_DisableIRQ(TIM7_IRQn);
		/* USER CODE BEGIN TIM7_MspDeInit 1 */

		/* USER CODE END TIM7_MspDeInit 1 */
	}
	else if (tim_baseHandle->Instance == TIM8)
	{
		/* USER CODE BEGIN TIM8_MspDeInit 0 */

		/* USER CODE END TIM8_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_TIM8_CLK_DISABLE();
		/* USER CODE BEGIN TIM8_MspDeInit 1 */

		/* USER CODE END TIM8_MspDeInit 1 */
	}
	else if (tim_baseHandle->Instance == TIM11)
	{
		/* USER CODE BEGIN TIM11_MspDeInit 0 */

		/* USER CODE END TIM11_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_TIM11_CLK_DISABLE();
		/* USER CODE BEGIN TIM11_MspDeInit 1 */

		/* USER CODE END TIM11_MspDeInit 1 */
	}
	else if (tim_baseHandle->Instance == TIM13)
	{
		/* USER CODE BEGIN TIM13_MspDeInit 0 */

		/* USER CODE END TIM13_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_TIM13_CLK_DISABLE();
		/* USER CODE BEGIN TIM13_MspDeInit 1 */

		/* USER CODE END TIM13_MspDeInit 1 */
	}
	else if (tim_baseHandle->Instance == TIM14)
	{
		/* USER CODE BEGIN TIM14_MspDeInit 0 */

		/* USER CODE END TIM14_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_TIM14_CLK_DISABLE();
		/* USER CODE BEGIN TIM14_MspDeInit 1 */

		/* USER CODE END TIM14_MspDeInit 1 */
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
	else if (tim_encoderHandle->Instance == TIM5)
	{
		/* USER CODE BEGIN TIM5_MspDeInit 0 */

		/* USER CODE END TIM5_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_TIM5_CLK_DISABLE();

		/**TIM5 GPIO Configuration
		PA0-WKUP     ------> TIM5_CH1
		PA1     ------> TIM5_CH2
		*/
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0 | GPIO_PIN_1);

		/* USER CODE BEGIN TIM5_MspDeInit 1 */

		/* USER CODE END TIM5_MspDeInit 1 */
	}
}

/**
 * @brief   FreeRTOS计数初始化
 * @note    用了Tim11，20KHz
 * @param   None
 * @retval  None
 */
void ConfigureTimerForTimeStats(void)
{
	FreeRTOSRunTimeTicks = 0;
	MX_TIM14_Init(50 - 1, (u16)(SystemCoreClock / 1000000 / 2 - 1)); // 定时器14初始化，120分频，计数50，为20KHz
}

/**
 * @brief   定时任务计数初始化
 * @note    用了Tim14，1ms
 * @param   None
 * @retval  None
 */
void Tim_ConfigureTimerForTask(void)
{
	MX_TIM7_Init(100 - 1, (u16)(SystemCoreClock / 100000 / 2 - 1)); // 定时器7初始化，周期1ms
}
