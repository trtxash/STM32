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
// LCD帧缓冲区首地址,这里定义在SDRAM里面.
#define LCD_FRAME_BUF_ADDR Bank5_SDRAM_ADDR

#if LCD_PIXFORMAT == LCD_PIXEL_FORMAT_RGB565
//---------------------颜色表--------------------
#define GUI_LightPink            0xFDB8 // 浅粉红
#define GUI_Pink                 0xFE19 // 粉红
#define GUI_Crimson              0xD8A7 // 猩红
#define GUI_LavenderBlush        0xFF9E // 脸红的淡紫色
#define GUI_PaleVioletRed        0xDB92 // 苍白的紫罗兰红色
#define GUI_HotPink              0xFB56 // 热情的粉红
#define GUI_DeepPink             0xF8B2 // 深粉色
#define GUI_MediumVioletRed      0xC0B0 // 适中的紫罗兰红色
#define GUI_Orchid               0xDB9A // 兰花的紫色
#define GUI_Thistle              0xDDFB // 蓟
#define GUI_plum                 0xDD1B // 李子
#define GUI_Violet               0xEC1D // 紫罗兰
#define GUI_Magenta              0xF81F // 洋红
#define GUI_Fuchsia              0xF81F // 灯笼海棠(紫红色)
#define GUI_DarkMagenta          0x8811 // 深洋红色
#define GUI_Purple               0x8010 // 紫色
#define GUI_MediumOrchid         0xBABA // 适中的兰花紫
#define GUI_DarkVoilet           0x901A // 深紫罗兰色
#define GUI_DarkOrchid           0x9999 // 深兰花紫
#define GUI_Indigo               0x4810 // 靛青
#define GUI_BlueViolet           0x895C // 深紫罗兰的蓝色
#define GUI_MediumPurple         0x939B // 适中的紫色
#define GUI_MediumSlateBlue      0x7B5D // 适中的板岩暗蓝灰色
#define GUI_SlateBlue            0x6AD9 // 板岩暗蓝灰色
#define GUI_DarkSlateBlue        0x49F1 // 深岩暗蓝灰色
#define GUI_Lavender             0xE73F // 熏衣草花的淡紫色
#define GUI_GhostWhite           0xFFDF // 幽灵的白色
#define GUI_Blue                 0x001F // 纯蓝
#define GUI_MediumBlue           0x0019 // 适中的蓝色
#define GUI_MidnightBlue         0x18CE // 午夜的蓝色
#define GUI_DarkBlue             0x0011 // 深蓝色
#define GUI_Navy                 0x0010 // 海军蓝
#define GUI_RoyalBlue            0x435C // 皇家蓝
#define GUI_CornflowerBlue       0x64BD // 矢车菊的蓝色
#define GUI_LightSteelBlue       0xB63B // 淡钢蓝
#define GUI_LightSlateGray       0x7453 // 浅石板灰
#define GUI_SlateGray            0x7412 // 石板灰
#define GUI_DoderBlue            0x1C9F // 道奇蓝
#define GUI_AliceBlue            0xF7DF // 爱丽丝蓝
#define GUI_SteelBlue            0x4416 // 钢蓝
#define GUI_LightSkyBlue         0x867F // 淡蓝色
#define GUI_SkyBlue              0x867D // 天蓝色
#define GUI_DeepSkyBlue          0x05FF // 深天蓝
#define GUI_LightBLue            0xAEDC // 淡蓝
#define GUI_PowDerBlue           0xB71C // 火药蓝
#define GUI_CadetBlue            0x5CF4 // 军校蓝
#define GUI_Azure                0xF7FF // 蔚蓝色
#define GUI_LightCyan            0xE7FF // 淡青色
#define GUI_PaleTurquoise        0xAF7D // 苍白的绿宝石
#define GUI_Cyan                 0x07FF // 青色
#define GUI_Aqua                 0xD79C // 水绿色
#define GUI_DarkTurquoise        0x067A // 深绿宝石
#define GUI_DarkSlateGray        0x2A69 // 深石板灰
#define GUI_DarkCyan             0x0451 // 深青色
#define GUI_Teal                 0x0410 // 水鸭色
#define GUI_MediumTurquoise      0x4E99 // 适中的绿宝石
#define GUI_LightSeaGreen        0x2595 // 浅海洋绿
#define GUI_Turquoise            0x471A // 绿宝石
#define GUI_Auqamarin            0x7FF5 // 绿玉\碧绿色
#define GUI_MediumAquamarine     0x07D3 // 适中的碧绿色
#define GUI_MediumSpringGreen    0x07EF // 适中的春天的绿色
#define GUI_MintCream            0xF7FF // 薄荷奶油
#define GUI_SpringGreen          0x3D8E // 春天的绿色
#define GUI_SeaGreen             0x2C4A // 海洋绿
#define GUI_Honeydew             0xF7FE // 蜂蜜
#define GUI_LightGreen           0x9772 // 淡绿色
#define GUI_PaleGreen            0x9FD3 // 苍白的绿色
#define GUI_DarkSeaGreen         0x8DF1 // 深海洋绿
#define GUI_LimeGreen            0x3666 // 酸橙绿
#define GUI_Lime                 0x07E0 // 酸橙色
#define GUI_ForestGreen          0x2444 // 森林绿
#define GUI_Green                0x0400 // 纯绿
#define GUI_DarkGreen            0x0320 // 深绿色
#define GUI_Chartreuse           0x7FE0 // 查特酒绿
#define GUI_LawnGreen            0x7FE0 // 草坪绿
#define GUI_GreenYellow          0xAFE5 // 绿黄色
#define GUI_OliveDrab            0x5345 // 橄榄土褐色
#define GUI_Beige                0xF7BB // 米色(浅褐色)
#define GUI_LightGoldenrodYellow 0xFFDA // 浅秋麒麟黄
#define GUI_Ivory                0xFFFE // 象牙
#define GUI_LightYellow          0xFFFC // 浅黄色
#define GUI_Yellow               0xFFE0 // 纯黄
#define GUI_Olive                0x8400 // 橄榄
#define GUI_DarkKhaki            0xBDAD // 深卡其布
#define GUI_LemonChiffon         0xFFD9 // 柠檬薄纱
#define GUI_PaleGodenrod         0xEF55 // 灰秋麒麟
#define GUI_Khaki                0xF731 // 卡其布
#define GUI_Gold                 0xFEA0 // 金
#define GUI_Cornislk             0xFFDB // 玉米色
#define GUI_GoldEnrod            0xDD24 // 秋麒麟
#define GUI_FloralWhite          0xFFDE // 花的白色
#define GUI_OldLace              0xFFBC // 老饰带
#define GUI_Wheat                0xF6F6 // 小麦色
#define GUI_Moccasin             0xFF36 // 鹿皮鞋
#define GUI_Orange               0xFD20 // 橙色
#define GUI_PapayaWhip           0xFF7A // 番木瓜
#define GUI_BlanchedAlmond       0xFF59 // 漂白的杏仁
#define GUI_NavajoWhite          0xFEF5 // 纳瓦霍白
#define GUI_AntiqueWhite         0xFF5A // 古代的白色
#define GUI_Tan                  0xD5B1 // 晒黑
#define GUI_BrulyWood            0xDDD0 // 结实的树
#define GUI_Bisque               0xFF38 //(浓汤)乳脂, 番茄等
#define GUI_DarkOrange           0xFC60 // 深橙色
#define GUI_Linen                0xFF9C // 亚麻布
#define GUI_Peru                 0xCC27 // 秘鲁
#define GUI_PeachPuff            0xFED7 // 桃色
#define GUI_SandyBrown           0xF52C // 沙棕色
#define GUI_Chocolate            0xD343 // 巧克力
#define GUI_SaddleBrown          0x8A22 // 马鞍棕色
#define GUI_SeaShell             0xFFBD // 海贝壳
#define GUI_Sienna               0xA285 // 黄土赭色
#define GUI_LightSalmon          0xFD0F // 浅鲜肉(鲑鱼) 色
#define GUI_Coral                0xFBEA // 珊瑚
#define GUI_OrangeRed            0xFA20 // 橙红色
#define GUI_DarkSalmon           0xECAF // 深鲜肉(鲑鱼) 色
#define GUI_Tomato               0xFB08 // 番茄
#define GUI_MistyRose            0xFF3C // 薄雾玫瑰
#define GUI_Salmon               0xFC0E // 鲜肉(鲑鱼) 色
#define GUI_Snow                 0xFFDF // 雪
#define GUI_LightCoral           0xF410 // 淡珊瑚色
#define GUI_RosyBrown            0xBC71 // 玫瑰棕色
#define GUI_IndianRed            0xCAEB // 印度红
#define GUI_Red                  0xF800 // 纯红
#define GUI_Brown                0xA145 // 棕色
#define GUI_FireBrick            0xB104 // 耐火砖
#define GUI_DarkRed              0x8800 // 深红色
#define GUI_Maroon               0x8000 // 栗色
#define GUI_White                0xFFFF // 纯白
#define GUI_WhiteSmoke           0xF7BE // 白烟
#define GUI_Gainsboro            0xDEFB // 亮灰色
#define GUI_LightGrey            0xD69A // 浅灰色
#define GUI_Silver               0xC618 // 银白色
#define GUI_DarkGray             0xAD55 // 深灰色
#define GUI_Gray                 0x8410 // 灰色
#define GUI_DimGray              0x6B4D // 暗淡的灰色
#define GUI_Black                0x0000 // 纯黑

// LCD的画笔颜色和背景色
#define POINT_COLOR GUI_Black // 画笔颜色
#define BACK_COLOR  GUI_White // 背景色

#endif

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

    void LTDC_Layer_Parameter_Config(u8 layerx, u32 bufaddr, u8 pixformat, u8 alpha, u8 alpha0, u8 bfac1, u8 bfac2, u32 bkcolor);
    void LTDC_Layer_Window_Config(u8 layerx, u16 sx, u16 sy, u16 width, u16 height);
    void LTDC_Switch(u8 sw);
    void LTDC_Layer_Switch(u8 layerx, u8 sw);
    void LTDC_Select_Layer(u8 layerx);
    void LTDC_Display_Dir(u8 dir);
    void LTDC_Draw_Point(u16 x, u16 y, u32 color);
    u32 LTDC_Read_Point(u16 x, u16 y);
    void LTDC_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u32 color);
    void LTDC_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 *color);
    void LTDC_Clear(u32 color);
    void LTDC_Draw_Line(u16 x1, u16 y1, u16 x2, u16 y2, u32 color);
    void LTDC_Draw_Rectangle(u16 x1, u16 y1, u16 x2, u16 y2, u32 color);
    void LTDC_Draw_Circle(u16 x0, u16 y0, u8 r, u32 color);
    void LTDC_Show_Char(u16 x, u16 y, u8 num, u8 size, u8 mode, u32 color);
    u32 LCD_Pow(u8 m, u8 n);
    void LTDC_Show_Num(u16 x, u16 y, u32 num, u8 len, u8 size, u32 color);
    void LTDC_Show_xNum(u16 x, u16 y, u32 num, u8 len, u8 size, u8 mode, u32 color);
    void LTDC_Show_String(u16 x, u16 y, u16 width, u16 height, u8 size, u8 *p, u32 color);

#if defined(__cplusplus)
}
#endif

#endif
