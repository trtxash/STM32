#ifndef __MLX90614_H
#define __MLX90614_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include "delay.h"

#define ACK 0
#define NACK 1
#define SA 0x00            //设备地址
#define RAM_ACCESS 0x00    // RAM access command RAM
#define EEPROM_ACCESS 0x20 // EEPROM access command EEPROM
#define RAM_TOBJ1 0x07     // To1 address in the eeprom  -70.01 ~ 382.19

#define SCK_GPIO_Port GPIOB
#define SCK_Pin GPIO_PIN_3
#define SCK_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define SCK_GPIO_CLK_DISABLE() __HAL_RCC_GPIOB_CLK_DISABLE()
#define SDA_GPIO_Port GPIOA
#define SDA_Pin GPIO_PIN_15
#define SDA_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define SDA_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()

#define SMBUS_SCK_H() HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_SET)
#define SMBUS_SCK_L() HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_RESET)
#define SMBUS_SDA_H() HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET)
#define SMBUS_SDA_L() HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET)
#define SMBUS_SDA_PIN() HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin)

void SMBus_StartBit(void);
void SMBus_StopBit(void);
void SMBus_SendBit(u8);
u8 SMBus_SendByte(u8);
u8 SMBus_ReceiveBit(void);
u8 SMBus_ReceiveByte(u8 ack_nack);
void SMBus_GpioInit(void);
void SMBus_Init(void);
u16 SMBus_ReadMemory(u8, u8);
u8 PEC_Calculation(u8 *);
float SMBus_ReadTemp(void);

#endif