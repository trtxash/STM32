/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    dma.c
 * @brief   This file provides code for the configuration
 *          of all the requested memory to memory DMA transfers.
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
#include "dma.h"

/**
 * Enable DMA controller clock
 */
void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */

  /* DMA1_Stream6_IRQn interrupt configuration */
  // HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 3, 0);
  // HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

  /* DMA2_Stream0_IRQn interrupt configuration */
  // HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 8, 0);
  // HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

  /* DMA2_Stream3_IRQn interrupt configuration */
  // HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 3, 1);
  // HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
}

void DMA1_Stream6_IRQHandler(void) // I2C1
{
  if (__HAL_DMA_GET_FLAG(&hdma_i2c1_tx, DMA_FLAG_TCIF2_6))
  {
    fps_num++;

    __HAL_DMA_CLEAR_FLAG(&hdma_i2c1_tx, DMA_FLAG_TCIF2_6);
  }
}

void DMA2_Stream0_IRQHandler(void) // ADC1
{
  if (__HAL_DMA_GET_FLAG(&hdma_adc1, DMA_FLAG_TCIF0_4))
  {
    // value_v[i] = (float)adcx[0] * 3.3 / 4096;                          // 电压值
    // value_t[i] = ((float)adcx[1] * (3.3 / 4096) - 0.76) / 0.0025 + 20; // 转换为温度值

    // i++; // 计数

    // if (i >= ADC_Sec)
    // {
    //   i = 0;
    // }

    __HAL_DMA_CLEAR_FLAG(&hdma_adc1, DMA_FLAG_TCIF0_4);
  }
}

void DMA2_Stream3_IRQHandler(void) // SPI1，太快了，中断还是不开好
{
  if (__HAL_DMA_GET_FLAG(&hdma_spi1_tx, DMA_FLAG_TCIF3_7))
  {
    fps_num++;

    __HAL_DMA_CLEAR_FLAG(&hdma_spi1_tx, DMA_FLAG_TCIF3_7);
  }
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
