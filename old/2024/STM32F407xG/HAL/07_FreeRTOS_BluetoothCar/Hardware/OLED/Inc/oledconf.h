/**
 * @file	oledconf.h
 * @brief 	OLED相关驱动配置文件
 *          根据实际设备配置，硬件IIC，SPI相关配置在CORE中
 * @author 	TRTX-gamer      https://github.com/TRTX-gamer；
 *          突然吐血    https://space.bilibili.com/12890038;
 * @version 1.1
 * @date 	2022年10月28号14点07分
 */

#ifndef __OLEDCONF_H
#define __OLEDCONF_H

/* OLED 设备IC定义 */
#define OLED_SSD1306_SSD1315 0 // 设备IC为1306或1315，我的设备是单色
#define OLED_SSD1351 1         // 设备IC为1351，我的设备是RGB
#define OLED_SH1106 2          // 设备IC为1306或1315，我的设备是单色

/* OLED 驱动接口定义 */
#define OLED_SOFT 0 // 软件模拟通信
#define OLED_HARD 1 // 硬件通信

#define OLED_IIC_INTERFACE 0 // iic 接口
#define OLED_SPI_INTERFACE 1 // spi 接口

/* OLED DMA定义 */
#define OLED_DMA_DISABLE 0
#define OLED_DMA_ABLE 1

/* OLED RGB颜色深度定义 */
#define K65 0  // RGB565
#define K262 1 // RGB666

/* I2C,SPI 句柄选择------------------------------------------------------------------------- */
#define OLED_I2C_HandleTypeDef hi2c1 // I2C句柄选择
#define OLED_SPI_HandleTypeDef hspi1 // SPI句柄选择

/* OLED 具体选择------------------------------------------------------------------------- */
#define _OLED_DRIVER_IC_TYPE OLED_SH1106         // 改SSD种类
#define _SOFT_OR_HARE OLED_SOFT                  // 改是否软件模拟
#define _DRIVE_INTERFACE_TYPE OLED_IIC_INTERFACE // 改接口类型
#define _OLED_DMA OLED_DMA_DISABLE               // 改是否开启DMA
#define _OLED_RGB_COLOR_DEPTH K65                // RGB颜色深度选择

/* OLED 的通信地址 */
#define OLED_ADDRESS 0x78
#define OLED_RAM_ADDRESS 0x00 // 暂时未用到

/* OLED 的宽度和高度 */
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

/* OLED 软件IIC延时时间 */
#define OledDrv_IICDelay_Time 0

#endif // __OLEDCONF_H
