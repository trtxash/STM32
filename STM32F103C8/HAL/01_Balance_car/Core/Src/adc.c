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

// 锂离子电池电量一般为v = 9413.0184 - 303.204 * j * j * j * j * j + 114.0062 * j * j * j * j * j * j * j - 44.7181 * j * j * j * j * j * j * j * j + 6.547055 * j * j * j * j * j * j * j * j * j - 0.336029617 * j * j * j * j * j * j * j * j * j * j;j为电压
#include "adc.h"

/* USER CODE BEGIN 0 */

u16 adcx[2][ADC_Sec];

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

    /* USER CODE BEGIN ADC1_Init 0 */

    /* USER CODE END ADC1_Init 0 */

    ADC_ChannelConfTypeDef sConfig = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /* USER CODE BEGIN ADC1_Init 1 */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) // ADC时钟分频，设置为12M
    {
        Error_Handler();
    }
    /* USER CODE END ADC1_Init 1 */
    /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
     */
    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;       // 扫描模式
    hadc1.Init.ContinuousConvMode = ENABLE;           // 连续转换
    hadc1.Init.DiscontinuousConvMode = DISABLE;       // 禁止不连续采样模式
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START; // 软件触发
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;       // 右对齐
    hadc1.Init.NbrOfConversion = 1;                   // 1个转换在规则序列中 也就是只转换规则序列1
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_6;                   // 通道
    sConfig.Rank = ADC_REGULAR_RANK_1;                 // 第1个序列，序列1
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5; // 采样时间
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    // sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;          // 通道
    // sConfig.Rank = 2;                                  // 第2个序列，序列2
    // sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5; // 采样时间
    // if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    // {
    //     Error_Handler();
    // }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *adcHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (adcHandle->Instance == ADC1)
    {
        /* USER CODE BEGIN ADC1_MspInit 0 */

        /* USER CODE END ADC1_MspInit 0 */
        /* ADC1 clock enable */
        __HAL_RCC_ADC1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**ADC1 GPIO Configuration
        PA1     ------> ADC1_IN6
        */
        GPIO_InitStruct.Pin = GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* ADC1 DMA Init */
        /* ADC1 Init */
        hdma_adc1.Instance = DMA1_Channel1;                           // 数据流
        hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;              // 传输方向
        hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;                  // 外设是否递增
        hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;                      // 内存是否递增
        hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; // 外设数据大小
        hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;    // 内存数据大小
        hdma_adc1.Init.Mode = DMA_CIRCULAR;                           // 模式选择
        hdma_adc1.Init.Priority = DMA_PRIORITY_MEDIUM;                // 优先级
        if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(adcHandle, DMA_Handle, hdma_adc1);

        /* USER CODE BEGIN ADC1_MspInit 1 */

        /* USER CODE END ADC1_MspInit 1 */
    }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *adcHandle)
{

    if (adcHandle->Instance == ADC1)
    {
        /* USER CODE BEGIN ADC1_MspDeInit 0 */

        /* USER CODE END ADC1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC1_CLK_DISABLE();

        /**ADC1 GPIO Configuration
        PA1     ------> ADC1_IN1
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6);

        /* ADC1 DMA DeInit */
        HAL_DMA_DeInit(adcHandle->DMA_Handle);
        /* USER CODE BEGIN ADC1_MspDeInit 1 */

        /* USER CODE END ADC1_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

// 获得ADC值
// ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
// 返回值:转换结果
u16 Get_Adc(u32 ch)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    sConfig.Channel = ch;                              // 通道
    sConfig.Rank = 1;                                  // 第1个序列，序列1
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5; // 采样时间

    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    HAL_ADC_Start(&hadc1);                 // 开启ADC
    HAL_ADC_PollForConversion(&hadc1, 10); // 轮询转换

    return (u16)HAL_ADC_GetValue(&hadc1); // 返回最近一次ADC1规则组的转换结果
}

// 获取指定通道的转换值，取times次,然后平均
// times:获取次数
// 返回值:通道ch的times次转换结果平均值
u16 Get_Adc_Average(u32 ch, u8 times)
{
    u32 temp_val = 0;
    u8 t;
    for (t = 0; t < times; t++)
    {
        temp_val += Get_Adc(ch);
    }
    return temp_val / times;
}

// dma下获取指定通道的值，返回是ADC_Sec*2次扫描的平均值
u16 Get_Adc_Average_DMA(void)
{
    u8 i;
    u32 value_temp;
    for (i = 0, value_temp = 0; i < ADC_Sec; i++)
    {
        value_temp += adcx[0][i];
    }
    for (i = 0; i < ADC_Sec; i++)
    {
        value_temp += adcx[1][i];
    }
    return (u16)(value_temp / (ADC_Sec * 2));
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    // printf("DMA transfer completern");
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
    // printf("DMA Half transfer completern");
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
    // printf("DMA transfer errorrn");
}

/* USER CODE END 1 */