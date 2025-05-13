#ifndef __OLEDIO_H
#define __OLEDIO_H

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "oledconf.h"
#include "sys.h"

#if _SOFT_OR_HARE == OLED_HARD
#if _DRIVE_INTERFACE_TYPE == OLED_IIC_INTERFACE
#include "i2c.h"
#else
#include "spi.h"
#endif
#endif

#if _SOFT_OR_HARE == OLED_SOFT // 是否软件模拟

#if _DRIVE_INTERFACE_TYPE == OLED_IIC_INTERFACE
/*************************************************** OLED IIC 端口定义 ************************************************/
#include "myiic.h"

#define OLED_BIRCH_IIC_BUS_POINTER &birch_iic_bus[0]

#define OLED_SCLK_Port GPIOB
// #define OLED_SCLK_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_SCLK_Pin GPIO_PIN_4

#define OLED_SDIN_Port GPIOB
// #define OLED_SDIN_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_SDIN_Pin GPIO_PIN_5

#define OLED_RST_Port GPIOB
// #define OLED_RST_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_RST_Pin GPIO_PIN_0

// // IO方向设置
// #define OLED_SDA_IN()                             \
//     {                                             \
//         OLED_SDIN_Port->MODER &= ~(3 << (7 * 2)); \
//         OLED_SDIN_Port->MODER |= 0 << 7 * 2;      \
//     } // PB9输入模式，括号内数字为几号管脚
// #define OLED_SDA_OUT()                            \
//     {                                             \
//         OLED_SDIN_Port->MODER &= ~(3 << (7 * 2)); \
//         OLED_SDIN_Port->MODER |= 1 << 7 * 2;      \
//     } // PB9输出模式，括号内数字为几号管脚

// #define OLED_SCLK_Clr() OLED_SCLK_Port->BSRR = (uint32_t)OLED_SCLK_Pin << 16U
// #define OLED_SCLK_Set() OLED_SCLK_Port->BSRR = OLED_SCLK_Pin

// #define OLED_SDIN_Clr()  OLED_SDIN_Port->BSRR = (uint32_t)OLED_SDIN_Pin << 16U
// #define OLED_SDIN_Set()  OLED_SDIN_Port->BSRR = OLED_SDIN_Pin
// #define OLED_READ_SDIN() OLED_SDIN_Port->IDR &OLED_SDIN_Pin // 0 or 1

// #define OLED_RST_Clr() OLED_RST_Port->BSRR = (uint32_t)OLED_RST_Pin << 16U
// #define OLED_RST_Set() OLED_RST_Port->BSRR = OLED_RST_Pin
/*************************************************** OLED IIC 端口定义 ************************************************/
#else
/*************************************************** OLED SPI 端口定义 ************************************************/
#define OLED_DC_Port              GPIOG
#define OLED_DC_Port_Clk_Enable() __HAL_RCC_GPIOA_CLK_ENABLE()
#define OLED_DC_Pin               GPIO_PIN_10

#define OLED_RST_Port              GPIOG
#define OLED_RST_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_RST_Pin               GPIO_PIN_9

#define OLED_DIN_Port              GPIOG
#define OLED_DIN_Port_Clk_Enable() __HAL_RCC_GPIOG_CLK_ENABLE()
#define OLED_DIN_Pin               GPIO_PIN_14

#define OLED_CLK_Port              GPIOG
#define OLED_CLK_Port_Clk_Enable() __HAL_RCC_GPIOG_CLK_ENABLE()
#define OLED_CLK_Pin               GPIO_PIN_13

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
void OledDrv_IICStart(void);
void OledDrv_IICStop(void);
u8 OledDrv_IICWaitAck(void);
void OledDrv_IICAck(void);
void OledDrv_IICNAck(void);
void OledDrv_IICSendByte(uint8_t data);
u8 OledDrv_IICReadByte(u8 ack);

#else // 硬件

#if _DRIVE_INTERFACE_TYPE == OLED_IIC_INTERFACE // 是否IIC

#define OLED_RST_Port              GPIOB
#define OLED_RST_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_RST_Pin               GPIO_PIN_0

#define OLED_RST_Clr() OLED_RST_Port->BSRR = (uint32_t)OLED_RST_Pin << 16U
#define OLED_RST_Set() OLED_RST_Port->BSRR = OLED_RST_Pin

#else // SPI

#define OLED_CS_Port              GPIOG
#define OLED_CS_Port_Clk_Enable() __HAL_RCC_GPIOG_CLK_ENABLE()
#define OLED_CS_Pin               GPIO_PIN_12

#define OLED_DC_Port              GPIOG
#define OLED_DC_Port_Clk_Enable() __HAL_RCC_GPIOG_CLK_ENABLE()
#define OLED_DC_Pin               GPIO_PIN_10

#define OLED_RST_Port              GPIOG
#define OLED_RST_Port_Clk_Enable() __HAL_RCC_GPIOG_CLK_ENABLE()
#define OLED_RST_Pin               GPIO_PIN_9

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
