#include "ultrasonic.h"

u8 ULTRASONIC_CAPTURE_STA = 0; // 输入捕获状态
u32 ULTRASONIC_CAPTURE_VAL;	   // 输入捕获值 (TIM2/TIM5是 32位 )

/**
 * @brief  初始化超声波
 * @note   使用tim5，计数步长1us
 * @param  NONE
 * @retval None
 */
void ultrasonic_init(void)
{
	GPIO_InitTypeDef GPIO_Initure = {0};

	Trig_Port_Clk_Enable();

	GPIO_Initure.Pin = Trig_Pin;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Initure.Pull = GPIO_PULLUP;
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(Trig_Port, &GPIO_Initure);

	MX_TIM5_Init(0XFFFFFFFF, sys_clock / 2 - 1); // 1MHz,1us
}

/**
 * @brief  输入捕获值得到
 * @note   使用tim5，计数步长1us
 * @param  NONE
 * @retval u32 temp，单位us
 */
u32 get_capture_value(void)
{
	u32 temp = 0;
	if (ULTRASONIC_CAPTURE_STA & 0X80) // 成功捕获到了一次高电平
	{
		temp = ULTRASONIC_CAPTURE_STA & 0X3F;
		temp *= 0XFFFFFFFF;				// 溢出时间总和
		temp += ULTRASONIC_CAPTURE_VAL; // 得到总的高电平时间

		ULTRASONIC_CAPTURE_STA = 0; // 开启下一次捕获
	}
	return temp;
}

/**
 * @brief  得到距离,单位mm
 * @note   使用tim5，计数步长1us
 * @param  NONE
 * @retval float temp，单位mm
 */
float get_ultrasonic_value(void)
{
	float temp;
	temp = (float)(0.340 * get_capture_value());

	return temp;
}

/**
 * @brief  超声波任务，周期运行
 * @note   第一次开启超声，后每次读取上次值
 * @param  NONE
 * @retval float temp，单位mm
 */
float ultrasonic_task(void)
{
	float temp;
	temp = get_ultrasonic_value();

	Trig_Set();
	delay_us(10);
	Trig_Clr();

	return temp;
}

// TIM5设置参考
//  /**
//   * @brief TIM5 Initialization Function
//   * @note 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//   * @param None
//   * @retval None
//   */
//  void MX_TIM5_Init(u32 arr, u16 psc)
//  {

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
// }

// if ((ULTRASONIC_CAPTURE_STA & 0X80) == 0) // 还未成功捕获
// {
// 	if (ULTRASONIC_CAPTURE_STA & 0X40) // 已经捕获到高电平了
// 	{
// 		if ((ULTRASONIC_CAPTURE_STA & 0X3F) == 0X3F) // 高电平太长了
// 		{
// 			ULTRASONIC_CAPTURE_STA |= 0X80; // 强制标记成功捕获了一次
// 			ULTRASONIC_CAPTURE_VAL = 0XFFFFFFFF;
// 		}
// 		else
// 			ULTRASONIC_CAPTURE_STA++;
// 	}
// }

// if ((ULTRASONIC_CAPTURE_STA & 0X80) == 0) // 还未成功捕获
// {

// 	if (ULTRASONIC_CAPTURE_STA & 0X40) // 捕获到一个下降沿
// 	{
// 		ULTRASONIC_CAPTURE_STA |= 0X80;											 // 标记成功捕获到一次高电平脉宽
// 		ULTRASONIC_CAPTURE_VAL = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // 获取当前的捕获值
// 		TIM_RESET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1);							 // 一定要先清除原来的设置！！
// 		TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING);	 // 配置 TIM5通道 1上升沿捕获
// 	}
// 	else // 还未开始 ,第一次捕获上升沿
// 	{
// 		ULTRASONIC_CAPTURE_STA = 0; // 清空
// 		ULTRASONIC_CAPTURE_VAL = 0;
// 		ULTRASONIC_CAPTURE_STA |= 0X40; // 标记捕获到了上升沿
// 		__HAL_TIM_DISABLE(htim);		// 关闭定时器 5
// 		__HAL_TIM_SET_COUNTER(htim, 0);
// 		TIM_RESET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1);						  // 一定要先清除原来的设置！！
// 		TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_ICPOLARITY_FALLING); // 定时器 5通道 1设置为下降沿捕获
// 		__HAL_TIM_ENABLE(htim);												  // 使能定时器 5
// 	}
// }