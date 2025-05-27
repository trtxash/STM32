/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : FMC.c
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

/* Includes ------------------------------------------------------------------*/
#include "fmc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SDRAM_HandleTypeDef hsdram1;

/* FMC initialization function */
void FMC_Init(void)
{
    /* USER CODE BEGIN FMC_Init 0 */

    /* USER CODE END FMC_Init 0 */

    FMC_SDRAM_TimingTypeDef SdramTiming = {0};

    /* USER CODE BEGIN FMC_Init 1 */

    /* USER CODE END FMC_Init 1 */

    /** Perform the SDRAM1 memory initialization sequence
     */
    hsdram1.Instance = FMC_SDRAM_DEVICE; // SDRAM在BANK5,6
    /* hsdram1.Init */
    hsdram1.Init.SDBank = FMC_SDRAM_BANK1;                             // 第一个SDRAM BANK
    hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;       // 列数量
    hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;            // 行数量
    hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;         // 数据宽度为16位
    hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;    // 一共4个BANK
    hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;                 // CAS为3
    hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE; // 失能写保护
    hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;             // SDRAM时钟为HCLK/2=240/2=120M=8.3ns
    hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;                  // 使能突发
    hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;              // 读通道延时

    /* SdramTiming */

    // SdramTiming.LoadToActiveDelay = 3;    // TMRD:加载模式寄存器到激活时间的延迟,min=
    // SdramTiming.ExitSelfRefreshDelay = 10; // TXSR:退出自刷新延迟,min=72ns
    // SdramTiming.SelfRefreshTime = 7;      // TRAS:自刷新时间为,min=42ns
    // SdramTiming.RowCycleDelay = 9;        // TRC:行循环延迟为,min=60ns
    // SdramTiming.WriteRecoveryTime = 3;    // TWR:恢复延迟,min=2*Tck,Tck为SDRAM时钟周期
    // SdramTiming.RPDelay = 3;              // TRP:行预充电延迟,min=15ns
    // SdramTiming.RCDDelay = 3;             // TRCD:行到列延迟,min=15ns

    SdramTiming.LoadToActiveDelay = 2;    // TMRD:加载模式寄存器到激活时间的延迟,min=
    SdramTiming.ExitSelfRefreshDelay = 9; // TXSR:退出自刷新延迟,min=72ns
    SdramTiming.SelfRefreshTime = 6;      // TRAS:自刷新时间为,min=42ns
    SdramTiming.RowCycleDelay = 8;        // TRC:行循环延迟为,min=60ns
    SdramTiming.WriteRecoveryTime = 2;    // TWR:恢复延迟,min=2*Tck,Tck为SDRAM时钟周期
    SdramTiming.RPDelay = 2;              // TRP:行预充电延迟,min=15ns
    SdramTiming.RCDDelay = 2;             // TRCD:行到列延迟,min=15ns

    if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
    {
        Error_Handler();
    }

    /* USER CODE BEGIN FMC_Init 2 */

    /* USER CODE END FMC_Init 2 */
}
