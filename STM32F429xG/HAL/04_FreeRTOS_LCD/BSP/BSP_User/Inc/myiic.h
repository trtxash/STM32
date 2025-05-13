#ifndef _MYIIC_H
#define _MYIIC_H

#include "sys.h"

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

// #define OLED_SCLK_Clr() (OLED_SCLK_Port->BSRR = (uint32_t)OLED_SCLK_Pin << 16U)
// #define OLED_SCLK_Set() (OLED_SCLK_Port->BSRR = OLED_SCLK_Pin)

typedef struct
{
    GPIO_TypeDef *RST_GPIOx;
    uint16_t RST_Pin;

    GPIO_TypeDef *SCL_GPIOx;
    uint16_t SCL_Pin;

    GPIO_TypeDef *SDA_GPIOx;
    uint16_t SDA_Pin;

    uint8_t Drv_IICDelay_Time;
} birch_iic_bus_t;

extern birch_iic_bus_t birch_iic_bus[1];

void Drv_Init(birch_iic_bus_t *iic_bus);
void Drv_IICDelay(birch_iic_bus_t *iic_bus);
void Drv_IICStart(birch_iic_bus_t *iic_bus);
void Drv_IICStop(birch_iic_bus_t *iic_bus);
u8 Drv_IICWaitAck(birch_iic_bus_t *iic_bus);
void Drv_IICAck(birch_iic_bus_t *iic_bus);
void Drv_IICNAck(birch_iic_bus_t *iic_bus);
void Drv_IICSendByte(uint8_t data, birch_iic_bus_t *iic_bus);
u8 Drv_IICReadByte(u8 ack, birch_iic_bus_t *iic_bus);

#endif
