#include "lcd.h"
#include "dma2d.h"
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
    LCD_BLK_Init(); // 背光IO初始化
    LTDC_Init();    // LTDC初始化
    // MX_DMA2D_Init(); // DMA2D初始化
    // LCD_BLK_Set();   // 开背光
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
// 注意:sx,ex,不能大于lcddev.width-1;sy,ey,不能大于lcddev.height-1!!!
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
