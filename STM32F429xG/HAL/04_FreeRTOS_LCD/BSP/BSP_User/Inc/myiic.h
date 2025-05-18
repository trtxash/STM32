#ifndef _MYIIC_H
#define _MYIIC_H

#include "sys.h"

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
