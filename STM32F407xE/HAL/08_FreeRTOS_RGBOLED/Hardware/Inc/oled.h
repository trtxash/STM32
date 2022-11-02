#ifndef __OLED_H
#define __OLED_H

#include "sys.h"
#include "delay.h"
#include "oledio.h"
#include "oledconf.h"

/* 储存绘画信息的结构体 */
typedef struct __OLED_DrawData_HandleTypeDef
{
    u8 x;      // x位置
    u8 y;      // y位置
    u8 x1;     // x位置
    u8 y1;     // y位置
    u8 r;      // 画圆半径
    u8 size;   // 显示大小
    u8 mode;   // 显示模式
    u16 color; // 显示颜色
} OLED_DrawData_HandleTypeDef;

extern OLED_DrawData_HandleTypeDef DrawData;

// 颜色
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40 // 棕色
#define BRRED 0XFC07 // 棕红色
#define GRAY 0X8430  // 灰色
// GUI颜色

#define DARKBLUE 0X01CF  // 深蓝色
#define LIGHTBLUE 0X7D7C // 浅蓝色
#define GRAYBLUE 0X5458  // 灰蓝色
// 以上三色为PANEL的颜色

#define LIGHTGREEN 0X841F // 浅绿色
#define LGRAY 0XC618      // 浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE 0XA651 // 浅灰蓝色(中间层颜色)
#define LBBLUE 0X2B12    // 浅棕蓝色(选择条目的反色)

void OLED_WR_Byte(u8 dat, u8 mode);
void OLED_WR_DATA8(u8 dat);
void OLED_WR_DATA16(u16 dat);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(u16 x, u8 y, u8 t, u16 color);
void OLED_DrawLine(u16 x1, u8 y1, u16 x2, u8 y2, u8 mode, u16 color);
void OLED_DrawCircle(u16 x, u8 y, u8 r, u16 color);
void OLED_ShowChar(u16 x, u8 y, u8 chr, u8 size1, u8 mode, u16 color);
void OLED_ShowString(u16 x, u8 y, u8 *chr, u8 size1, u8 mode, u16 color);
void OLED_ShowNum(u16 x, u8 y, u32 num, u8 len, u8 size1, u8 mode, u16 color);
void OLED_ShowChinese(u16 x, u8 y, u8 num, u8 size1, u8 mode, u16 color);
void OLED_ScrollDisplay(u8 num, u8 space, u8 mode, u16 color);
void OLED_ShowPicture(u16 x, u8 y, u8 sizex, u8 sizey, u8 BMP[], u8 mode);
void OLED_Init(void);

#endif // __OLED_H
