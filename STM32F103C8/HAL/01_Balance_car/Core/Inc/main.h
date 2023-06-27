/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "valuepack.h"

#include "dma.h"
#include "adc.h"
#include "tim.h"
#include "encoder.h"
#include "positional_pid.h"

#include "oled.h"
#include "tb6612.h"

#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
    // #include "stm3210e_eval.h"

    extern positional_pid_params_t motor1_blance_pid;
    extern float battery_v;      // 单电池电压
    extern double battery_v_100; // 电池容量百分比

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
