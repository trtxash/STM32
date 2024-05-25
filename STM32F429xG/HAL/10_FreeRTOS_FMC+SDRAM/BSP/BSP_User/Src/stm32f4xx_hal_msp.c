/* Includes ------------------------------------------------------------------*/
#include "main.h"

/**
 * Initializes the Global MSP.
 */
void HAL_MspInit(void)
{
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
}

/**
 * @brief TIM_Base MSP Initialization
 * This function configures the hardware resources used in this example
 * @param tim_baseHandle: TIM_Base handle pointer
 * @retval None
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *tim_baseHandle)
{
	if (tim_baseHandle->Instance == TIM14)
	{
		__HAL_RCC_TIM14_CLK_ENABLE();

		/* TIM6 interrupt Init */
		HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, 4, 0);
		HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);
		/* USER CODE BEGIN TIM6_MspInit 1 */

		/* USER CODE END TIM6_MspInit 1 */
	}
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *tim_baseHandle)
{

	if (tim_baseHandle->Instance == TIM14)
	{
		__HAL_RCC_TIM14_CLK_DISABLE();
	}
}

void HAL_WWDG_MspInit(WWDG_HandleTypeDef *wwdgHandle)
{

	if (wwdgHandle->Instance == WWDG)
	{
		__HAL_RCC_WWDG_CLK_ENABLE();

		HAL_NVIC_SetPriority(WWDG_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(WWDG_IRQn);
	}
}