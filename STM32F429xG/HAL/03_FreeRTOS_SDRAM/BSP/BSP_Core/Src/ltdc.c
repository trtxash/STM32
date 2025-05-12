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

uint16_t *ltdc_lcd_framebuf0 = (uint16_t *)LCD_FRAME_BUF_ADDR;

// // 根据不同的颜色格式,定义帧缓存数组
// #if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 || LCD_PIXFORMAT == LCD_PIXFORMAT_RGB888
// u32 ltdc_lcd_framebuf[1280][800] __attribute__((at(LCD_FRAME_BUF_ADDR))); // 定义最大屏分辨率时,LCD所需的帧缓存数组大小
// #else
// u16 ltdc_lcd_framebuf[1280][800] __attribute__((at(LCD_FRAME_BUF_ADDR))); // 定义最大屏分辨率时,LCD所需的帧缓存数组大小
// #endif

// // LTDC,基本参数设置.
// // 注意:此函数,必须在LTDC_Layer_Window_Config之前设置.
// // layerx:层值,0/1.
// // bufaddr:层颜色帧缓存起始地址
// // pixformat:颜色格式.0,ARGB8888;1,RGB888;2,RGB565;3,ARGB1555;4,ARGB4444;5,L8;6;AL44;7;AL88
// // alpha:层颜色Alpha值,0,全透明;255,不透明
// // alpha0:默认颜色Alpha值,0,全透明;255,不透明
// // bfac1:混合系数1,4(100),恒定的Alpha;6(101),像素Alpha*恒定Alpha
// // bfac2:混合系数2,5(101),恒定的Alpha;7(111),像素Alpha*恒定Alpha
// // bkcolor:层默认颜色,32位,低24位有效,RGB888格式
// // 返回值:无
// void LTDC_Layer_Parameter_Config(u8 layerx, u32 bufaddr, u8 pixformat, u8 alpha, u8 alpha0, u8 bfac1, u8 bfac2, u32 bkcolor)
// {
//     LTDC_LayerCfgTypeDef pLayerCfg;

//     pLayerCfg.WindowX0 = 0;                                      // 窗口起始X坐标
//     pLayerCfg.WindowY0 = 0;                                      // 窗口起始Y坐标
//     pLayerCfg.WindowX1 = lcdltdc.pwidth;                         // 窗口终止X坐标
//     pLayerCfg.WindowY1 = lcdltdc.pheight;                        // 窗口终止Y坐标
//     pLayerCfg.PixelFormat = pixformat;                           // 像素格式
//     pLayerCfg.Alpha = alpha;                                     // Alpha值设置，0~255,255为完全不透明
//     pLayerCfg.Alpha0 = alpha0;                                   // 默认Alpha值
//     pLayerCfg.BlendingFactor1 = (u32)bfac1 << 8;                 // 设置层混合系数
//     pLayerCfg.BlendingFactor2 = (u32)bfac2 << 8;                 // 设置层混合系数
//     pLayerCfg.FBStartAdress = bufaddr;                           // 设置层颜色帧缓存起始地址
//     pLayerCfg.ImageWidth = lcdltdc.pwidth;                       // 设置颜色帧缓冲区的宽度
//     pLayerCfg.ImageHeight = lcdltdc.pheight;                     // 设置颜色帧缓冲区的高度
//     pLayerCfg.Backcolor.Red = (u8)(bkcolor & 0X00FF0000) >> 16;  // 背景颜色红色部分
//     pLayerCfg.Backcolor.Green = (u8)(bkcolor & 0X0000FF00) >> 8; // 背景颜色绿色部分
//     pLayerCfg.Backcolor.Blue = (u8)bkcolor & 0X000000FF;         // 背景颜色蓝色部分
//     HAL_LTDC_ConfigLayer(&LTDC_Handler, &pLayerCfg, layerx);     // 设置所选中的层
// }

/* LTDC init function */
void MX_LTDC_Init(void)
{

    /* USER CODE BEGIN LTDC_Init 0 */

    /* USER CODE END LTDC_Init 0 */

    LTDC_LayerCfgTypeDef pLayerCfg = {0};
    // LTDC_LayerCfgTypeDef pLayerCfg1 = {0};

    /* USER CODE BEGIN LTDC_Init 1 */

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
    hltdc.Init.Backcolor.Blue = 0xFF;                                                                                         // 屏幕背景层颜色
    hltdc.Init.Backcolor.Green = 0xFF;
    hltdc.Init.Backcolor.Red = 0xFF;
    if (HAL_LTDC_Init(&hltdc) != HAL_OK)
    {
        Error_Handler();
    }

    pLayerCfg.WindowX0 = 0;
    pLayerCfg.WindowX1 = ACTIVE_WIDTH;
    pLayerCfg.WindowY0 = 0;
    pLayerCfg.WindowY1 = ACTIVE_HEIGHT;
    pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
    pLayerCfg.Alpha = 0;
    pLayerCfg.Alpha0 = 0;
    pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
    pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
    pLayerCfg.FBStartAdress = (uint32_t)ltdc_lcd_framebuf0;
    pLayerCfg.ImageWidth = ACTIVE_WIDTH;
    pLayerCfg.ImageHeight = ACTIVE_HEIGHT;
    pLayerCfg.Backcolor.Blue = 0;
    pLayerCfg.Backcolor.Green = 0;
    pLayerCfg.Backcolor.Red = 0;
    if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
    {
        Error_Handler();
    }
    // pLayerCfg1.WindowX0 = 0;
    // pLayerCfg1.WindowX1 = 0;
    // pLayerCfg1.WindowY0 = 0;
    // pLayerCfg1.WindowY1 = 0;
    // pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
    // pLayerCfg1.Alpha = 0;
    // pLayerCfg1.Alpha0 = 0;
    // pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
    // pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
    // pLayerCfg1.FBStartAdress = 0;
    // pLayerCfg1.ImageWidth = 0;
    // pLayerCfg1.ImageHeight = 0;
    // pLayerCfg1.Backcolor.Blue = 0;
    // pLayerCfg1.Backcolor.Green = 0;
    // pLayerCfg1.Backcolor.Red = 0;
    // if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg1, 1) != HAL_OK)
    // {
    //     Error_Handler();
    // }
    /* USER CODE BEGIN LTDC_Init 2 */

    /* USER CODE END LTDC_Init 2 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
