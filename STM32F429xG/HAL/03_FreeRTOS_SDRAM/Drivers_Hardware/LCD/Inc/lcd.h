#ifndef _LCD_H
#define _LCD_H

#include "sys.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

// #define LCD_WIDTH             480
// #define LCD_HEIGHT            800
// #define LCD_FB_BYTE_PER_PIXEL 1

#define HORIZONTAL_SYNCHRONOUS_PULSE 10 // 水平同步宽度,单位为相素时钟（CLK）个数
#define HORIZONTAL_BACK_PORCH        150
#define ACTIVE_WIDTH                 800
#define HORIZONTAL_FRONT_PORCH       15

#define VERTICAL_SYNCHRONOUS_PULSE 10 // 垂直同步高度，单位为行周期个数
#define VERTICAL_BACK_PORCH        140
#define ACTIVE_HEIGHT              480
#define VERTICAL_FRONT_PORCH       40

#define LCD_FRAME_BUF_ADDR 0XC0000000

/* GPIO */
#define LCD_DISP_GPIO GPIOD

/* GPIO CLK */
#define LCD_DISP_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()

/* GPIO PIN */
#define LCD_DISP_PIN GPIO_PIN_13

/* GPIO操作 */
#define LCD_DISP_Clr() (LCD_DISP_GPIO->BSRR = (uint32_t)LCD_DISP_PIN << 16U)
#define LCD_DISP_Set() (LCD_DISP_GPIO->BSRR = LCD_DISP_PIN)

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

    void lcd_disp_config(void);

#if defined(__cplusplus)
}
#endif

#endif
