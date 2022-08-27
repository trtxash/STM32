#ifndef __OLED_H
#define __OLED_H

#include "sys.h"
#include "delay.h"

/******************************************
定义
OledCS  ：CS
OledRS  : DC
OledRst ：RES可以直接接单片机RST引脚
OledSck : D0
OledSdin: D1
******************************************/

// OLEDIIC模式选择
// 0: SPI模式
// 1: IIC模式
#define OLEDIIC 0

#if OLEDIIC == 1
#define I2CSCLPort GPIOC
#define I2CSDAPort GPIOC
#define I2CSCLPortClkEnable() __HAL_RCC_GPIOC_CLK_ENABLE()
#define I2CSDAPortClkEnable() __HAL_RCC_GPIOC_CLK_ENABLE()
#define I2CSCLPin GPIO_PIN_14
#define I2CSDAPin GPIO_PIN_15

#define OledSCL_H() HAL_GPIO_WritePin(I2CSCLPort, I2CSCLPin, GPIO_PIN_SET)
#define OledSCL_L() HAL_GPIO_WritePin(I2CSCLPort, I2CSCLPin, GPIO_PIN_RESET)

#define OledSDA_H() HAL_GPIO_WritePin(I2CSDAPort, I2CSDAPin, GPIO_PIN_SET)
#define OledSDA_L() HAL_GPIO_WritePin(I2CSDAPort, I2CSDAPin, GPIO_PIN_RESET)

#define OledSDA_Read() HAL_GPIO_ReadPin(I2CSDAPort, I2CSDAPin)

#else

// OLED模式设置
// 0: SPI串行模式  （模块的BS1，BS2均接GND）
// 1: 并行8080模式 （模块的BS1，BS2均接VCC）
#define OLED_MODE 0

// SPI模式设置
// 0: 4线串行模式 8 bit data send
// 1: 3线串行模式 9 bit data send
#define OLED_SPI_X 0

#define OledCsPort GPIOB
#define OledCsPin GPIO_PIN_0

#define OledRstPort GPIOB
#define OledRstPin GPIO_PIN_1

#define OledRSPort GPIOF
#define OledRSPin GPIO_PIN_11

#define OledCs_H() HAL_GPIO_WritePin(OledCsPort, OledCsPin, GPIO_PIN_SET)
#define OledCs_L() HAL_GPIO_WritePin(OledCsPort, OledCsPin, GPIO_PIN_RESET)

#define OledRst_H() HAL_GPIO_WritePin(OledRstPort, OledRstPin, GPIO_PIN_SET)
#define OledRst_L() HAL_GPIO_WritePin(OledRstPort, OledRstPin, GPIO_PIN_RESET)

#if OLED_MODE == 1
#define OledWRPort GPIOB
#define OledWRPin GPIO_PIN_15

#define OledRDPort GPIOB
#define OledRDPin GPIO_PIN_11

#define OledWR_H() HAL_GPIO_WritePin(OledWRPort, OledWRPin, GPIO_PIN_SET)
#define OledWR_L() HAL_GPIO_WritePin(OledWRPort, OledWRPin, GPIO_PIN_RESET)

#define OledRD_H() HAL_GPIO_WritePin(OledRDPort, OledRDPin, GPIO_PIN_SET)
#define OledRD_L() HAL_GPIO_WritePin(OledRDPort, OledRDPin, GPIO_PIN_RESET)

#define DATAOUT(x) HAL_GPIO_WritePin(GPIOA, x, GPIO_PIN_SET); //输出  //PA0~7,作为数据线
// #define DATAOUT(x) GPIO_Write(GPIOA, x); //输出  //PA0~7,作为数据线

#else
#define OledSckPort GPIOB
#define OledSckPin GPIO_PIN_2

#define OledSdinPort GPIOC
#define OledSdinPin GPIO_PIN_2

#define OledSck_H() HAL_GPIO_WritePin(OledSckPort, OledSckPin, GPIO_PIN_SET)
#define OledSck_L() HAL_GPIO_WritePin(OledSckPort, OledSckPin, GPIO_PIN_RESET)

#define OledSdin_H() HAL_GPIO_WritePin(OledSdinPort, OledSdinPin, GPIO_PIN_SET)
#define OledSdin_L() HAL_GPIO_WritePin(OledSdinPort, OledSdinPin, GPIO_PIN_RESET)

#endif

#define OledRS_H() HAL_GPIO_WritePin(OledRSPort, OledRSPin, GPIO_PIN_SET)
#define OledRS_L() HAL_GPIO_WritePin(OledRSPort, OledRSPin, GPIO_PIN_RESET)

#endif

#define OLED_ADDRESS 0x78 // IIC模式下的地址,需要结合硬件
#define OLED_CMD 0x00     //写命令
#define OLED_DATA 0x01    //写数据

void OLED_ON(void);
void OLED_CLS(void);
void OLED_OFF(void);
void OLED_Init(void);
void WriteDat(u8 I2C_Data);
void OLED_SetPos(u8 x, u8 y);
void OLED_Fill(u8 fill_Data);
void I2C_Configuration(void);
void WriteCmd(u8 I2C_Command);
void OLED_ShowCN(u8 x, u8 y, u8 N);
void OLED_ShowCHinese(u8 x, u8 y, u8 no);
void I2C_WriteByte(uint8_t addr, uint8_t data);
void OLED_ShowStr(u8 x, u8 y, u8 ch[], u8 TextSize);
void OLED_DrawBMP(u8 x0, u8 y0, u8 x1, u8 y1, u8 BMP[]);
void OLED_ShowString(u8 x, u8 y, char *chr, u8 Char_Size);

#endif
