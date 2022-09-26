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

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure DMA                                                              */
/*----------------------------------------------------------------------------*/

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
 * Enable DMA controller clock
 */
void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
}

void DMA2_Stream0_IRQHandler(void)
{
  if (__HAL_DMA_GET_FLAG(&hdma_adc1, DMA_FLAG_TCIF0_4))
  {

    __HAL_DMA_CLEAR_FLAG(&hdma_adc1, DMA_FLAG_TCIF0_4);
  }
}
void DMA2_Stream3_IRQHandler(void)
{
  if (__HAL_DMA_GET_FLAG(&hdma_spi1_tx, DMA_FLAG_TCIF3_7))
  {
    fps_num++;

    __HAL_DMA_CLEAR_FLAG(&hdma_spi1_tx, DMA_FLAG_TCIF3_7);
  }
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
