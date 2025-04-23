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

TIM_HandleTypeDef htim14; // 基础任务定时

extern uint32_t SystemCoreClock; // 系统时钟频率，初始化后发生变化
#if SYSTEM_SUPPORT_OS
volatile unsigned long long FreeRTOSRunTimeTicks = 0; // 系统任务计数
#endif
/**
 * @brief   TIM7 Initialization Function
 * @note
 *          定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
 * @param 	arr-Period
 * 			psc-Prescaler
 * @retval  None
 */
void TIM14_Init(uint16_t arr, uint16_t psc)
{

    /* USER CODE BEGIN TIM7_Init 0 */

    /* USER CODE END TIM7_Init 0 */

    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM7_Init 1 */

    /* USER CODE END TIM7_Init 1 */
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
