/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    i2c.c
 * @brief   This file provides code for the configuration
 *          of the I2C instances.
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
#include "i2c.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_tx;
DMA_HandleTypeDef hdma_i2c1_rx;

/* I2C1 init function */
void I2C1_Init(void)
{

    /* USER CODE BEGIN I2C1_Init 0 */

    /* USER CODE END I2C1_Init 0 */

    /* USER CODE BEGIN I2C1_Init 1 */

    /* USER CODE END I2C1_Init 1 */
    hi2c1.Instance = I2C1;

    // 注意：实际时钟需满足公式：I2C_Speed ≤ APB1时钟/4（当DutyCycle=2时）
    hi2c1.Init.ClockSpeed = 400000; // 最大1.2Mbit/s

    // 快速模式占空比选择：
    // I2C_DUTYCYCLE_16_9 (t_low/t_high=16/9)
    // I2C_DUTYCYCLE_2 (t_low/t_high=2)
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;

    /* 地址配置 (主模式可忽略) */
    hi2c1.Init.OwnAddress1 = 0;                           // 主模式时从地址无效，建议置0
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;  // 7位地址模式
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; // 禁用双地址模式
    hi2c1.Init.OwnAddress2 = 0;
    /* 高级功能配置 */
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; // 禁用广播呼叫响应
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;     // 允许时钟拉伸, 使能时从机可拉低SCL延长传输
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Analogue filter
     */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Digital filter
     */
    // 过滤数字信号抖动
    // 数字滤波器系数=1时，采样周期=1/(2*I2C_SPEED)
    // 400kHz时采样间隔1.25μs，可过滤<0.5μs的抖动
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C1_Init 2 */

    /* USER CODE END I2C1_Init 2 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
