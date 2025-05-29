#include "lcd.h"
#include "delay.h"
#include "dma2d.h"
#include "lcdfont.h"
#include "ltdc.h"
#include "sys.h"

#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h" //FreeRTOS使用
#include "task.h"
#endif

// #include "lv_port_disp.h"

// 根据不同的颜色格式,定义帧缓存数组
#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 || LCD_PIXFORMAT == LCD_PIXFORMAT_RGB888
u32 *ltdc_lcd_framebuf = (u16 *)LCD_FRAME_BUF_ADDR; // LTDC LCD帧缓存数组指针,必须指向对应大小的内存区域
#else
u16 *ltdc_lcd_framebuf = (u16 *)LCD_FRAME_BUF_ADDR; // LTDC LCD帧缓存数组指针,必须指向对应大小的内存区域u16 ltdc_lcd_framebuf[1280][800] __attribute__((at(LCD_FRAME_BUF_ADDR))); // 定义最大屏分辨率时,LCD所需的帧缓存数组大小
#endif

u32 *ltdc_framebuf[2];
_ltdc_dev lcdltdc; // 管理LCD LTDC的重要参数

/**
 * @brief	LCD的blk背光IO初始化
 * @param 	无
 * @arg
 * @note  	额外的LCD初始化
 * @retval	无
 */
void LCD_BLK_Init(void)
{
    GPIO_InitTypeDef GPIO_InitTure = {0};

    LCD_BLK_GPIO_CLK_ENABLE(); // 开启GPIO时钟

    /*进行结构体内的参数配置，先找到下面HAL_GPIO_Init();的定义处，再对定义处的函数详细找参数*/
    GPIO_InitTure.Mode = GPIO_MODE_OUTPUT_PP; // 推挽输出
    GPIO_InitTure.Pull = GPIO_NOPULL;
    GPIO_InitTure.Speed = GPIO_SPEED_FREQ_LOW; // 高速
    GPIO_InitTure.Pin = LCD_BLK_PIN;
    HAL_GPIO_Init(LCD_BLK_GPIO, &GPIO_InitTure); // 先在上面四行设置GPIO的模式，上下拉，速度，再对GPIOB管脚初始化

    LCD_BLK_Clr(); // 关闭背光
}

/**
 * @brief	RGB LCD初始化
 * @param 	无
 * @arg
 * @note  	加载了LTDC，DMA2D
 * @retval	无
 */
void LCD_Init(void)
{
// 延时等待屏幕硬件上电完成,不然可能出现显示不全的情况
#if SYSTEM_SUPPORT_OS
    delay_xms(10);
#else
    delay_ms(10);
#endif
    LCD_BLK_Init(); // 背光IO初始化
    LTDC_Init();    // LTDC初始化,LTDC 25MHz时钟,分辨率为800*480,16bit;计算得到刷新率38.27fps,带宽29.39MB/s
}

// LTDC,基本参数设置.
// 注意:此函数,必须在LTDC_Layer_Window_Config之前设置.
// layerx:层值,0/1.
// bufaddr:层颜色帧缓存起始地址
// pixformat:颜色格式.0,ARGB8888;1,RGB888;2,RGB565;3,ARGB1555;4,ARGB4444;5,L8;6;AL44;7;AL88
// alpha:层颜色Alpha值,0,全透明;255,不透明
// alpha0:默认颜色Alpha值,0,全透明;255,不透明
// bfac1:混合系数1,4(100),恒定的Alpha;6(101),像素Alpha*恒定Alpha
// bfac2:混合系数2,5(101),恒定的Alpha;7(111),像素Alpha*恒定Alpha
// bkcolor:层默认颜色,32位,低24位有效,RGB888格式
// 返回值:无
void LTDC_Layer_Parameter_Config(u8 layerx, u32 bufaddr, u8 pixformat, u8 alpha, u8 alpha0, u8 bfac1, u8 bfac2, u32 bkcolor)
{
    LTDC_LayerCfgTypeDef pLayerCfg;

    pLayerCfg.WindowX0 = 0;                                      // 窗口起始X坐标
    pLayerCfg.WindowY0 = 0;                                      // 窗口起始Y坐标
    pLayerCfg.WindowX1 = lcdltdc.pwidth;                         // 窗口终止X坐标
    pLayerCfg.WindowY1 = lcdltdc.pheight;                        // 窗口终止Y坐标
    pLayerCfg.PixelFormat = pixformat;                           // 像素格式
    pLayerCfg.Alpha = alpha;                                     // Alpha值设置，0~255,255为完全不透明
    pLayerCfg.Alpha0 = alpha0;                                   // 默认Alpha值
    pLayerCfg.BlendingFactor1 = (u32)bfac1 << 8;                 // 设置层混合系数
    pLayerCfg.BlendingFactor2 = (u32)bfac2 << 8;                 // 设置层混合系数
    pLayerCfg.FBStartAdress = bufaddr;                           // 设置层颜色帧缓存起始地址
    pLayerCfg.ImageWidth = lcdltdc.pwidth;                       // 设置颜色帧缓冲区的宽度
    pLayerCfg.ImageHeight = lcdltdc.pheight;                     // 设置颜色帧缓冲区的高度
    pLayerCfg.Backcolor.Red = (u8)(bkcolor & 0X00FF0000) >> 16;  // 背景颜色红色部分
    pLayerCfg.Backcolor.Green = (u8)(bkcolor & 0X0000FF00) >> 8; // 背景颜色绿色部分
    pLayerCfg.Backcolor.Blue = (u8)bkcolor & 0X000000FF;         // 背景颜色蓝色部分
    HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, layerx);            // 设置所选中的层
}

// LTDC,层颜窗口设置,窗口以LCD面板坐标系为基准
// 注意:此函数必须在LTDC_Layer_Parameter_Config之后再设置.
// layerx:层值,0/1.
// sx,sy:起始坐标
// width,height:宽度和高度
void LTDC_Layer_Window_Config(u8 layerx, u16 sx, u16 sy, u16 width, u16 height)
{
    HAL_LTDC_SetWindowPosition(&hltdc, sx, sy, layerx);    // 设置窗口的位置
    HAL_LTDC_SetWindowSize(&hltdc, width, height, layerx); // 设置窗口大小
}

// 打开LCD开关
// lcd_switch:1 打开,0，关闭
void LTDC_Switch(u8 sw)
{
    if (sw == 1)
        __HAL_LTDC_ENABLE(&hltdc);
    else if (sw == 0)
        __HAL_LTDC_DISABLE(&hltdc);
}

// 开关指定层
// layerx:层号,0,第一层; 1,第二层
// sw:1 打开;0关闭
void LTDC_Layer_Switch(u8 layerx, u8 sw)
{
    if (sw == 1)
        __HAL_LTDC_LAYER_ENABLE(&hltdc, layerx);
    else if (sw == 0)
        __HAL_LTDC_LAYER_DISABLE(&hltdc, layerx);
    __HAL_LTDC_RELOAD_CONFIG(&hltdc);
}

// 选择层
// layerx:层号;0,第一层;1,第二层;
void LTDC_Select_Layer(u8 layerx)
{
    lcdltdc.activelayer = layerx;
}

// 设置LCD显示方向
// dir:0,竖屏；1,横屏
void LTDC_Display_Dir(u8 dir)
{
    lcdltdc.dir = dir; // 显示方向
    if (dir == 0)      // 竖屏
    {
        lcdltdc.width = lcdltdc.pheight;
        lcdltdc.height = lcdltdc.pwidth;
    }
    else if (dir == 1) // 横屏
    {
        lcdltdc.width = lcdltdc.pwidth;
        lcdltdc.height = lcdltdc.pheight;
    }
}

// 画点函数
// x,y:写入坐标
// color:颜色值
void LTDC_Draw_Point(u16 x, u16 y, u32 color)
{
#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 || LCD_PIXFORMAT == LCD_PIXFORMAT_RGB888
    if (lcdltdc.dir) // 横屏
    {
        *(u32 *)((u32)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x)) = color;
    }
    else // 竖屏
    {
        *(u32 *)((u32)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x) + y)) = color;
    }
#else
    if (lcdltdc.dir) // 横屏
    {
        *(u16 *)((u32)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x)) = color;
    }
    else // 竖屏
    {
        *(u16 *)((u32)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)) = color;
    }
#endif
}

// 读点函数
// x,y:读取点的坐标
// 返回值:颜色值
u32 LTDC_Read_Point(u16 x, u16 y)
{
#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 || LCD_PIXFORMAT == LCD_PIXFORMAT_RGB888
    if (lcdltdc.dir) // 横屏
    {
        return *(u32 *)((u32)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x));
    }
    else // 竖屏
    {
        return *(u32 *)((u32)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x) + y));
    }
#else
    if (lcdltdc.dir) // 横屏
    {
        return *(u16 *)((u32)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x));
    }
    else // 竖屏
    {
        return *(u16 *)((u32)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y));
    }
#endif
}

// LTDC填充矩形,DMA2D填充
// (sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
// color:要填充的颜色
// 有时候需要频繁的调用填充函数，所以为了速度，填充函数采用寄存器版本，
// 不过下面有对应的库函数版本的代码。
void LTDC_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u32 color)
{
    u32 psx, psy, pex, pey; // 以LCD面板为基准的坐标系,不随横竖屏变化而变化
    u32 timeout = 0;
    u16 offline;
    u32 addr;
    // 坐标系转换
    if (lcdltdc.dir) // 横屏
    {
        psx = sx;
        psy = sy;
        pex = ex;
        pey = ey;
    }
    else // 竖屏
    {
        psx = sy;
        psy = lcdltdc.pheight - ex - 1;
        pex = ey;
        pey = lcdltdc.pheight - sx - 1;
    }
    offline = lcdltdc.pwidth - (pex - psx + 1);
    addr = ((u32)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * psy + psx));
    __HAL_RCC_DMA2D_CLK_ENABLE();   // 使能DM2D时钟
    DMA2D->CR &= ~(DMA2D_CR_START); // 先停止DMA2D
    DMA2D->CR = DMA2D_R2M;          // 寄存器到存储器模式
    DMA2D->OPFCCR = LCD_PIXFORMAT;  // 设置颜色格式
    DMA2D->OOR = offline;           // 设置行偏移

    DMA2D->OMAR = addr;                                     // 输出存储器地址
    DMA2D->NLR = (pey - psy + 1) | ((pex - psx + 1) << 16); // 设定行数寄存器
    DMA2D->OCOLR = color;                                   // 设定输出颜色寄存器
    DMA2D->CR |= DMA2D_CR_START;                            // 启动DMA2D
    while ((DMA2D->ISR & (DMA2D_FLAG_TC)) == 0)             // 等待传输完成
    {
        timeout++;
        if (timeout > 0X1FFFFF)
            break; // 超时退出
    }
    DMA2D->IFCR |= DMA2D_FLAG_TC; // 清除传输完成标志
}

// 在指定区域内填充指定颜色块,DMA2D填充
// 此函数仅支持u16,RGB565格式的颜色数组填充.
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
// 注意:sx,ex,不能大于lcdltdc.width-1;sy,ey,不能大于lcdltdc.height-1!!!
// color:要填充的颜色数组首地址
void LTDC_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 *color)
{
    u32 psx, psy, pex, pey; // 以LCD面板为基准的坐标系,不随横竖屏变化而变化
    u32 timeout = 0;
    u16 offline;
    u32 addr;
    // 坐标系转换
    if (lcdltdc.dir) // 横屏
    {
        psx = sx;
        psy = sy;
        pex = ex;
        pey = ey;
    }
    else // 竖屏
    {
        psx = sy;
        psy = lcdltdc.pheight - ex - 1;
        pex = ey;
        pey = lcdltdc.pheight - sx - 1;
    }
    offline = lcdltdc.pwidth - (pex - psx + 1);
    addr = ((u32)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * psy + psx));
    __HAL_RCC_DMA2D_CLK_ENABLE();   // 使能DM2D时钟
    DMA2D->CR &= ~(DMA2D_CR_START); // 先停止DMA2D
    DMA2D->CR = DMA2D_M2M;          // 存储器到存储器模式
    DMA2D->FGPFCCR = LCD_PIXFORMAT; // 设置颜色格式
    DMA2D->FGOR = 0;                // 前景层行偏移为0
    DMA2D->OOR = offline;           // 设置行偏移

    DMA2D->FGMAR = (u32)color;                              // 源地址
    DMA2D->OMAR = addr;                                     // 输出存储器地址
    DMA2D->NLR = (pey - psy + 1) | ((pex - psx + 1) << 16); // 设定行数寄存器
    DMA2D->CR |= DMA2D_CR_START;                            // 启动DMA2D
    while ((DMA2D->ISR & (DMA2D_FLAG_TC)) == 0)             // 等待传输完成
    {
        timeout++;
        if (timeout > 0X1FFFFF)
            break; // 超时退出
    }
    DMA2D->IFCR |= DMA2D_FLAG_TC; // 清除传输完成标志
}

// LCD清屏
// color:颜色值
void LTDC_Clear(u32 color)
{
    LTDC_Fill(0, 0, lcdltdc.width - 1, lcdltdc.height - 1, color);
}

// 画线
// x1,y1:起点坐标
// x2,y2:终点坐标
void LTDC_Draw_Line(u16 x1, u16 y1, u16 x2, u16 y2, u32 color)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; // 计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    if (delta_x > 0)
        incx = 1; // 设置单步方向
    else if (delta_x == 0)
        incx = 0; // 垂直线
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0; // 水平线
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)
        distance = delta_x; // 选取基本增量坐标轴
    else
        distance = delta_y;
    for (t = 0; t <= distance + 1; t++) // 画线输出
    {
        LTDC_Draw_Point(uRow, uCol, color); // 画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

// 画矩形
//(x1,y1),(x2,y2):矩形的对角坐标
void LTDC_Draw_Rectangle(u16 x1, u16 y1, u16 x2, u16 y2, u32 color)
{
    LTDC_Draw_Line(x1, y1, x2, y1, color);
    LTDC_Draw_Line(x1, y1, x1, y2, color);
    LTDC_Draw_Line(x1, y2, x2, y2, color);
    LTDC_Draw_Line(x2, y1, x2, y2, color);
}

// 在指定位置画一个指定大小的圆
//(x,y):中心点
// r    :半径
void LTDC_Draw_Circle(u16 x0, u16 y0, u8 r, u32 color)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1); // 判断下个点位置的标志
    while (a <= b)
    {
        LTDC_Draw_Point(x0 + a, y0 - b, color); // 5
        LTDC_Draw_Point(x0 + b, y0 - a, color); // 0
        LTDC_Draw_Point(x0 + b, y0 + a, color); // 4
        LTDC_Draw_Point(x0 + a, y0 + b, color); // 6
        LTDC_Draw_Point(x0 - a, y0 + b, color); // 1
        LTDC_Draw_Point(x0 - b, y0 + a, color);
        LTDC_Draw_Point(x0 - a, y0 - b, color); // 2
        LTDC_Draw_Point(x0 - b, y0 - a, color); // 7
        a++;
        // 使用Bresenham算法画圆
        if (di < 0)
            di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}

// 在指定位置显示一个字符
// x,y:起始坐标
// num:要显示的字符:" "--->"~"
// size:字体大小 1206/1608/2412/3216
// mode:叠加方式(1)还是非叠加方式(0)
void LTDC_Show_Char(u16 x, u16 y, u8 num, u8 size, u8 mode, u32 color)
{
    u8 temp, t1, t;
    u16 y0 = y;
    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); // 得到字体一个字符对应点阵集所占的字节数
    num = num - ' ';                                           // 得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
    for (t = 0; t < csize; t++)
    {
        if (size == 12)
            temp = lcd_asc2_1206[num][t]; // 调用1206字体
        else if (size == 16)
            temp = lcd_asc2_1608[num][t]; // 调用1608字体
        else if (size == 24)
            temp = lcd_asc2_2412[num][t]; // 调用2412字体
        else if (size == 32)
            temp = lcd_asc2_3216[num][t]; // 调用3216字体
        else
            return;                // 没有的字库
        for (t1 = 0; t1 < 8; t1++) // 一次画8个点
        {
            if (y >= lcdltdc.height || x >= lcdltdc.width)
                return; // 超区域了
            if (temp & 0x80)
                LTDC_Draw_Point(x, y, color);
            else if (mode == 0)
                LTDC_Draw_Point(x, y, BACK_COLOR);
            temp <<= 1;
            y++;
            if ((y - y0) == size) // 一列画完,字符画点换列
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}

// m^n函数
// 返回值:m^n次方.
u32 LCD_Pow(u8 m, u8 n)
{
    u32 result = 1;
    while (n--)
        result *= m;
    return result;
}

// 显示数字,高位为0,则不显示
// x,y :起点坐标
// len :数字的位数
// size:字体大小 1206/1608/2412/3216
// color:颜色
// num:数值(0~4294967295);
void LTDC_Show_Num(u16 x, u16 y, u32 num, u8 len, u8 size, u8 mode, u32 color)
{
    u8 t, temp;
    u8 enshow = 0;
    for (t = 0; t < len; t++)
    {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                LTDC_Show_Char(x + (size / 2) * t, y, ' ', size, mode, color);
                continue;
            }
            else
                enshow = 1;
        }
        LTDC_Show_Char(x + (size / 2) * t, y, temp + '0', size, mode, color);
    }
}

// 显示数字,高位为0,还是显示
// x,y:起点坐标
// num:数值(0~999999999);
// len:长度(即要显示的位数)
// size:字体大小 1206/1608/2412/3216
// mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LTDC_Show_xNum(u16 x, u16 y, u32 num, u8 len, u8 size, u8 mode, u32 color)
{
    u8 t, temp;
    u8 enshow = 0;
    for (t = 0; t < len; t++)
    {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                if (mode & 0X80)
                    LTDC_Show_Char(x + (size / 2) * t, y, '0', size, mode & 0X01, color);
                else
                    LTDC_Show_Char(x + (size / 2) * t, y, ' ', size, mode & 0X01, color);
                continue;
            }
            else
                enshow = 1;
        }
        LTDC_Show_Char(x + (size / 2) * t, y, temp + '0', size, mode & 0X01, color);
    }
}

// 显示字符串
// x,y:起点坐标,含0
// width,height:区域大小,不含0
// size:字体大小 1206/1608/2412/3216
//*p:字符串起始地址
void LTDC_Show_String(u16 x, u16 y, u16 width, u16 height, u8 size, u8 *p, u8 mode, u32 color)
{
    u8 x0 = x;
    width += x;
    height += y;
    while ((*p <= '~') && (*p >= ' ')) // 判断是不是非法字符!
    {
        if (x >= width)
        {
            x = x0;
            y += size;
        }
        if (y >= height)
            break; // 退出
        LTDC_Show_Char(x, y, *p, size, mode, color);
        x += size / 2;
        p++;
    }
}
