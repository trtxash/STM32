/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    ltdc.c
 * @brief   This file provides code for the configuration
 *          of the LTDC instances.
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
/* Includes ------------------------------------------------------------------*/
#include "ltdc.h"
#include "lcd.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

LTDC_HandleTypeDef hltdc;

/* LTDC init function */
void LTDC_Init(void)
{

    /* USER CODE BEGIN LTDC_Init 0 */

    /* USER CODE END LTDC_Init 0 */

    /* USER CODE BEGIN LTDC_Init 1 */

    lcdltdc.pwidth = ACTIVE_WIDTH;              // 面板宽度,单位:像素
    lcdltdc.pheight = ACTIVE_HEIGHT;            // 面板高度,单位:像素
    lcdltdc.hsw = HORIZONTAL_SYNCHRONOUS_PULSE; // 水平同步宽度
    lcdltdc.vsw = VERTICAL_SYNCHRONOUS_PULSE;   // 垂直同步宽度
    lcdltdc.hbp = HORIZONTAL_BACK_PORCH;        // 水平后廊
    lcdltdc.vbp = VERTICAL_BACK_PORCH;          // 垂直后廊
    lcdltdc.hfp = HORIZONTAL_FRONT_PORCH;       // 水平前廊
    lcdltdc.vfp = VERTICAL_FRONT_PORCH;         // 垂直前廊

#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 || LCD_PIXFORMAT == LCD_PIXFORMAT_RGB888
    lcdltdc.pixsize = 4; // 颜色字节数
#else
    lcdltdc.pixsize = 2; // 颜色字节数
#endif
    /* USER CODE END LTDC_Init 1 */
    hltdc.Instance = LTDC;
    hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;                                                                               // 水平同步极性
    hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;                                                                               // 垂直同步极性
    hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;                                                                               // 数据使能极性
    hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;                                                                              // 像素时钟极性
    hltdc.Init.HorizontalSync = HORIZONTAL_SYNCHRONOUS_PULSE - 1;                                                             // 水平同步宽度,单位为相素时钟（CLK）个数
    hltdc.Init.VerticalSync = VERTICAL_SYNCHRONOUS_PULSE - 1;                                                                 // 垂直同步高度，单位为行周期个数
    hltdc.Init.AccumulatedHBP = HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH - 1;                                     // 水平同步后沿宽度，表示水平同步信号开始到行有效数据开始之间的相素时钟（CLK）个数
    hltdc.Init.AccumulatedVBP = VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH - 1;                                         // 垂直同步后沿高度，表示垂直同步信号后，无效行的个数
    hltdc.Init.AccumulatedActiveW = HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH + ACTIVE_WIDTH - 1;                  // 累加有效宽度
    hltdc.Init.AccumulatedActiveH = VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH + ACTIVE_HEIGHT - 1;                     // 累加有效高度
    hltdc.Init.TotalWidth = HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH + ACTIVE_WIDTH + HORIZONTAL_FRONT_PORCH - 1; // 总宽度
    hltdc.Init.TotalHeigh = VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH + ACTIVE_HEIGHT + VERTICAL_FRONT_PORCH - 1;      // 总高度
    hltdc.Init.Backcolor.Blue = 0x00;                                                                                         // 屏幕背景层颜色
    hltdc.Init.Backcolor.Green = 0x00;
    hltdc.Init.Backcolor.Red = 0x00;
    if (HAL_LTDC_Init(&hltdc) != HAL_OK)
    {
        Error_Handler();
    }

    // 层配置
    LTDC_Layer_Parameter_Config(0, (u32)ltdc_framebuf[0], LCD_PIXFORMAT, 255, 0, 6, 7, 0X000000); // 层参数配置
    LTDC_Layer_Window_Config(0, 0, 0, lcdltdc.pwidth, lcdltdc.pheight);                           // 层窗口配置,以LCD面板坐标系为基准,不要随便修改!

    LTDC_Display_Dir(1);
    LTDC_Select_Layer(0);   // 选择第1层
    LCD_BLK_Set();          // 点亮背光
    LTDC_Clear(0XFFFFFFFF); // 清屏

    /* USER CODE BEGIN LTDC_Init 2 */

    /* USER CODE END LTDC_Init 2 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
