/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32f1xx_it.c
 * @brief   Interrupt Service Routines.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f1xx_it.h"
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

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
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
 * @brief This function handles Prefetch fault, memory access fault.
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
void SVC_Handler(void)
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
void PendSV_Handler(void)
{
    /* USER CODE BEGIN PendSV_IRQn 0 */

    /* USER CODE END PendSV_IRQn 0 */
    /* USER CODE BEGIN PendSV_IRQn 1 */

    /* USER CODE END PendSV_IRQn 1 */
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void)
{
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
    /* USER CODE BEGIN SysTick_IRQn 1 */

    /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
 * @brief 串口6中断服务程序
 */
void USART3_IRQHandler(void)
{
    HAL_UART_IRQHandler(&UART3_Handler);
}

/**
 * @brief This function handles DMA1 channel1 global interrupt.
 */
void DMA1_Channel1_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

    /* USER CODE END DMA1_Channel1_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_adc1);
    /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

    /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
 * @brief This function handles EXTI line[15:10] interrupts.
 */
void EXTI15_10_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI15_10_IRQn 0 */

    /* USER CODE END EXTI15_10_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12); // 调用中断处理公用函数
    /* USER CODE BEGIN EXTI15_10_IRQn 1 */

    /* USER CODE END EXTI15_10_IRQn 1 */
}

// 中断服务程序中需要做的事情
// 在HAL库中所有的外部中断服务函数都会调用此函数
//  GPIO_Pin:中断引脚号
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static u8 Delay_sum = 0;

    if (GPIO_Pin == GPIO_PIN_12)
    {
        Get_Angle(1); // 读取角度

        // Delay_sum++; // 延时计数

        Encoder_val[0] = -Read_Encoder(2); // 读取编码器，前进为正
        Encoder_val[1] = -Read_Encoder(4);
        if (rxvaluepack.bools[0] == 1)
        {
            static r, l;
            if (Angle_Balance > 45.0 || Angle_Balance < -45.0)
            {
                r = l = 0;
            }
            else
            {
                r = l = (int)positional_pid_compute(&motor1_blance_pid, Modle_Angle_Balance, Angle_Balance);
                r = l -= (int)positional_pid_compute(&motor1_velocity_pid, rxvaluepack.integers[0], Encoder_val[1] + Encoder_val[0]); // 正反馈，-=,目标速度为0

                r -= rxvaluepack.integers[1] * 25;
                l += rxvaluepack.integers[1] * 25;
                // i = (int)positional_pid_compute(&motor1_turn_pid, rxvaluepack.integers[0] + rxvaluepack.integers[1], Encoder_val[0]);

                if (r > 7199)
                    r = 7199;
                if (r < -7199)
                    r = -7199;
                if (l > 7199)
                    l = 7199;
                if (l < -7199)
                    l = -7199;
            }
            control_speed(r, l);
        }
        else
            control_speed(0, 0);
    }
}

// 控制左右轮速度,arr范围7199 ~ -7199
void control_speed(int arr_r, int arr_l)
{
    if (arr_r >= 0) // 正转
    {
        TB6612_AIN1_Clr(); // 右轮正转
        TB6612_AIN2_Set();
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, arr_r); // tim1，ch1大，左轮
    }
    else
    {
        TB6612_AIN1_Set(); // 右轮反转
        TB6612_AIN2_Clr();
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, -arr_r); // tim1，ch1大，左轮
    }

    if (arr_l >= 0) // 正转
    {
        TB6612_BIN1_Set(); // 左轮正转
        TB6612_BIN2_Clr();
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, arr_l); // tim1，ch1大，左轮
    }
    else
    {
        TB6612_BIN1_Clr(); // 左轮反转
        TB6612_BIN2_Set();
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, -arr_l); // tim1，ch1大，左轮
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
