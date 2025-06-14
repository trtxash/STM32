/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32f4xx_it.h
 * @brief   This file contains the headers of the interrupt handlers.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_IT_CALLBACK_H
#define __STM32F4xx_IT_CALLBACK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"

    extern void HAL_DMA2D_TransferCpltCallback(DMA2D_HandleTypeDef *hdma2d);

#if SYSTEM_SUPPORT_OS

#endif

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_IT_CALLBACK_H */
