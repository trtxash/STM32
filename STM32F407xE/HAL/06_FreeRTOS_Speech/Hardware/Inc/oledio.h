#ifndef __OLEDIO_H
#define __OLEDIO_H

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "sys.h"
#include "oledconf.h"

#if _SOFT_OR_HARE == OLED_HARD
#if _DRIVE_INTERFACE_TYPE == OLED_IIC_INTERFACE
#include "i2c.h"
#else
#include "spi.h"
#endif
#endif

#if _SOFT_OR_HARE == OLED_SOFT // 是否软件模拟
/* Exported constants ------------------------------------------------------------------------------------------------*/
#if _DRIVE_INTERFACE_TYPE == OLED_IIC_INTERFACE
/*************************************************** OLED IIC 端口定义 ************************************************/
#define OLED_SCLK_Port GPIOB
#define OLED_SCLK_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_SCLK_Pin GPIO_PIN_6

#define OLED_SDIN_Port GPIOB
#define OLED_SDIN_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_SDIN_Pin GPIO_PIN_7

#define OLED_RST_Port GPIOB
#define OLED_RST_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_RST_Pin GPIO_PIN_0

#define OLED_SCLK_Clr() OLED_SCLK_Port->BSRR = (uint32_t)OLED_SCLK_Pin << 16U
#define OLED_SCLK_Set() OLED_SCLK_Port->BSRR = OLED_SCLK_Pin

#define OLED_SDIN_Clr() OLED_SDIN_Port->BSRR = (uint32_t)OLED_SDIN_Pin << 16U
#define OLED_SDIN_Set() OLED_SDIN_Port->BSRR = OLED_SDIN_Pin
#define OLED_READ_SDIN() OLED_SDIN_Port->IDR &OLED_SDIN_Pin // 0 or 1

#define OLED_RST_Clr() HAL_GPIO_WritePin(OLED_RST_Port, OLED_RST_Pin, GPIO_PIN_RESET)
#define OLED_RST_Set() HAL_GPIO_WritePin(OLED_RST_Port, OLED_RST_Pin, GPIO_PIN_SET)
/*************************************************** OLED IIC 端口定义 ************************************************/
#else
/*************************************************** OLED SPI 端口定义 ************************************************/
#define OLED_CS_Port GPIOB
#define OLED_CS_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_CS_Pin GPIO_PIN_0

#define OLED_DC_Port GPIOF
#define OLED_DC_Port_Clk_Enable() __HAL_RCC_GPIOF_CLK_ENABLE()
#define OLED_DC_Pin GPIO_PIN_11

#define OLED_RST_Port GPIOB
#define OLED_RST_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_RST_Pin GPIO_PIN_1

#define OLED_DIN_Port GPIOA
#define OLED_DIN_Port_Clk_Enable() __HAL_RCC_GPIOA_CLK_ENABLE()
#define OLED_DIN_Pin GPIO_PIN_7

#define OLED_CLK_Port GPIOA
#define OLED_CLK_Port_Clk_Enable() __HAL_RCC_GPIOA_CLK_ENABLE()
#define OLED_CLK_Pin GPIO_PIN_5

#define OLED_CS_Clr() OLED_CS_Port->BSRR = (uint32_t)OLED_CS_Pin << 16U
#define OLED_CS_Set() OLED_CS_Port->BSRR = OLED_CS_Pin

#define OLED_DC_Clr() OLED_DC_Port->BSRR = (uint32_t)OLED_DC_Pin << 16U
#define OLED_DC_Set() OLED_DC_Port->BSRR = OLED_DC_Pin

#define OLED_RST_Clr() OLED_RST_Port->BSRR = (uint32_t)OLED_RST_Pin << 16U
#define OLED_RST_Set() OLED_RST_Port->BSRR = OLED_RST_Pin

#define OLED_DIN_Clr() OLED_DIN_Port->BSRR = (uint32_t)OLED_DIN_Pin << 16U
#define OLED_DIN_Set() OLED_DIN_Port->BSRR = OLED_DIN_Pin

#define OLED_CLK_Clr() OLED_CLK_Port->BSRR = (uint32_t)OLED_CLK_Pin << 16U
#define OLED_CLK_Set() OLED_CLK_Port->BSRR = OLED_CLK_Pin
/*************************************************** OLED SPI 端口定义 ************************************************/
#endif

/* Exported functions ------------------------------------------------------------------------------------------------*/

/* 硬件初始化函数 *****************************************************************************************************/
void OledDrv_Init(void);

/* IIC 通信接口函数 ***************************************************************************************************/
void OledDrv_IICDelay(void);
void OledDrv_IICWaitAck(void);
void OledDrv_IICStart(void);
void OledDrv_IICStop(void);
void OledDrv_IICSendByte(uint8_t data);

#else // 硬件

#if _DRIVE_INTERFACE_TYPE == OLED_IIC_INTERFACE // 是否IIC

#define OLED_RST_Port GPIOB
#define OLED_RST_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_RST_Pin GPIO_PIN_0

#define OLED_RST_Clr() HAL_GPIO_WritePin(OLED_RST_Port, OLED_RST_Pin, GPIO_PIN_RESET)
#define OLED_RST_Set() HAL_GPIO_WritePin(OLED_RST_Port, OLED_RST_Pin, GPIO_PIN_SET)

#else // SPI

#define OLED_CS_Port GPIOB
#define OLED_CS_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_CS_Pin GPIO_PIN_0

#define OLED_DC_Port GPIOF
#define OLED_DC_Port_Clk_Enable() __HAL_RCC_GPIOF_CLK_ENABLE()
#define OLED_DC_Pin GPIO_PIN_11

#define OLED_RST_Port GPIOB
#define OLED_RST_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_RST_Pin GPIO_PIN_1

#define OLED_CS_Clr() OLED_CS_Port->BSRR = (uint32_t)OLED_CS_Pin << 16U
#define OLED_CS_Set() OLED_CS_Port->BSRR = OLED_CS_Pin

#define OLED_DC_Clr() OLED_DC_Port->BSRR = (uint32_t)OLED_DC_Pin << 16U
#define OLED_DC_Set() OLED_DC_Port->BSRR = OLED_DC_Pin

#define OLED_RST_Clr() OLED_RST_Port->BSRR = (uint32_t)OLED_RST_Pin << 16U
#define OLED_RST_Set() OLED_RST_Port->BSRR = OLED_RST_Pin

#endif

/* 硬件初始化函数 *****************************************************************************************************/
void OledDrv_Init(void);

#endif

#endif // __OLEDIO_H
