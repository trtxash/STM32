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

#define OLED_SCLK_Clr() HAL_GPIO_WritePin(OLED_SCLK_Port, OLED_SCLK_Pin, GPIO_PIN_RESET) // SCL IIC接口的时钟信号
#define OLED_SCLK_Set() HAL_GPIO_WritePin(OLED_SCLK_Port, OLED_SCLK_Pin, GPIO_PIN_SET)

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(OLED_SDIN_Port, OLED_SDIN_Pin, GPIO_PIN_RESET) // SDA IIC接口的数据信号
#define OLED_SDIN_Set() HAL_GPIO_WritePin(OLED_SDIN_Port, OLED_SDIN_Pin, GPIO_PIN_SET)

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

#define OLED_DIN_Port GPIOC
#define OLED_DIN_Port_Clk_Enable() __HAL_RCC_GPIOC_CLK_ENABLE()
#define OLED_DIN_Pin GPIO_PIN_2

#define OLED_CLK_Port GPIOB
#define OLED_CLK_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_CLK_Pin GPIO_PIN_2

#define OLED_CS_Clr() OLED_CS_Port->BSRR = (uint32_t)OLED_CS_Pin << 16U
#define OLED_CS_Set() OLED_CS_Port->BSRR = OLED_CS_Pin

#define OLED_DC_Clr() OLED_DC_Port->BSRR = (uint32_t)OLED_DC_Pin << 16U
#define OLED_DC_Set() OLED_DC_Port->BSRR = OLED_DC_Pin

#define OLED_RST_Clr() OLED_RST_Port->BSRR = (uint32_t)OLED_RST_Pin << 16U
#define OLED_RST_Set() OLED_RST_Port->BSRR = OLED_RST_Pin

#define OLED_DIN_Clr() HAL_GPIO_WritePin(OLED_DIN_Port, GPIO_PIN_2, GPIO_PIN_RESET)
#define OLED_DIN_Set() HAL_GPIO_WritePin(OLED_DIN_Port, GPIO_PIN_2, GPIO_PIN_SET)

#define OLED_CLK_Clr() HAL_GPIO_WritePin(OLED_CLK_Port, GPIO_PIN_2, GPIO_PIN_RESET)
#define OLED_CLK_Set() HAL_GPIO_WritePin(OLED_CLK_Port, GPIO_PIN_2, GPIO_PIN_SET)
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

/* SPI 通信接口函数 ***************************************************************************************************/
void OledDrv_SPIWriteByte(uint8_t data);

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
