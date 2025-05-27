/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    adc.c
 * @brief   This file provides code for the configuration
 *          of the ADC instances.
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
#include "adc.h"

/* USER CODE BEGIN 0 */

volatile u32 adcx[ADC_Sec];

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* ADC1 init function */
void ADC1_Init(void)
{

    /* USER CODE BEGIN ADC1_Init 0 */

    /* USER CODE END ADC1_Init 0 */

    ADC_ChannelConfTypeDef sConfig = {0};

    /* USER CODE BEGIN ADC1_Init 1 */

    /* USER CODE END ADC1_Init 1 */
    /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
     */
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;            // 8分频，ADCCLK=PCLK2/8=120/8=15MHZ
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;                      // 12位模式
    hadc1.Init.ScanConvMode = ENABLE;                                // 扫描模式
    hadc1.Init.ContinuousConvMode = ENABLE;                          // 连续转换
    hadc1.Init.DiscontinuousConvMode = DISABLE;                      // 禁止不连续采样模式
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; // 使用软件触发
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;                // 软件触发
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;                      // 右对齐
    hadc1.Init.NbrOfConversion = 2;                                  // 几个转换在规则序列中
    hadc1.Init.NbrOfDiscConversion = 1;                              // 不连续采样通道数为1
    hadc1.Init.DMAContinuousRequests = ENABLE;                       // DMA请求
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;                   // 转换结束标志连续生成？关闭EOC中断？
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;        // 通道
    sConfig.Rank = 1;                                // 第1个序列，序列1
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES; // 采样时间
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfig.Rank = 2; // 第2个序列，序列2
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */
}

/* USER CODE BEGIN 1 */

// void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
// {
//     printf("DMA transfer completern");
// }

// void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
// {
//     printf("DMA Half transfer completern");
// }

// void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
// {
//     printf("DMA transfer errorrn");
// }

/* USER CODE END 1 */
