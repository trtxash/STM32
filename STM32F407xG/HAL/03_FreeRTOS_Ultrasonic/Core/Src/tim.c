/**
 * @file	tim.c
 * @brief 	定时器设置
 *          适用STM32F4，但具体定时器需要配置
 *          顺序为，句柄，定时器初始化，定时器PWM初始化，定时器底层驱动，定时器PWM底层驱动，定时器PWM占空比设置，中断服务，中断回调函数
 *          中断服务函数名可在 “startup_stm32f40_41xxx.s”文件中找到
 *          中断定义在stm32f4xx_it.c中
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2022年11月17号12点50分
 */
#include "tim.h"

TIM_HandleTypeDef htim4;
DMA_HandleTypeDef hdma_tim4_ch1;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim13;
TIM_HandleTypeDef htim14;

/**
 * @brief TIM4 Initialization Function
 * @note 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
 * @param None
 * @retval None
 */
void MX_TIM4_Init(u16 arr, u16 psc)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	htim4.Instance = TIM4;
	htim4.Init.Prescaler = psc;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = arr;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_TIM_MspPostInit(&htim4);
}

/**
 * @brief TIM5 Initialization Function
 * @note 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
 * @param None
 * @retval None
 */
void MX_TIM5_Init(u32 arr, u16 psc)
{

	/* USER CODE BEGIN TIM5_Init 0 */

	/* USER CODE END TIM5_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_IC_InitTypeDef sConfigIC = {0};

	/* USER CODE BEGIN TIM5_Init 1 */

	/* USER CODE END TIM5_Init 1 */
	htim5.Instance = TIM5;
	htim5.Init.Prescaler = psc;
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim5.Init.Period = arr;
	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_IC_Init(&htim5) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	if (HAL_TIM_IC_ConfigChannel(&htim5, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM5_Init 2 */

	/* USER CODE END TIM5_Init 2 */
	HAL_TIM_MspPostInit(&htim5);

	if (HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1) != HAL_OK) // 开启 TIM5的捕获通道 1，并且开启捕获中断
	{
		Error_Handler();
	}

	__HAL_TIM_ENABLE_IT(&htim5, TIM_IT_UPDATE); // 使能更新中断
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
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	htim13.Instance = TIM13;
	htim13.Init.Prescaler = psc;
	htim13.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim13.Init.Period = arr;
	htim13.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim13.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim13) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_Base_Start_IT(&htim13) != HAL_OK) // 也可以用这个，使能定时器3和定时器3更新中断：TIM_IT_UPDATE
	{
		Error_Handler();
	}
}

/**
 * @brief   TIM14 Initialization Function
 * @note    !!!用作了计数定时!!!，
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
	if (HAL_TIM_Base_Start_IT(&htim14) != HAL_OK) // 也可以用这个，使能定时器3和定时器3更新中断：TIM_IT_UPDATE
	{
		Error_Handler();
	}
}

/**
 * @brief TIM_Base MSP Initialization
 * This function configures the hardware resources used in this example
 * @param htim_base: TIM_Base handle pointer
 * @retval None
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim_base)
{
	if (htim_base->Instance == TIM4)
	{
		/* Peripheral clock enable */
		__HAL_RCC_TIM4_CLK_ENABLE();

		/* TIM4 DMA Init */
		/* TIM4_CH1 Init */
		hdma_tim4_ch1.Instance = DMA1_Stream0;
		hdma_tim4_ch1.Init.Channel = DMA_CHANNEL_2;
		hdma_tim4_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_tim4_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_tim4_ch1.Init.MemInc = DMA_MINC_ENABLE;
		hdma_tim4_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma_tim4_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		hdma_tim4_ch1.Init.Mode = DMA_CIRCULAR;
		hdma_tim4_ch1.Init.Priority = DMA_PRIORITY_MEDIUM;
		if (HAL_DMA_Init(&hdma_tim4_ch1) != HAL_OK)
		{
			Error_Handler();
		}

		/* Several peripheral DMA handle pointers point to the same DMA handle.
		 Be aware that there is only one channel to perform all the requested DMAs. */
		__HAL_LINKDMA(htim_base, hdma[TIM_DMA_ID_CC1], hdma_tim4_ch1);
	}
	else if (htim_base->Instance == TIM5)
	{
		/* TIM5 clock enable */
		__HAL_RCC_TIM5_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM5_IRQn, 3, 0);
		HAL_NVIC_EnableIRQ(TIM5_IRQn);
	}
	else if (htim_base->Instance == TIM13)
	{
		__HAL_RCC_TIM13_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 3, 0);
		HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
	}
	else if (htim_base->Instance == TIM14)
	{
		__HAL_RCC_TIM14_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, 3, 0);
		HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);
	}
}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if (htim->Instance == TIM4)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();
		/**TIM4 GPIO Configuration
		PD12    ------> TIM4_CH1
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_12;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	}
	else if (htim->Instance == TIM5)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**TIM5 GPIO Configuration
		PA0-WKUP     ------> TIM5_CH1
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_0;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}