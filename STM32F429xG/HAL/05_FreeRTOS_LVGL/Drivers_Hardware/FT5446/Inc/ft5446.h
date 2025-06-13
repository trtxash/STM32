#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "i2c.h"
#include "sys.h"

#define RST_PORT              GPIOD
#define RST_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE();
#define RST_PIN               GPIO_PIN_12
#define RST_ON                (RST_PORT->BSRR = RST_PIN)
#define RST_OFF               (RST_PORT->BSRR = (uint32_t)RST_PIN << 16U)

#define INT_PORT              GPIOD
#define INT_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE();
#define INT_PIN               GPIO_PIN_11

// I2C读写命令
#define FT_ADDRESS 0x38                       // I2C 7位地址
#define FT_CMD_WR  (FT_ADDRESS << 1U)         // 写命令
#define FT_CMD_RD  (FT_ADDRESS << 1U | 0x01U) // 读命令

// FT5xxx 寄存器缓冲区长度定义
#define Touch_Other_Reg_Data_Len 3
#define Touch_Data_Len           (Touch_Other_Reg_Data_Len + CT_MAX_TOUCH * 6)

// FT5xxx 部分寄存器定义
#define FT_DEVIDE_MODE    0x00 // FT5xxx模式控制寄存器
#define FT_REG_NUM_FINGER 0x02 // 触摸状态寄存器
#define FT_TP1_REG        0X03 // 第一个触摸点数据地址
#define FT_TP2_REG        0X09 // 第二个触摸点数据地址
#define FT_TP3_REG        0X0F // 第三个触摸点数据地址
#define FT_TP4_REG        0X15 // 第四个触摸点数据地址
#define FT_TP5_REG        0X1B // 第五个触摸点数据地址

#define FT_ID_G_LIB_VERSION  0xA1 // 版本
#define FT_ID_G_MODE         0xA4 // FT5xxx中断模式控制寄存器
#define FT_ID_G_THGROUP      0x80 // 触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE 0x88 // 激活状态周期设置寄存器

#define CT_MAX_TOUCH 10     // 电容屏支持的点数

// 触摸屏控制器
typedef struct
{
    // uint8_t (*init)(void);    // 初始化触摸屏控制器
    // uint8_t (*scan)(uint8_t); // 扫描触摸屏.0,屏幕扫描;1,物理坐标;
    // void (*adjust)(void);     // 触摸屏校准
    uint16_t x[CT_MAX_TOUCH]; // 当前坐标
    uint16_t y[CT_MAX_TOUCH]; // 电容屏有最多5组坐标,电阻屏则用x[0],y[0]代表:此次扫描时,触屏的坐标,用

    // sta笔的状态
    // b15~b0:电容触摸屏按下的点数(0,表示未按下,1表示按下)
    uint16_t sta;

    // float xfac;
    // float yfac;
    // short xoff;
    // short yoff;
    uint8_t touchtype;
} _m_tp_dev;
extern _m_tp_dev tp_dev; // 触屏控制器在touch.c里面定义

// 触摸屏操作函数
// uint8_t FT5xxx_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len);
// void FT5xxx_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len);
// uint8_t FT5xxx_Init_Soft(void);
uint8_t FT5xxx_Init_Hard(void);
uint8_t FT5xxx_Scan(uint8_t *buff);

#endif
