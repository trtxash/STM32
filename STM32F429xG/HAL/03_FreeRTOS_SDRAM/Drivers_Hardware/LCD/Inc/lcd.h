#ifndef _LCD_H
#define _LCD_H

#include "sdram.h"
#include "sys.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* LCD参数 */
#define HORIZONTAL_SYNCHRONOUS_PULSE 10 // 水平同步宽度,单位为相素时钟（CLK）个数
#define HORIZONTAL_BACK_PORCH        150
#define ACTIVE_WIDTH                 800
#define HORIZONTAL_FRONT_PORCH       15

#define VERTICAL_SYNCHRONOUS_PULSE 10 // 垂直同步高度，单位为行周期个数
#define VERTICAL_BACK_PORCH        140
#define ACTIVE_HEIGHT              480
#define VERTICAL_FRONT_PORCH       40

/* LCD IO */
#define LCD_BLK_GPIO              GPIOD
#define LCD_BLK_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
#define LCD_BLK_PIN               GPIO_PIN_13

/* GPIO操作 */
#define LCD_BLK_Clr() (LCD_BLK_GPIO->BSRR = (uint32_t)LCD_BLK_PIN << 16U)
#define LCD_BLK_Set() (LCD_BLK_GPIO->BSRR = LCD_BLK_PIN)

/* LCD颜色格式 */
#define LCD_PIXEL_FORMAT_ARGB8888 0X00
#define LCD_PIXEL_FORMAT_RGB888   0X01
#define LCD_PIXEL_FORMAT_RGB565   0X02
#define LCD_PIXEL_FORMAT_ARGB1555 0X03
#define LCD_PIXEL_FORMAT_ARGB4444 0X04
#define LCD_PIXEL_FORMAT_L8       0X05
#define LCD_PIXEL_FORMAT_AL44     0X06
#define LCD_PIXEL_FORMAT_AL88     0X07

/* ------用户修改配置部分:-------- */

// 定义颜色像素格式,一般用RGB565
#define LCD_PIXFORMAT LCD_PIXEL_FORMAT_RGB565
// 定义默认背景层颜色
#define LTDC_BACKLAYERCOLOR 0X00000000
// LCD帧缓冲区首地址,这里定义在SDRAM里面.
#define LCD_FRAME_BUF_ADDR Bank5_SDRAM_ADDR

    // LCD LTDC重要参数集
    typedef struct
    {
        u32 pwidth;     // LCD面板的宽度,固定参数,不随显示方向改变,如果为0,说明没有任何RGB屏接入
        u32 pheight;    // LCD面板的高度,固定参数,不随显示方向改变
        u16 hsw;        // 水平同步宽度
        u16 vsw;        // 垂直同步宽度
        u16 hbp;        // 水平后廊
        u16 vbp;        // 垂直后廊
        u16 hfp;        // 水平前廊
        u16 vfp;        // 垂直前廊
        u8 activelayer; // 当前层编号:0/1
        u8 dir;         // 0,竖屏;1,横屏;
        u16 width;      // LCD宽度
        u16 height;     // LCD高度
        u32 pixsize;    // 每个像素所占字节数
    } _ltdc_dev;

    extern _ltdc_dev lcdltdc; // 管理LCD LTDC参数

    extern u16 *ltdc_lcd_framebuf; // LTDC LCD帧缓存数组指针,必须指向对应大小的内存区域
    extern u32 *ltdc_framebuf[2];

    void LCD_BLK_Init(void);
    void LCD_Init(void);

#if defined(__cplusplus)
}
#endif

#endif
