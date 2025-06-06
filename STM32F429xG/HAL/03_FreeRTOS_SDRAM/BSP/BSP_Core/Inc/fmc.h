/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : FMC.h
 * Description        : This file provides code for the configuration
 *                      of the FMC peripheral.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#ifndef __FMC_H
#define __FMC_H
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

    /* USER CODE BEGIN Includes */

    /* USER CODE END Includes */

    extern SDRAM_HandleTypeDef hsdram1;

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

    void FMC_Init(void);

    /* USER CODE BEGIN Prototypes */

    /* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__FMC_H */
