#include "stm32f4xx_it.h"
#include "adc.h"
#include "adc_task.h"
#include "dma2d.h"
#include "i2c.h"
#include "tasks_common.h"
#include "tasks_sync.h"
#include "tim.h"

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
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
    while (1)
    {
    }
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles System service call via SWI instruction.
 */
__attribute__((weak)) void SVC_Handler(void)
{
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief This function handles Pendable request for system service.
 */
__attribute__((weak)) void PendSV_Handler(void)
{
}

/**
 * @brief This function handles System tick timer.
 */
__attribute__((weak)) void SysTick_Handler(void)
{
    HAL_IncTick();
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

// 窗口看门狗中断服务函数
// void WWDG_IQHandler(void)
// {
//   HAL_WWDG_IRQHandler(&hwwdg);
// }

// 中断服务函数处理过程,此函数会被HAL_WWDG_IRQHandler()调用
// void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)
// {
//   static uint32_t FreeRTOSRunTimeTicksold = 0;
//   HAL_WWDG_Refresh(hwwdg); // 更新窗口看门狗值
//   LED1_Reverse();
//   LOGI("WWDG_tim=%u", (uint32_t)((uint32_t)(FreeRTOSRunTimeTicks - FreeRTOSRunTimeTicksold) * 50));
//   FreeRTOSRunTimeTicksold = FreeRTOSRunTimeTicks;
// }

#if SYSTEM_SUPPORT_OS
/**
 * @brief This function handles TIM7 global interrupt.
 */
void TIM7_IRQHandler(void)
{
    /* USER CODE BEGIN TIM7_IRQn 0 */

    /* USER CODE END TIM7_IRQn 0 */
    HAL_TIM_IRQHandler(&htim7);
    /* USER CODE BEGIN TIM7_IRQn 1 */

    /* USER CODE END TIM7_IRQn 1 */
}
#endif

void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim14);
}

/**
 * @brief This function handles DMA1 stream0 global interrupt.
 */
void DMA1_Stream0_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */

    /* USER CODE END DMA1_Stream0_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_i2c1_rx);
    /* USER CODE BEGIN DMA1_Stream0_IRQn 1 */

    /* USER CODE END DMA1_Stream0_IRQn 1 */
}

/**
 * @brief This function handles DMA1 stream6 global interrupt.
 */
void DMA1_Stream6_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Stream6_IRQn 0 */

    /* USER CODE END DMA1_Stream6_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_i2c1_tx);
    /* USER CODE BEGIN DMA1_Stream6_IRQn 1 */

    /* USER CODE END DMA1_Stream6_IRQn 1 */
}

/**
 * @brief This function handles I2C1 event interrupt.
 */
void I2C1_EV_IRQHandler(void)
{
    /* USER CODE BEGIN I2C1_EV_IRQn 0 */

    /* USER CODE END I2C1_EV_IRQn 0 */
    HAL_I2C_EV_IRQHandler(&hi2c1);
    /* USER CODE BEGIN I2C1_EV_IRQn 1 */

    /* USER CODE END I2C1_EV_IRQn 1 */
}

/**
 * @brief This function handles DMA2 stream0 global interrupt.
 */
void DMA2_Stream0_IRQHandler(void)
{
    /* USER CODE BEGIN DMA2_Stream0_IRQn 0 */

    /* USER CODE END DMA2_Stream0_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_adc1);
    /* USER CODE BEGIN DMA2_Stream0_IRQn 1 */

    /* USER CODE END DMA2_Stream0_IRQn 1 */
}

/**
 * @brief This function handles DMA2D global interrupt.
 */
void DMA2D_IRQHandler(void)
{
    /* USER CODE BEGIN DMA2D_IRQn 0 */

    /* USER CODE END DMA2D_IRQn 0 */
    HAL_DMA2D_IRQHandler(&hdma2d);
    /* USER CODE BEGIN DMA2D_IRQn 1 */

    /* USER CODE END DMA2D_IRQn 1 */
}
