/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32f4xx_it.c
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
#include "stm32f4xx_it.h"
#include "main.h"
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

/* USER CODE BEGIN 1 */

/**
 * @brief 串口1中断服务程序
 */
void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&UART1_Handler);
}

/**
 * @brief 串口6中断服务程序
 */
void USART6_IRQHandler(void)
{
    HAL_UART_IRQHandler(&UART6_Handler);
}

/*
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);//发送完成回调函数
void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart);//发送完成过半
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);//接收完成回调函数
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart);//接收完成过半
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);//错误处理回调函数
*/
/**
 * @brief 串口接收完成回调函数
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) // 如果是串口1
    {
        // aRxBuffer[0] = USART1->DR;		  // 读取可以自动清楚RXNE,这里用了HAL_UART_Receive_IT函数，这里不需要
        if ((USART_RX_STA & 0x8000) == 0) // 接收未完成
        {
            if (USART_RX_STA & 0x4000) // 接收到了0x0d
            {
                if (aRxBuffer[0] != 0x0a)
                    USART_RX_STA = 0; // 接收错误,重新开始
                else
                    USART_RX_STA |= 0x8000; // 接收完成了
            }
            else // 还没收到0X0D
            {
                if (aRxBuffer[0] == 0x0d)
                    USART_RX_STA |= 0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA & 0X3FFF] = aRxBuffer[0];
                    USART_RX_STA++;
                    if (USART_RX_STA > (USART_REC_LEN - 1))
                    {
                        USART_RX_STA = 0; // 接收数据错误,重新开始接收
                        Error_sum++;
                    }
                }
            }
        }

        HAL_UART_Receive_IT(&UART1_Handler, aRxBuffer, RXBUFFERSIZE); // Receive_IT中会关闭中断，需要重开
    }
    else if (huart->Instance == USART6) // 如果是串口6
    {
        AnoPTv8HwRecvByte(bRxBuffer[0]);
        HAL_UART_Receive_IT(&UART6_Handler, bRxBuffer, RXBUFFERSIZE); // 如果要调用处理回调函数，用这个函数使能接收中断
    }
}

/**
 * @brief This function handles TIM6 global interrupt, DAC1 and DAC2 underrun error interrupts.
 */
void TIM6_DAC_IRQHandler(void)
{
    /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

    /* USER CODE END TIM6_DAC_IRQn 0 */
    HAL_TIM_IRQHandler(&htim6);
    /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

    /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
 * @brief This function handles TIM6 global interrupt, DAC1 and DAC2 underrun error interrupts.
 */
void TIM7_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim7);
}

/**
 * @brief   定时器更新回调函数
 * @note    void HAL_TIM_IRQHandler(TIM_HandleTypeDef *htim)；调用，其会自动清除中断标志
 * @param   *htim
 * @retval  None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&htim6))
    {
        static float v_b = 0;
        short temp = 0;

        time_flag = 1;
        /* 每50毫秒读取编码器数值 */
        Encoder[0] = -Read_Encoder(&htim2);
        Encoder[1] = -Read_Encoder(&htim3);
        // 位置积分
        Location_sum = Location_integral((short)((Encoder[0] + Encoder[1]) / 2));
        // 位置环速度限幅,决定速度
        motor12_location.output_max = TARGET_V;
        motor12_location.output_min = -TARGET_V;
        // PID位置环计算
        vl = vr = positional_pid_compute(&motor12_location, TARGET_LOCATION, Location_sum);
        // 巡线
        if (LINE_FLAG)
        {
            // 巡线PID计算
            temp = positional_pid_compute(&xunxian, 0, gray_sensor_sum_val);
            vr += temp;
            vl -= temp;
        }
        // PID转向环计算
        temp = positional_pid_compute(&motor_turn, TARGET_ANGLE, Yaw);
        vr += temp;
        vl -= temp;
        // PID速度环计算
        if (motor12_location.control == DISABLE & motor_turn.control == DISABLE) // 调试用
            vl = vr = TARGET_V;
        vl = positional_pid_compute(&motor1_velocity, vl, Encoder[0]);
        vr = positional_pid_compute(&motor2_velocity, vr, Encoder[1]);

        TB6612_control_speed(vr, vl);

        AnoPTv8SendBuf(ANOPTV8_SWJID, 0xF1, databuf, ANOPTV8_PARNUM_UPPER); // 数据发送到上位机
    }
    else if (htim == (&htim7))
    {
        AnoPTv8HwTrigger1ms(); // 上位机1ms调用
    }
}

/**
 * @brief This function handles DMA2 stream1 global interrupt.
 */
void DMA2_Stream1_IRQHandler(void)
{
    /* USER CODE BEGIN DMA2_Stream1_IRQn 0 */

    /* USER CODE END DMA2_Stream1_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_usart6_rx);
    /* USER CODE BEGIN DMA2_Stream1_IRQn 1 */

    /* USER CODE END DMA2_Stream1_IRQn 1 */
}

/**
 * @brief This function handles DMA2 stream6 global interrupt.
 */
void DMA2_Stream6_IRQHandler(void)
{
    /* USER CODE BEGIN DMA2_Stream6_IRQn 0 */

    /* USER CODE END DMA2_Stream6_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_usart6_tx);
    /* USER CODE BEGIN DMA2_Stream6_IRQn 1 */

    /* USER CODE END DMA2_Stream6_IRQn 1 */
}

/* USER CODE END 1 */
