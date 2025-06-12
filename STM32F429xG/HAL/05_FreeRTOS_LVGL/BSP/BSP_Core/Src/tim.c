/**
 * @file	tim.c
 * @brief 	定时器设置
 *          适用STM32F4，但具体定时器需要配置
 *          顺序为，句柄，定时器初始化，定时器PWM初始化，定时器底层驱动，定时器PWM底层驱动，定时器PWM占空比设置，中断服务，中断回调函数
 *          中断服务函数名可在 “startup_stm32f40_41xxx.s”文件中找到
 *          中断定义在stm32f4xx_it.c中
 *
 *           FreeRTOS任务计时用了Tim14,20KHz.
 *           HAL库Time Base用了Tim7,1000Hz.
 *           
 * @author 	TRTX-gamer
 * @version 1.06
 * @date 	2025年6月1号19点23分
 */
#include "tim.h"

TIM_HandleTypeDef htim14; // 基础任务定时

extern uint32_t SystemCoreClock; // 系统时钟频率，初始化后发生变化

#if SYSTEM_SUPPORT_OS
TIM_HandleTypeDef htim7;
volatile uint32_t FreeRTOSRunTimeTicks = 0; // 系统任务计数
#endif

/**
 * @brief   TIM14 Initialization Function
 * @note
 *          定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
 * @param 	arr-Period
 * 			psc-Prescaler
 * @retval  None
 */
void TIM14_Init(uint16_t arr, uint16_t psc)
{

    /* USER CODE BEGIN TIM14_Init 0 */

    /* USER CODE END TIM14_Init 0 */

    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM14_Init 1 */

    /* USER CODE END TIM14_Init 1 */
    htim14.Instance = TIM14;
    htim14.Init.Prescaler = psc;
    htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim14.Init.Period = arr;
    htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim14, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_TIM_Base_Start_IT(&htim14) != HAL_OK)
    {
        Error_Handler();
    }
}

#if SYSTEM_SUPPORT_OS
/**
 * @brief  This function configures the TIM7 as a time base source.
 *         The time source is configured  to have 1ms time base with a dedicated
 *         Tick interrupt priority.
 * @note   This function is called  automatically at the beginning of program after
 *         reset by HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig().
 * @param  TickPriority: Tick interrupt priority.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    RCC_ClkInitTypeDef clkconfig;
    uint32_t uwTimclock, uwAPB1Prescaler = 0U;

    uint32_t uwPrescalerValue = 0U;
    uint32_t pFLatency;

    HAL_StatusTypeDef status;

    /* Enable TIM7 clock */
    __HAL_RCC_TIM7_CLK_ENABLE();

    /* Get clock configuration */
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

    /* Get APB1 prescaler */
    uwAPB1Prescaler = clkconfig.APB1CLKDivider;
    /* Compute TIM7 clock */
    if (uwAPB1Prescaler == RCC_HCLK_DIV1)
    {
        uwTimclock = HAL_RCC_GetPCLK1Freq();
    }
    else
    {
        uwTimclock = 2UL * HAL_RCC_GetPCLK1Freq();
    }

    /* Compute the prescaler value to have TIM7 counter clock equal to 1MHz */
    uwPrescalerValue = (uint32_t)((uwTimclock / 1000000U) - 1U);

    /* Initialize TIM7 */
    htim7.Instance = TIM7;

    /* Initialize TIMx peripheral as follow:
     * Period = [(TIM7CLK/1000) - 1]. to have a (1/1000) s time base.
     * Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
     * ClockDivision = 0
     * Counter direction = Up
     */
    htim7.Init.Period = (1000000U / 1000U) - 1U;
    htim7.Init.Prescaler = uwPrescalerValue;
    htim7.Init.ClockDivision = 0;
    htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    status = HAL_TIM_Base_Init(&htim7);
    if (status == HAL_OK)
    {
        /* Start the TIM time Base generation in interrupt mode */
        status = HAL_TIM_Base_Start_IT(&htim7);
        if (status == HAL_OK)
        {
            /* Enable the TIM7 global Interrupt */
            HAL_NVIC_EnableIRQ(TIM7_IRQn);
            /* Configure the SysTick IRQ priority */
            if (TickPriority < (1UL << __NVIC_PRIO_BITS))
            {
                /* Configure the TIM IRQ priority */
                HAL_NVIC_SetPriority(TIM7_IRQn, TickPriority, 0U);
                uwTickPrio = TickPriority;
            }
            else
            {
                status = HAL_ERROR;
            }
        }
    }

    /* Return function status */
    return status;
}

/**
 * @brief  Suspend Tick increment.
 * @note   Disable the tick increment by disabling TIM7 update interrupt.
 * @param  None
 * @retval None
 */
void HAL_SuspendTick(void)
{
    /* Disable TIM7 update Interrupt */
    __HAL_TIM_DISABLE_IT(&htim7, TIM_IT_UPDATE);
}

/**
 * @brief  Resume Tick increment.
 * @note   Enable the tick increment by Enabling TIM7 update interrupt.
 * @param  None
 * @retval None
 */
void HAL_ResumeTick(void)
{
    /* Enable TIM7 Update interrupt */
    __HAL_TIM_ENABLE_IT(&htim7, TIM_IT_UPDATE);
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
    TIM14_Init(50 - 1, (u16)(SystemCoreClock / 1000000 / 2 - 1)); // 定时器14初始化，90分频，计数50，为20KHz
}
#endif
