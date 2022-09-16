#ifndef __OLEDCONF_H
#define __OLEDCONF_H

/* OLED 驱动接口定义 */
#define OLED_IIC_INTERFACE 0 // iic 接口
#define OLED_SPI_INTERFACE 1 // spi 接口

#define _DRIVE_INTERFACE_TYPE OLED_IIC_INTERFACE // 改这个

/* OLED 的宽度和高度 */
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

/* OLED IIC延时时间 */
#define OledDrv_IICDelay_Time 0

#endif // __OLEDCONF_H
