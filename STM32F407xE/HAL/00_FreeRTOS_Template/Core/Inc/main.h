/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"

#define Debug 1 // 控制Debug的一些相关函数

#define START_TASK_PRIO 1              //任务优先级
#define START_STK_SIZE 128             //任务堆栈大小
  TaskHandle_t StartTask_Handler;      //任务句柄
  void start_task(void *pvParameters); //任务函数

#define LED0_TASK_PRIO 2              //任务优先级
#define LED0_STK_SIZE 50              //任务堆栈大小
  TaskHandle_t LED0Task_Handler;      //任务句柄
  void led0_task(void *pvParameters); //任务函数

#define LED1_TASK_PRIO 3              //任务优先级
#define LED1_STK_SIZE 50              //任务堆栈大小
  TaskHandle_t LED1Task_Handler;      //任务句柄
  void led1_task(void *pvParameters); //任务函数

#define FLOAT_TASK_PRIO 4              //任务优先级
#define FLOAT_STK_SIZE 256             //任务堆栈大小
  TaskHandle_t FLOATTask_Handler;      //任务句柄
  void float_task(void *pvParameters); //任务函数

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
