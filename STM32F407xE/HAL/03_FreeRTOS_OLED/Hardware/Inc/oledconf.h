#ifndef __OLEDCONF_H
#define __OLEDCONF_H

/* OLED 驱动接口定义 */
#define OLED_IIC_INTERFACE 1 // iic 接口
#define OLED_SPI_INTERFACE 1 // spi 接口

/**
 * @brief  OLED 的宽度和高度
 */
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

/**
 * @brief  OLED IIC延时时间
 */
#define OledDrv_IICDelay_Time 0

#endif // __OLEDCONF_H
