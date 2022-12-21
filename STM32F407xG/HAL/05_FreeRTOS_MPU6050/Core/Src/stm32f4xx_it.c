/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32f4xx_it.c
 * @brief   Interrupt Service Routines.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */
extern DMA_HandleTypeDef hdma_tim4_ch1;

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
	/* USER CODE BEGIN NonMaskableInt_IRQn 0 */

	/* USER CODE END NonMaskableInt_IRQn 0 */
	/* USER CODE BEGIN NonMaskableInt_IRQn 1 */
	while (1)
	{
	}
	/* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
	/* USER CODE BEGIN HardFault_IRQn 0 */

	/* USER CODE END HardFault_IRQn 0 */
	while (1)
	{
		/* USER CODE BEGIN W1_HardFault_IRQn 0 */
		/* USER CODE END W1_HardFault_IRQn 0 */
	}
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{
	/* USER CODE BEGIN MemoryManagement_IRQn 0 */

	/* USER CODE END MemoryManagement_IRQn 0 */
	while (1)
	{
		/* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
		/* USER CODE END W1_MemoryManagement_IRQn 0 */
	}
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
	/* USER CODE BEGIN BusFault_IRQn 0 */

	/* USER CODE END BusFault_IRQn 0 */
	while (1)
	{
		/* USER CODE BEGIN W1_BusFault_IRQn 0 */
		/* USER CODE END W1_BusFault_IRQn 0 */
	}
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
	/* USER CODE BEGIN UsageFault_IRQn 0 */

	/* USER CODE END UsageFault_IRQn 0 */
	while (1)
	{
		/* USER CODE BEGIN W1_UsageFault_IRQn 0 */
		/* USER CODE END W1_UsageFault_IRQn 0 */
	}
}

/**
 * @brief This function handles System service call via SWI instruction.
 */
__weak void SVC_Handler(void)
{
	/* USER CODE BEGIN SVCall_IRQn 0 */

	/* USER CODE END SVCall_IRQn 0 */
	/* USER CODE BEGIN SVCall_IRQn 1 */

	/* USER CODE END SVCall_IRQn 1 */
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void)
{
	/* USER CODE BEGIN DebugMonitor_IRQn 0 */

	/* USER CODE END DebugMonitor_IRQn 0 */
	/* USER CODE BEGIN DebugMonitor_IRQn 1 */

	/* USER CODE END DebugMonitor_IRQn 1 */
}

/**
 * @brief This function handles Pendable request for system service.
 */
__weak void PendSV_Handler(void)
{
	/* USER CODE BEGIN PendSV_IRQn 0 */

	/* USER CODE END PendSV_IRQn 0 */
	/* USER CODE BEGIN PendSV_IRQn 1 */

	/* USER CODE END PendSV_IRQn 1 */
}

/**
 * @brief This function handles System tick timer.
 */
__weak void SysTick_Handler(void)
{
	/* USER CODE BEGIN SysTick_IRQn 0 */

	/* USER CODE END SysTick_IRQn 0 */
	HAL_IncTick();
	/* USER CODE BEGIN SysTick_IRQn 1 */

	/* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
 * @brief 定时器4中断服务函数
 */
void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim4);
}

/**
 * @brief 定时器5中断服务函数
 */
void TIM5_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim5);
}

/**
 * @brief 定时器1com,11中断服务函数
 */
void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim11);
}

/**
 * @brief 定时器8up,13中断服务函数
 */
void TIM8_UP_TIM13_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim13);
}

/**
 * @brief 定时器8com,14中断服务函数
 */
void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim14);
}

/**
 * @brief   定时器更新回调函数
 * @note    void HAL_TIM_IRQHandler(TIM_HandleTypeDef *htim)；调用，其会自动清除中断标志
 * @param   *htim
 * @retval  None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == (&htim4))
	{
	}
	else if (htim == (&htim5))
	{
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
	}
	else if (htim == (&htim11))
	{
		FreeRTOSRunTimeTicks++; // important
	}
	else if (htim == (&htim13))
	{
		usmart_dev.scan();												// 执行usmart扫描
		__HAL_TIM_SET_COUNTER(&USMARTTimer_HandleTypeDef, 0);			// 清空定时器的CNT
		__HAL_TIM_SET_AUTORELOAD(&USMARTTimer_HandleTypeDef, 1000 - 1); // 恢复原来的设置
	}
	else if (htim == (&htim14))
	{
		static u16 x1ms = 0;

		x1ms++;

		if (x1ms % 10 == 0) // 10ms
		{
		}
		if (x1ms % 100 == 0) // 100ms
		{
		}
		if (x1ms % 1000 == 0) // 1000ms
		{
			printf("Pitch:  %f\r\n", (float)pitch);
			printf("Roll:  %f\r\n", (float)roll);
			printf("yaw:  %f\r\n", (float)yaw);
			printf("temp:  %f\r\n", (float)temp);
			printf("next \r\n");
		}
		if (x1ms % 10000 == 0) // 10000ms
		{
		}

		if (x1ms >= 60000)
		{
			x1ms = 0; // 归零
		}
	}
}

/**
 * @brief   定时器捕获回调函数
 * @note    void HAL_TIM_IRQHandler(TIM_HandleTypeDef *htim)；调用，其会自动清除中断标志
 * @param   *htim
 * @retval  None
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim == (&htim5))
	{
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
	}
}

/**
 * @brief This function handles DMA1 stream0 global interrupt.
 */
void DMA1_Stream0_IRQHandler(void)
{
	/* USER CODE BEGIN DMA1_Stream0_IRQn 0 */

	/* USER CODE END DMA1_Stream0_IRQn 0 */
	HAL_DMA_IRQHandler(&hdma_tim4_ch1);
	/* USER CODE BEGIN DMA1_Stream0_IRQn 1 */

	/* USER CODE END DMA1_Stream0_IRQn 1 */
}

// /**
//  * @brief This function handles DMA1 stream5 global interrupt.
//  */
// void DMA1_Stream5_IRQHandler(void)
// {
// 	/* USER CODE BEGIN DMA1_Stream5_IRQn 0 */

// 	/* USER CODE END DMA1_Stream5_IRQn 0 */
// 	HAL_DMA_IRQHandler(&hdma_dac1);
// 	/* USER CODE BEGIN DMA1_Stream5_IRQn 1 */

// 	/* USER CODE END DMA1_Stream5_IRQn 1 */
// }

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
