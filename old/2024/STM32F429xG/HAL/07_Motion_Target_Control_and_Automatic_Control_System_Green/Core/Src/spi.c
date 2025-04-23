/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    spi.c
 * @brief   This file provides code for the configuration
 *          of the SPI instances.
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
#include "spi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi3;
DMA_HandleTypeDef hdma_spi1_tx;
SPI_HandleTypeDef hspi6;
DMA_HandleTypeDef hdma_spi6_tx;

/* SPI1 init function */
void MX_SPI1_Init(void)
{

    /* USER CODE BEGIN SPI1_Init 0 */

    /* USER CODE END SPI1_Init 0 */

    /* USER CODE BEGIN SPI1_Init 1 */

    /* USER CODE END SPI1_Init 1 */
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;                      // 设置SPI工作模式：主机模式
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;            // 双线模式
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;                // 设置SPI数据大小：8位帧结构
    hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;             // 串行同步时钟空闲时SCLK位高电平
    hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;                  // 串行同步时钟空第2个时钟沿捕获
    hspi1.Init.NSS = SPI_NSS_SOFT;                          // NSS信号由软件管理
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8; // 波特率预分频值：波特率预分频值为2
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;                 // 数据传输高位先行
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10; // CRC值计算的多项式
    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI1_Init 2 */

    /* USER CODE END SPI1_Init 2 */
}

void MX_SPI3_Init(void)
{
    hspi3.Instance = SPI3;
    hspi3.Init.Mode = SPI_MODE_MASTER;                      // 设置SPI工作模式：主机模式
    hspi3.Init.Direction = SPI_DIRECTION_2LINES;            // 双线模式
    hspi3.Init.DataSize = SPI_DATASIZE_8BIT;                // 设置SPI数据大小：8位帧结构
    hspi3.Init.CLKPolarity = SPI_POLARITY_HIGH;             // 串行同步时钟空闲时SCLK位高电平
    hspi3.Init.CLKPhase = SPI_PHASE_2EDGE;                  // 串行同步时钟空第2个时钟沿捕获
    hspi3.Init.NSS = SPI_NSS_SOFT;                          // NSS信号由软件管理
    hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2; // 波特率预分频值：波特率预分频值为2
    hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;                 // 数据传输高位先行
    hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi3.Init.CRCPolynomial = 10; // CRC值计算的多项式
    if (HAL_SPI_Init(&hspi3) != HAL_OK)
    {
        Error_Handler();
    }
}

/* SPI6 init function */
void MX_SPI6_Init(void) // 因为OLED_RGB时钟周期不小于50ns，故降频
{

    /* USER CODE BEGIN SPI6_Init 0 */

    /* USER CODE END SPI6_Init 0 */

    /* USER CODE BEGIN SPI6_Init 1 */

    /* USER CODE END SPI6_Init 1 */
    hspi6.Instance = SPI6;
    hspi6.Init.Mode = SPI_MODE_MASTER;
    hspi6.Init.Direction = SPI_DIRECTION_2LINES;
    hspi6.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi6.Init.CLKPolarity = SPI_POLARITY_HIGH;
    hspi6.Init.CLKPhase = SPI_PHASE_2EDGE;
    hspi6.Init.NSS = SPI_NSS_SOFT;
    hspi6.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    hspi6.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi6.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi6.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi6.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi6) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI6_Init 2 */

    /* USER CODE END SPI6_Init 2 */
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *spiHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (spiHandle->Instance == SPI1)
    {
        /* USER CODE BEGIN SPI1_MspInit 0 */

        /* USER CODE END SPI1_MspInit 0 */
        /* SPI1 clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**SPI1 GPIO Configuration
        PA5     ------> SPI1_SCK
        PA7     ------> SPI1_MOSI
        */
        GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* SPI1 DMA Init */
        /* SPI1_TX Init */
        hdma_spi1_tx.Instance = DMA2_Stream3;                        // 数据流
        hdma_spi1_tx.Init.Channel = DMA_CHANNEL_3;                   // 通道
        hdma_spi1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;          // 传输方向
        hdma_spi1_tx.Init.PeriphInc = DMA_PINC_DISABLE;              // 外设是否递增
        hdma_spi1_tx.Init.MemInc = DMA_MINC_ENABLE;                  // 内存是否递增
        hdma_spi1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; // 外设数据大小
        hdma_spi1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;    // 内存数据大小
        hdma_spi1_tx.Init.Mode = DMA_CIRCULAR;                       // 模式选择
        hdma_spi1_tx.Init.Priority = DMA_PRIORITY_HIGH;              // 优先级
        hdma_spi1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;           // FIFO
        if (HAL_DMA_Init(&hdma_spi1_tx) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(spiHandle, hdmatx, hdma_spi1_tx);

        /* USER CODE BEGIN SPI1_MspInit 1 */

        /* USER CODE END SPI1_MspInit 1 */
    }
    if (spiHandle->Instance == SPI3)
    {
        /* SPI3 clock enable */
        __HAL_RCC_SPI3_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**SPI3 GPIO Configuration
        PB3     ------> SPI3_SCK
        PB4     ------> SPI3_MOSI
        PB5     ------> SPI3_MISO
        */
        GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        // /* SPI3 DMA Init */
        // /* SPI3_TX Init */
        // hdma_spi1_tx.Instance = DMA2_Stream3;                        // 数据流
        // hdma_spi1_tx.Init.Channel = DMA_CHANNEL_3;                   // 通道
        // hdma_spi1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;          // 传输方向
        // hdma_spi1_tx.Init.PeriphInc = DMA_PINC_DISABLE;              // 外设是否递增
        // hdma_spi1_tx.Init.MemInc = DMA_MINC_ENABLE;                  // 内存是否递增
        // hdma_spi1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; // 外设数据大小
        // hdma_spi1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;    // 内存数据大小
        // hdma_spi1_tx.Init.Mode = DMA_CIRCULAR;                       // 模式选择
        // hdma_spi1_tx.Init.Priority = DMA_PRIORITY_MEDIUM;            // 优先级
        // hdma_spi1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;           // FIFO
        // if (HAL_DMA_Init(&hdma_spi1_tx) != HAL_OK)
        // {
        //   Error_Handler();
        // }

        // __HAL_LINKDMA(spiHandle, hdmatx, hdma_spi1_tx);
    }
    else if (spiHandle->Instance == SPI6)
    {
        /* USER CODE BEGIN SPI6_MspInit 0 */

        /* USER CODE END SPI6_MspInit 0 */
        /* SPI6 clock enable */
        __HAL_RCC_SPI6_CLK_ENABLE();

        __HAL_RCC_GPIOG_CLK_ENABLE();
        /**SPI6 GPIO Configuration
        PG12     ------> SPI6_MISO
        PG13     ------> SPI6_SCK
        PG14     ------> SPI6_MOSI
        */
        GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI6;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

        /* SPI6 DMA Init */
        /* SPI6_TX Init */
        hdma_spi6_tx.Instance = DMA2_Stream5;
        hdma_spi6_tx.Init.Channel = DMA_CHANNEL_1;
        hdma_spi6_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_spi6_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_spi6_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_spi6_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_spi6_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_spi6_tx.Init.Mode = DMA_CIRCULAR;
        hdma_spi6_tx.Init.Priority = DMA_PRIORITY_HIGH;
        hdma_spi6_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_spi6_tx) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(spiHandle, hdmatx, hdma_spi6_tx);

        /* USER CODE BEGIN SPI6_MspInit 1 */

        /* USER CODE END SPI6_MspInit 1 */
    }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *spiHandle)
{

    if (spiHandle->Instance == SPI1)
    {
        /* USER CODE BEGIN SPI1_MspDeInit 0 */

        /* USER CODE END SPI1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SPI1_CLK_DISABLE();

        /**SPI1 GPIO Configuration
        PA5     ------> SPI1_SCK
        PA7     ------> SPI1_MOSI
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5 | GPIO_PIN_7);

        /* SPI1 DMA DeInit */
        HAL_DMA_DeInit(spiHandle->hdmatx);
        /* USER CODE BEGIN SPI1_MspDeInit 1 */

        /* USER CODE END SPI1_MspDeInit 1 */
    }
    if (spiHandle->Instance == SPI3)
    {
        /* USER CODE BEGIN SPI1_MspDeInit 0 */

        /* USER CODE END SPI1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SPI3_CLK_DISABLE();

        /**SPI1 GPIO Configuration
        PA5     ------> SPI1_SCK
        PA7     ------> SPI1_MOSI
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);

        /* SPI1 DMA DeInit */
        HAL_DMA_DeInit(spiHandle->hdmatx);
        /* USER CODE BEGIN SPI1_MspDeInit 1 */

        /* USER CODE END SPI1_MspDeInit 1 */
    }
    else if (spiHandle->Instance == SPI6)
    {
        /* USER CODE BEGIN SPI6_MspDeInit 0 */

        /* USER CODE END SPI6_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SPI6_CLK_DISABLE();

        /**SPI6 GPIO Configuration
        PG12     ------> SPI6_MISO
        PG13     ------> SPI6_SCK
        PG14     ------> SPI6_MOSI
        */
        HAL_GPIO_DeInit(GPIOG, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);

        /* SPI6 DMA DeInit */
        HAL_DMA_DeInit(spiHandle->hdmatx);
        /* USER CODE BEGIN SPI6_MspDeInit 1 */

        /* USER CODE END SPI6_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
