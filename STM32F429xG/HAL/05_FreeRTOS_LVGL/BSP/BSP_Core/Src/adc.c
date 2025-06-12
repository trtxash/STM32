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

volatile u32 adcx[ADC_Ch][ADC_Sec];

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
    hadc1.Init.ScanConvMode = DISABLE;                               // 扫描模式
    hadc1.Init.ContinuousConvMode = ENABLE;                          // 连续转换
    hadc1.Init.DiscontinuousConvMode = DISABLE;                      // 禁止不连续采样模式
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; // 使用软件触发
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;                // 软件触发
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;                      // 右对齐
    hadc1.Init.NbrOfConversion = 2;                                  // 几个转换在规则序列中
    hadc1.Init.NbrOfDiscConversion = DISABLE;                        // 不连续采样通道数为1
    hadc1.Init.DMAContinuousRequests = ENABLE;                       // DMA请求
    hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;                      // 转换结束标志连续生成？关闭EOC中断？
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    sConfig.Channel = ADC_CHANNEL_TEMPSENSOR; // 内部温度通道
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES; // 采样时间
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    // sConfig.Channel = ADC_CHANNEL_15; // 电池电压测量通道
    // sConfig.Rank = 2;
    // sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES; // 采样时间
    // if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    // {
    //     Error_Handler();
    // }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */
}

void ADC_Change_Channel_SingleScan_DMA_NOMAL(ADC_HandleTypeDef *hadc, u8 ch, u16 SamplingTime)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    // 停止当前转换
    HAL_ADC_Stop_DMA(hadc);

    // 配置通道参数
    switch (ch)
    {
    case 0:
        sConfig.Channel = ADC_CHANNEL_0;
        break;
    case 1:
        sConfig.Channel = ADC_CHANNEL_1;
        break;
    case 2:
        sConfig.Channel = ADC_CHANNEL_2;
        break;
    case 3:
        sConfig.Channel = ADC_CHANNEL_3;
        break;
    case 4:
        sConfig.Channel = ADC_CHANNEL_4;
        break;
    case 5:
        sConfig.Channel = ADC_CHANNEL_5;
        break;
    case 6:
        sConfig.Channel = ADC_CHANNEL_6;
        break;
    case 7:
        sConfig.Channel = ADC_CHANNEL_7;
        break;
    case 8:
        sConfig.Channel = ADC_CHANNEL_8;
        break;
    case 9:
        sConfig.Channel = ADC_CHANNEL_9;
        break;
    case 10:
        sConfig.Channel = ADC_CHANNEL_10;
        break;
    case 11:
        sConfig.Channel = ADC_CHANNEL_11;
        break;
    case 12:
        sConfig.Channel = ADC_CHANNEL_12;
        break;
    case 13:
        sConfig.Channel = ADC_CHANNEL_13;
        break;
    case 14:
        sConfig.Channel = ADC_CHANNEL_14;
        break;
    case 15:
        sConfig.Channel = ADC_CHANNEL_15;
        break;
    case 16:
        sConfig.Channel = ADC_CHANNEL_16;
        break;
    case 17:
        sConfig.Channel = ADC_CHANNEL_17;
        break;
    case 18:
        sConfig.Channel = ADC_CHANNEL_18;
        break;
    default:
        break;
    }

    switch (SamplingTime)
    {
    case 3:
        sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
        break;
    case 15:
        sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
        break;
    case 28:
        sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
        break;
    case 56:
        sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;
        break;
    case 84:
        sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
        break;
    case 112:
        sConfig.SamplingTime = ADC_SAMPLETIME_112CYCLES;
        break;
    case 144:
        sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
        break;
    case 480:
        sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
        break;
    default:
        sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
        break;
    }

    sConfig.Rank = 1; // 规则组中的转换顺序
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler(); // 通道配置失败时处理
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
