/**
 **********************************************************************************************************************
 * @file    oledio.h
 * @brief   该文件提供 OLED 驱动硬件接口所有函数原型
 * @author  周鹏程    any question please send mail to const_zpc@163.com
 * @version V1.0.0
 * @date    2021-3-12
 **********************************************************************************************************************
 *
 **********************************************************************************************************************
 */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef __OLEDIO_H
#define __OLEDIO_H

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include "sys.h"

/* Exported macro ----------------------------------------------------------------------------------------------------*/

/* OLED 驱动接口定义 */
#define OLED_IIC_INTERFACE 0 // iic 接口
#define OLED_SPI_INTERFACE 1 // spi 接口
#define _DRIVE_INTERFACE_TYPE OLED_SPI_INTERFACE

/* Exported constants ------------------------------------------------------------------------------------------------*/
#if _DRIVE_INTERFACE_TYPE == OLED_IIC_INTERFACE
/*************************************************** OLED IIC 端口定义 ************************************************/
#define OLED_SCLK_Clr() HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) // SCL IIC接口的时钟信号
#define OLED_SCLK_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET)

#define OLED_SDIN_Clr() HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) // SDA IIC接口的数据信号
#define OLED_SDIN_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)
/*************************************************** OLED IIC 端口定义 ************************************************/
#else
/*************************************************** OLED SPI 端口定义 ************************************************/
#define OLED_DC_Clr() HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_11)
#define OLED_DC_Set() HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, GPIO_PIN_SET)

#define OLED_RST_Clr() HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)
#define OLED_RST_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)

#define OLED_DIN_Clr() HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)
#define OLED_DIN_Set() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET)

#define OLED_CLK_Clr() HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)
#define OLED_CLK_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET)
/*************************************************** OLED SPI 端口定义 ************************************************/
#endif

/* Exported functions ------------------------------------------------------------------------------------------------*/

/* 硬件初始化函数 *****************************************************************************************************/
extern void OledDrv_Init(void);

/* IIC 通信接口函数 ***************************************************************************************************/
extern void OledDrv_IICStart(void);
extern void OledDrv_IICStop(void);
extern void OledDrv_IICWriteByte(uint8_t data);

/* SPI 通信接口函数 ***************************************************************************************************/
extern void OledDrv_SPIWriteByte(uint8_t data);

#endif // __OLEDIO_H
