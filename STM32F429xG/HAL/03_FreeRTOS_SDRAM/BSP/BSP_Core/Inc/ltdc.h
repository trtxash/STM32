/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    ltdc.h
 * @brief   This file contains all the function prototypes for
 *          the ltdc.c file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#ifndef __LTDC_H__
#define __LTDC_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

    extern LTDC_HandleTypeDef hltdc;

    void LTDC_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __LTDC_H__ */
