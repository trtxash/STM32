#ifndef __OLEDCONF_H
#define __OLEDCONF_H

/* OLED 设备IC选择 */
#define OLED_SSD1306_SSD1315 0
#define OLED_SSD1351 1

/* OLED 驱动接口定义 */
#define OLED_IIC_INTERFACE 0 // iic 接口
#define OLED_SPI_INTERFACE 1 // spi 接口

#define OLED_SOFT 0 // 软件模拟通信
#define OLED_HARD 1 // 硬件通信

/* OLED 具体选择 */
#define _OLED_DRIVER_IC_TYPE OLED_SSD1306_SSD1315 // 改SSD种类
#define _DRIVE_INTERFACE_TYPE OLED_SPI_INTERFACE  // 改接口类型
#define _SOFT_OR_HARE OLED_HARD                   // 改是否模拟

/* OLED 的通信地址 */
#define OLED_ADDRESS 0x78

/* OLED 的宽度和高度 */
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

/* OLED IIC延时时间 */
#define OledDrv_IICDelay_Time 0

#endif // __OLEDCONF_H
