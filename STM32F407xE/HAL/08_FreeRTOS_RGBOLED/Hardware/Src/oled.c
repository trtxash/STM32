/**
 * @file	oled.c
 * @brief 	OLED相关驱动
 *          这是相关API
 * @author 	TRTX-gamer       https://github.com/TRTX-gamer;
 *          突然吐血    https://space.bilibili.com/12890038;
 * @version 1.3
 * @date 	2022年11月2号13点02分
 */

/**
 * 软件模拟，未开启GCC优化
 * IIC最大185fps，延时  TIM3_Init(540 - 1, 840 - 1);
 * SPI最大720fps，延时  TIM3_Init(139 - 1, 840 - 1);
 *
 * 硬件，未开启GCC优化
 * IIC最大126fps，延时  TIM3_Init(795 - 1, 840 - 1);I2C最大1.2Mbit/s
 * SPI最大495FPS，延时  TIM3_Init(202 - 1, 840 - 1);SPI最大42Mbit/s。因为用了HAL，速度较慢
 *
 * DMA,未开启GCC优化
 * I2C,DMA下FPS不好测，估算 126FPS，实际写入可能低一点,I2C最大1.2Mbit/s
 * SPI,DMA下FPS不好测，估算 5,126FPS，实际写入可能低一点,SPI最大42Mbit/s
 */

/**
 * 硬件
 * SPI最大17FPS
 * DMA
 * SPI,DMA下FPS不好测，估算 57FPS
 */

/**
 * 软件模拟优点：波特率高，速度快，可移植性好
 *        缺点：占用管脚口，使用MCU资源多，不太稳定，编时序复杂
 * 硬件优缺点和软件模拟相反
 *
 * 本次实验还可以继续优化
 *    优化方向1：不用HAL的SPI发送，用寄存器；
 */

#include "oled.h"
#include "oledfont.h"

OLED_DrawData_HandleTypeDef DrawData;

#if _OLED_DRIVER_IC_TYPE == OLED_SSD1306_SSD1315
static u8 OLED_GRAM[OLED_WIDTH][OLED_HEIGHT / 8] = {0}; // OLED画布,与寻址方式初始化有关
#elif _OLED_DRIVER_IC_TYPE == OLED_SSD1351
static u8 OLED_GRAM[OLED_HEIGHT][OLED_WIDTH * 2] = {0}; // OLED画布,与寻址方式初始化有关
#endif
/* 相关选择-------------------------------------------------------------------------------------------------- */

/**
 * @brief   OLED发送命令
 * @param   dat
 * @arg		u8，命令
 * @note
 * @retval  void
 */
void OLED_WR_CMD(u8 dat)
{
#if _SOFT_OR_HARE == OLED_SOFT

#if _DRIVE_INTERFACE_TYPE == OLED_IIC_INTERFACE
    OledDrv_IICStart();
    OledDrv_IICSendByte(OLED_ADDRESS);
    OledDrv_IICWaitAck();
    OledDrv_IICSendByte(0x00);
    OledDrv_IICWaitAck();
    OledDrv_IICSendByte(dat);
    OledDrv_IICWaitAck();
    OledDrv_IICStop();
#elif _DRIVE_INTERFACE_TYPE == OLED_SPI_INTERFACE // SPI通信
    char i = 8;

    OLED_DC_Clr();
    OLED_CS_Clr();
    while (i--)
    {
        OLED_CLK_Clr();
        if (dat & 0x80)
        {
            OLED_DIN_Set();
        }
        else
        {
            OLED_DIN_Clr();
        }
        OLED_CLK_Set();
        dat <<= 1;
    }
    OLED_CS_Set();
    OLED_DC_Set();
#endif

#elif _SOFT_OR_HARE == OLED_HARD

#if _DRIVE_INTERFACE_TYPE == OLED_IIC_INTERFACE
    HAL_I2C_Mem_Write(&OLED_I2C_HandleTypeDef, OLED_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, &dat, 1, 100);
#elif _DRIVE_INTERFACE_TYPE == OLED_SPI_INTERFACE // SPI通信
    OLED_DC_Clr();
    OLED_CS_Clr();
    HAL_SPI_Transmit(&OLED_SPI_HandleTypeDef, &dat, 1, 100);
    OLED_CS_Set();
    OLED_DC_Set();
#endif

#endif
}

/**
 * @brief   OLED发送数据
 * @param   dat
 * @arg		u8，数据
 * @note
 * @retval  void
 */
void OLED_WR_DATA8(u8 dat)
{
#if _SOFT_OR_HARE == OLED_SOFT

#if _DRIVE_INTERFACE_TYPE == OLED_IIC_INTERFACE
    OledDrv_IICStart();
    OledDrv_IICSendByte(OLED_ADDRESS);
    OledDrv_IICWaitAck();
    OledDrv_IICSendByte(0x40);
    OledDrv_IICWaitAck();
    OledDrv_IICSendByte(dat);
    OledDrv_IICWaitAck();
    OledDrv_IICStop();
#elif _DRIVE_INTERFACE_TYPE == OLED_SPI_INTERFACE // SPI通信
    char i = 8;

    OLED_CS_Clr();
    while (i--)
    {
        OLED_CLK_Clr();
        if (dat & 0x80)
        {
            OLED_DIN_Set();
        }
        else
        {
            OLED_DIN_Clr();
        }
        OLED_CLK_Set();
        dat <<= 1;
    }
    OLED_CS_Set();
#endif

#elif _SOFT_OR_HARE == OLED_HARD

#if _DRIVE_INTERFACE_TYPE == OLED_IIC_INTERFACE
    HAL_I2C_Mem_Write(&OLED_I2C_HandleTypeDef, OLED_ADDRESS, 0x40, I2C_MEMADD_SIZE_8BIT, &dat, 1, 100);
#elif _DRIVE_INTERFACE_TYPE == OLED_SPI_INTERFACE // SPI通信
    OLED_CS_Clr();
    HAL_SPI_Transmit(&OLED_SPI_HandleTypeDef, &dat, 1, 100);
    OLED_CS_Set();
#endif

#endif
}

/**
 * @brief   OLED发送数据
 * @param   dat
 * @arg		u16，数据
 * @note
 * @retval  void
 */
void OLED_WR_DATA16(u16 dat)
{
    OLED_WR_DATA8(dat >> 8); // 先发高八位
    OLED_WR_DATA8(dat);
}

// 更新显存到OLED
void OLED_Refresh(void)
{
#if _OLED_DMA == OLED_DMA_DISABLE
    u16 i, n;
#if _OLED_DRIVER_IC_TYPE == OLED_SSD1306_SSD1315
    for (n = 0; n < OLED_WIDTH; n++)
    {
#if _DRIVE_INTERFACE_TYPE == OLED_IIC_INTERFACE
        for (i = 0; i < OLED_HEIGHT / 8; i++)
        {
            OLED_WR_DATA8(OLED_GRAM[n][i]);
        }
#elif _DRIVE_INTERFACE_TYPE == OLED_SPI_INTERFACE // SPI通信
        for (i = 0; i < OLED_HEIGHT / 8; i++)
        {

            OLED_WR_DATA8(OLED_GRAM[n][i]);
        }

#endif
    }
#elif _OLED_DRIVER_IC_TYPE == OLED_SSD1351
    for (i = 0; i < OLED_HEIGHT; i++)
    {
        for (n = 0; n < OLED_WIDTH * 2; n++)
        {
            OLED_WR_DATA8(OLED_GRAM[i][n]);
        }
    }
#endif
#elif _OLED_DMA == OLED_DMA_ABLE

#if _DRIVE_INTERFACE_TYPE == OLED_IIC_INTERFACE
    HAL_I2C_Mem_Write_DMA(&OLED_I2C_HandleTypeDef, OLED_ADDRESS, 0x40, I2C_MEMADD_SIZE_8BIT, OLED_GRAM, OLED_WIDTH * OLED_HEIGHT / 8);
#elif _DRIVE_INTERFACE_TYPE == OLED_SPI_INTERFACE

#if _OLED_DRIVER_IC_TYPE == OLED_SSD1306_SSD1315
    OLED_CS_Clr();
    HAL_SPI_Transmit_DMA(&OLED_SPI_HandleTypeDef, OLED_GRAM, OLED_WIDTH * OLED_HEIGHT / 8); // DMA循环，运行一次就行
#elif _OLED_DRIVER_IC_TYPE == OLED_SSD1351
    OLED_CS_Clr();
    HAL_SPI_Transmit_DMA(&OLED_SPI_HandleTypeDef, OLED_GRAM, OLED_WIDTH * OLED_HEIGHT * 2); // DMA循环，运行一次就行
#endif

#endif

#endif
}

// 开启OLED显示
void OLED_DisPlay_On(void)
{
#if _OLED_DRIVER_IC_TYPE == OLED_SSD1306_SSD1315
    OLED_WR_CMD(0x8D); // 电荷泵使能
    OLED_WR_CMD(0x14); // 开启电荷泵
    OLED_WR_CMD(0xAF); // 点亮屏幕
#elif _OLED_DRIVER_IC_TYPE == OLED_SSD1351
#endif
}

// 关闭OLED显示
void OLED_DisPlay_Off(void)
{
#if _OLED_DRIVER_IC_TYPE == OLED_SSD1306_SSD1315
    OLED_WR_CMD(0x8D); // 电荷泵使能
    OLED_WR_CMD(0x10); // 关闭电荷泵
    OLED_WR_CMD(0xAE); // 关闭屏幕
#elif _OLED_DRIVER_IC_TYPE == OLED_SSD1351
#endif
}

// 清屏函数
void OLED_Clear(void)
{
    u8 i, n;
    for (n = 0; n < OLED_WIDTH; n++)
    {
        for (i = 0; i < OLED_HEIGHT / 8; i++)
        {
            OLED_GRAM[n][i] = 0; // 清除所有数据
        }
    }
}

// 画点
//  x:0~127
//  y:0~63
//  t:1 填充 0,清空
void OLED_DrawPoint(u16 x, u8 y, u8 t, u16 color)
{
#if _OLED_DRIVER_IC_TYPE == OLED_SSD1306_SSD1315
    u8 i, m, n;
    i = y / 8;
    m = y % 8;
    n = 1 << m;
    if (t)
    {
        OLED_GRAM[x][i] |= n;
    }
    else
    {
        OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
        OLED_GRAM[x][i] |= n;
        OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
    }
#elif _OLED_DRIVER_IC_TYPE == OLED_SSD1351
    if (t)
    {
        OLED_GRAM[y][2 * x] = color >> 8;
        OLED_GRAM[y][2 * x + 1] = color;
    }
    else
    {
        OLED_GRAM[y][2 * x] = BLACK >> 8;
        OLED_GRAM[y][2 * x + 1] = BLACK;
    }
#endif
}

// 画线
//  x1,y1:起点坐标
//  x2,y2:结束坐标
void OLED_DrawLine(u16 x1, u8 y1, u16 x2, u8 y2, u8 mode, u16 color)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; // 计算坐标增量
    delta_y = y2 - y1;
    uRow = x1; // 画线起点坐标
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
        delta_y = -delta_x;
    }
    if (delta_x > delta_y)
        distance = delta_x; // 选取基本增量坐标轴
    else
        distance = delta_y;
    for (t = 0; t < distance + 1; t++)
    {
        OLED_DrawPoint(uRow, uCol, mode, color); // 画点
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
// x,y:圆心坐标
// r:圆的半径
void OLED_DrawCircle(u16 x, u8 y, u8 r, u16 color)
{
    int a, b, num;
    a = 0;
    b = r;
    while (2 * b * b >= r * r)
    {
        OLED_DrawPoint(x + a, y - b, 1, color);
        OLED_DrawPoint(x - a, y - b, 1, color);
        OLED_DrawPoint(x - a, y + b, 1, color);
        OLED_DrawPoint(x + a, y + b, 1, color);

        OLED_DrawPoint(x + b, y + a, 1, color);
        OLED_DrawPoint(x + b, y - a, 1, color);
        OLED_DrawPoint(x - b, y - a, 1, color);
        OLED_DrawPoint(x - b, y + a, 1, color);

        a++;
        num = (a * a + b * b) - r * r; // 计算画的点离圆心的距离
        if (num > 0)
        {
            b--;
            a--;
        }
    }
}

// 在指定位置显示一个字符,包括部分字符
//  x:0~127
//  y:0~63
//  size1:选择字体 6x8/6x12/8x16/12x24
//  mode:0,反色显示;1,正常显示
void OLED_ShowChar(u16 x, u8 y, u8 chr, u8 size1, u8 mode, u16 color)
{
    u16 i, m, temp, size2, chr1;
    u16 x0 = x, y0 = y;
    if (size1 == 8)
        size2 = 6;
    else
        size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2); // 得到字体一个字符对应点阵集所占的字节数
    chr1 = chr - ' ';                                              // 计算偏移后的值
    for (i = 0; i < size2; i++)
    {
        if (size1 == 8)
        {
            temp = asc2_0806[chr1][i];
        } // 调用0806字体
        else if (size1 == 12)
        {
            temp = asc2_1206[chr1][i];
        } // 调用1206字体
        else if (size1 == 16)
        {
            temp = asc2_1608[chr1][i];
        } // 调用1608字体
        else if (size1 == 24)
        {
            temp = asc2_2412[chr1][i];
        } // 调用2412字体
        else
            return;
        for (m = 0; m < 8; m++)
        {
            if (temp & 0x01)
                OLED_DrawPoint(x, y, mode, color);
            else
                OLED_DrawPoint(x, y, !mode, color);
            temp >>= 1;
            y++;
        }
        x++;
        if ((size1 != 8) && ((x - x0) == size1 / 2))
        {
            x = x0;
            y0 = y0 + 8;
        }
        y = y0;
    }
}

// 显示字符串
//  x,y:起点坐标
//  size1:字体大小
//*chr:字符串起始地址
//  mode:0,反色显示;1,正常显示
void OLED_ShowString(u16 x, u8 y, u8 *chr, u8 size1, u8 mode, u16 color)
{
    while ((*chr >= ' ') && (*chr <= '~')) // 判断是不是非法字符!
    {
        OLED_ShowChar(x, y, *chr, size1, mode, color);
        if (size1 == 8)
            x += 6;
        else
            x += size1 / 2;
        chr++;
    }
}

// m^n
u32 OLED_Pow(u8 m, u8 n)
{
    u32 result = 1;
    while (n--)
    {
        result *= m;
    }
    return result;
}

// 显示数字
//  x,y :起点坐标
//  num :要显示的数字
//  len :数字的位数
//  size:字体大小
//  mode:0,反色显示;1,正常显示
void OLED_ShowNum(u16 x, u8 y, u32 num, u8 len, u8 size1, u8 mode, u16 color)
{
    u8 t, temp, m = 0;
    if (size1 == 8)
        m = 2;
    for (t = 0; t < len; t++)
    {
        temp = (num / OLED_Pow(10, len - t - 1)) % 10;
        if (temp == 0)
        {
            OLED_ShowChar(x + (size1 / 2 + m) * t, y, '0', size1, mode, color);
        }
        else
        {
            OLED_ShowChar(x + (size1 / 2 + m) * t, y, temp + '0', size1, mode, color);
        }
    }
}

// 显示汉字
//  x,y:起点坐标
//  num:汉字对应的序号
//  mode:0,反色显示;1,正常显示
void OLED_ShowChinese(u16 x, u8 y, u8 num, u8 size1, u8 mode, u16 color)
{
    u8 m, temp;
    u8 x0 = x, y0 = y;
    u16 i, size3 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * size1; // 得到字体一个字符对应点阵集所占的字节数
    for (i = 0; i < size3; i++)
    {
        if (size1 == 16)
        {
            temp = Hzk1[num][i];
        } // 调用16*16字体
        else if (size1 == 24)
        {
            temp = Hzk2[num][i];
        } // 调用24*24字体
        else if (size1 == 32)
        {
            temp = Hzk3[num][i];
        } // 调用32*32字体
        else if (size1 == 64)
        {
            temp = Hzk4[num][i];
        } // 调用64*64字体
        else
            return;
        for (m = 0; m < 8; m++)
        {
            if (temp & 0x01)
                OLED_DrawPoint(x, y, mode, color);
            else
                OLED_DrawPoint(x, y, !mode, color);
            temp >>= 1;
            y++;
        }
        x++;
        if ((x - x0) == size1)
        {
            x = x0;
            y0 = y0 + 8;
        }
        y = y0;
    }
}

// num 显示汉字的个数
// space 每一遍显示的间隔
// mode:0,反色显示;1,正常显示
void OLED_ScrollDisplay(u8 num, u8 space, u8 mode, u16 color)
{
    u8 i, n, t = 0, m = 0, r;
    while (1)
    {
        if (m == 0)
        {
            OLED_ShowChinese(128, 24, t, 16, mode, color); // 写入一个汉字保存在OLED_GRAM[][]数组中
            t++;
        }
        if (t == num)
        {
            for (r = 0; r < 16 * space; r++) // 显示间隔
            {
                for (i = 1; i < 144; i++)
                {
                    for (n = 0; n < 8; n++)
                    {
                        OLED_GRAM[i - 1][n] = OLED_GRAM[i][n];
                    }
                }
                OLED_Refresh();
            }
            t = 0;
        }
        m++;
        if (m == 16)
        {
            m = 0;
        }
        for (i = 1; i < 144; i++) // 实现左移
        {
            for (n = 0; n < 8; n++)
            {
                OLED_GRAM[i - 1][n] = OLED_GRAM[i][n];
            }
        }
        OLED_Refresh();
    }
}

// x,y：起点坐标
// sizex,sizey,图片长宽
// BMP[]：要写入的图片数组
// mode:0,反色显示;1,正常显示
void OLED_ShowPicture(u16 x, u8 y, u8 sizex, u8 sizey, u8 BMP[], u8 mode)
{
    u16 j = 0;
    u8 i, n, temp, m;
    u8 x0 = x, y0 = y;
    sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
    for (n = 0; n < sizey; n++)
    {
        for (i = 0; i < sizex; i++)
        {
            temp = BMP[j];
            j++;
            for (m = 0; m < 8; m++)
            {
                if (temp & 0x01)
                    OLED_DrawPoint(x, y, mode, temp);
                else
                    OLED_DrawPoint(x, y, !mode, temp);
                temp >>= 1;
                y++;
            }
            x++;
            if ((x - x0) == sizex)
            {
                x = x0;
                y0 = y0 + 8;
            }
            y = y0;
        }
    }
}

// OLED的初始化
void OLED_Init(void)
{
    OledDrv_Init();
#if _OLED_DRIVER_IC_TYPE == OLED_SSD1306_SSD1315
    OLED_WR_CMD(0xAE); //--turn off oled panel
    OLED_WR_CMD(0x00); //---set low column address
    OLED_WR_CMD(0x10); //---set high column address
    OLED_WR_CMD(0x40); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_CMD(0x81); //--set contrast control register
    OLED_WR_CMD(0xCF); // Set SEG Output Current Brightness
    OLED_WR_CMD(0xA1); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WR_CMD(0xC8); // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_WR_CMD(0xA6); //--set normal display
    OLED_WR_CMD(0xA8); //--set multiplex ratio(1 to 64)
    OLED_WR_CMD(0x3f); //--1/64 duty
    OLED_WR_CMD(0xD3); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_CMD(0x00); //-not offset
    OLED_WR_CMD(0xd5); //--set display clock divide ratio/oscillator frequency
    OLED_WR_CMD(0x80); //--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_CMD(0xD9); //--set pre-charge period
    OLED_WR_CMD(0xF1); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_CMD(0xDA); //--set com pins hardware configuration
    OLED_WR_CMD(0x12);
    OLED_WR_CMD(0xDB); //--set vcomh
    OLED_WR_CMD(0x40); // Set VCOM Deselect Level
    OLED_WR_CMD(0x20); //-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_CMD(0x01); // 垂直寻址模式
    OLED_WR_CMD(0x8D); //--set Charge Pump enable/disable
    OLED_WR_CMD(0x14); //--set(0x10) disable
    OLED_WR_CMD(0xA4); // Disable Entire Display On (0xa4/0xa5)
    OLED_WR_CMD(0xA6); // Disable Inverse Display On (0xa6/a7)
    OLED_Clear();
    OLED_WR_CMD(0xAF);
#elif _OLED_DRIVER_IC_TYPE == OLED_SSD1351
    OLED_WR_CMD(0xFD);   // Set Command Lock
    OLED_WR_DATA8(0x12); // Unlock OLED driver IC MCU interface from entering command [reset]
    OLED_WR_CMD(0xFD);   // Set Command Lock
    OLED_WR_DATA8(0xB1); // Command A2,B1,B3,BB,BE accessible if inunlock state
    OLED_WR_CMD(0xAE);   // Set Sleep mode ON/OFF，Sleep mode On (Display OFF)
    OLED_WR_CMD(0xB3);   // Front Clock Divider(DivSet)/ Oscillator Frequency
    OLED_WR_DATA8(0xF0); // A[7:4]Oscillator frequency, frequency increases as level increases [reset=1101b]，A[3:0]divide by 1；
    OLED_WR_CMD(0xCA);   // Set MUX Ratio
    OLED_WR_DATA8(0x7F); // A[6:0] MUX ratio 16MUX ~ 128MUX, [reset=127],(Range from 15 to 127)
    OLED_WR_CMD(0xA2);   // Set Display Offset
    OLED_WR_DATA8(0x00); // Set vertical scroll by Row from 0-127. [reset=60h] Note (1) This command is locked by Command FDh by default. To unlock it, please refer to Command FDh.
    OLED_WR_CMD(0xA1);   // Set Display Start Line
    OLED_WR_DATA8(0x00); // Set vertical scroll by RAM from 0~127. [reset=00h]
    OLED_WR_CMD(0xA0);   // Set Re-map / Color Depth(Display RAM to Panel)
    OLED_WR_DATA8(0x74);
    /*
    A[0]=0b, Horizontal address increment [reset]
    A[0]=1b, Vertical address increment
    A[1]=0b, Column address 0 is mapped to SEG0 [reset]
    A[1]=1b, Column address 127 is mapped to SEG0
    A[2]=0b, Color sequence: A → B → C [reset]
    A[2]=1b, Color sequence is swapped: C → B → A
    A[3]=0b, Reserved
    A[3]=1b, Reserved
    A[4]=0b, Scan from COM0 to COM[N –1] [reset]
    A[4]=1b, Scan from COM[N-1] to COM0. Where N is the
    Multiplex ratio.
    A[5]=0b, Disable COM Split Odd Even [reset]
    A[5]=1b, Enable COM Split Odd Even
    A[7:6] Set Color Depth,
    00b 256 color
    01b 65K color, [reset]
    10b 262k color, 8/18-bit,16 bit (1st option) MCU interface
    11b 262k color, 16 - bit MCU interface (2nd option)
    Refer to section for 8.3.2 details.
    */
    OLED_WR_CMD(0xB5); // Set GPIO
    OLED_WR_DATA8(0x00);
    /*
    A[1:0] GPIO0: 00 pin HiZ, Input disabled
    01 pin HiZ, Input enabled
    10 pin output LOW [reset]
    11 pin output HIGH
    A[3:2] GPIO1: 00 pin HiZ, Input disabled
    01 pin HiZ, Input enabled
    10 pin output LOW [reset]
    11 pin output HIGH
    */
    OLED_WR_CMD(0xAB); // Function Selection
    OLED_WR_DATA8(0x01);
    /*
    A[0]=0b, Select external VDD
    A[0]=1b, Enable internal VDD regulator [reset]
    A[7:6]=00b, Select 8-bit parallel interface [reset]
    A[7:6]=01b, Select 16-bit parallel interface
    A[7:6]=11b, Select 18-bit parallel interface
    */
    OLED_WR_CMD(0xB4); // Set Segment Low Voltage (VSL)
    OLED_WR_DATA8(0xA0);
    OLED_WR_DATA8(0xB5); // 固定值
    OLED_WR_DATA8(0x55); // 固定值
    /*
    A[3:0] sets the VSL voltage as follow:
    A[1:0]=00 External VSL [reset]
    A[1:0]=10 Internal VSL (kept VSL pin NC)
    Note
    (1) When external VSL is enabled, in order to avoid distortion
    in display pattern, an external circuit is needed to connect
    between VSL and VSS as shown in Figure 14-1.
    */
    OLED_WR_CMD(0xC1); // Set Contrast Current for Color A,B,C
    OLED_WR_DATA8(0xC8);
    OLED_WR_DATA8(0x80);
    OLED_WR_DATA8(0xC8);
    /*
    A[7:0] Contrast Value Color A [reset=10001010b]
    B[7:0] Contrast Value Color B [reset=01010001b]
    C[7:0] Contrast Value Color C [reset=10001010b]
    */
    OLED_WR_CMD(0xC7); // Master Contrast Current Control
    OLED_WR_DATA8(0x0F);
    /*
    A[3:0] :
    0000b reduce output currents for all colors to 1/16
    0001b reduce output currents for all colors to 2/16
    ....
    1110b reduce output currents for all colors to 15/16
    1111b no change [reset = 1111b]
    */
    OLED_WR_CMD(0xB8); // Look Up Table for Gray Scale Pulse width
    /*
    The next 63 data bytes define Gray Scale (GS) Table by
    setting the gray scale pulse width in unit of DCLK’s
    (ranges from 0d ~ 180d)
    */
    OLED_WR_DATA8(0x02); // Gray Scale Level 1
    OLED_WR_DATA8(0x03); // Gray Scale Level 2
    OLED_WR_DATA8(0x04); // Gray Scale Level 3
    OLED_WR_DATA8(0x05); // Gray Scale Level 4
    OLED_WR_DATA8(0x06); // Gray Scale Level 5
    OLED_WR_DATA8(0x07); // Gray Scale Level 6
    OLED_WR_DATA8(0x08); // Gray Scale Level 7
    OLED_WR_DATA8(0x09); // Gray Scale Level 8
    OLED_WR_DATA8(0x0A); // Gray Scale Level 9
    OLED_WR_DATA8(0x0B); // Gray Scale Level 10
    OLED_WR_DATA8(0x0C); // Gray Scale Level 11
    OLED_WR_DATA8(0x0D); // Gray Scale Level 12
    OLED_WR_DATA8(0x0E); // Gray Scale Level 13
    OLED_WR_DATA8(0x0F); // Gray Scale Level 14
    OLED_WR_DATA8(0x10); // Gray Scale Level 15
    OLED_WR_DATA8(0x11); // Gray Scale Level 16
    OLED_WR_DATA8(0x12); // Gray Scale Level 17
    OLED_WR_DATA8(0x13); // Gray Scale Level 18
    OLED_WR_DATA8(0x15); // Gray Scale Level 19
    OLED_WR_DATA8(0x17); // Gray Scale Level 20
    OLED_WR_DATA8(0x19); // Gray Scale Level 21
    OLED_WR_DATA8(0x1B); // Gray Scale Level 22
    OLED_WR_DATA8(0x1D); // Gray Scale Level 23
    OLED_WR_DATA8(0x1F); // Gray Scale Level 24
    OLED_WR_DATA8(0x21); // Gray Scale Level 25
    OLED_WR_DATA8(0x23); // Gray Scale Level 26
    OLED_WR_DATA8(0x25); // Gray Scale Level 27
    OLED_WR_DATA8(0x27); // Gray Scale Level 28
    OLED_WR_DATA8(0x2A); // Gray Scale Level 29
    OLED_WR_DATA8(0x2D); // Gray Scale Level 30
    OLED_WR_DATA8(0x30); // Gray Scale Level 31
    OLED_WR_DATA8(0x33); // Gray Scale Level 32
    OLED_WR_DATA8(0x36); // Gray Scale Level 33
    OLED_WR_DATA8(0x39); // Gray Scale Level 34
    OLED_WR_DATA8(0x3C); // Gray Scale Level 35
    OLED_WR_DATA8(0x3F); // Gray Scale Level 36
    OLED_WR_DATA8(0x42); // Gray Scale Level 37
    OLED_WR_DATA8(0x45); // Gray Scale Level 38
    OLED_WR_DATA8(0x48); // Gray Scale Level 39
    OLED_WR_DATA8(0x4C); // Gray Scale Level 40
    OLED_WR_DATA8(0x50); // Gray Scale Level 41
    OLED_WR_DATA8(0x54); // Gray Scale Level 42
    OLED_WR_DATA8(0x58); // Gray Scale Level 43
    OLED_WR_DATA8(0x5C); // Gray Scale Level 44
    OLED_WR_DATA8(0x60); // Gray Scale Level 45
    OLED_WR_DATA8(0x64); // Gray Scale Level 46
    OLED_WR_DATA8(0x68); // Gray Scale Level 47
    OLED_WR_DATA8(0x6C); // Gray Scale Level 48
    OLED_WR_DATA8(0x70); // Gray Scale Level 49
    OLED_WR_DATA8(0x74); // Gray Scale Level 50
    OLED_WR_DATA8(0x78); // Gray Scale Level 51
    OLED_WR_DATA8(0x7D); // Gray Scale Level 52
    OLED_WR_DATA8(0x82); // Gray Scale Level 53
    OLED_WR_DATA8(0x87); // Gray Scale Level 54
    OLED_WR_DATA8(0x8C); // Gray Scale Level 55
    OLED_WR_DATA8(0x91); // Gray Scale Level 56
    OLED_WR_DATA8(0x96); // Gray Scale Level 57
    OLED_WR_DATA8(0x9B); // Gray Scale Level 58
    OLED_WR_DATA8(0xA0); // Gray Scale Level 59
    OLED_WR_DATA8(0xA5); // Gray Scale Level 60
    OLED_WR_DATA8(0xAA); // Gray Scale Level 61
    OLED_WR_DATA8(0xAF); // Gray Scale Level 62
    OLED_WR_DATA8(0xB4); // Gray Scale Level 63

    OLED_WR_CMD(0xB1); // Set Reset (Phase 1) / Precharge (Phase2) period
    OLED_WR_DATA8(0x32);
    /*
    A[3:0] Phase 1 period of 5~31 DCLK(s) clocks
    [reset=0010b]
    A[3:0]:
    0-1 invalid
    2 = 5 DCLKs
    3 = 7 DCLKs
    :
    15 =31DCLKs
    A[7:4] Phase 2 period of 3~15 DCLK(s) clocks
    [reset=1000b]
    A[7:4]:
    0-2 invalid
    3 = 3 DCLKs
    4 = 4 DCLKs
    :
    15 =15DCLKs
    Note
    (1) 0 DCLK is invalid in phase 1 & phase 2
    (2)This command is locked by Command FDh by default. To
    unlock it, please refer to Command FDh.
    */
    OLED_WR_CMD(0xBB); // Set Pre-charge voltage
    OLED_WR_DATA8(0x17);
    // OLED_WR_CMD(0xB2);
    // OLED_WR_DATA8(0xA4);
    // OLED_WR_DATA8(0x00);
    // OLED_WR_DATA8(0x00);
    OLED_WR_CMD(0xB6); // Set Second Precharge Period
    OLED_WR_DATA8(0x01);
    OLED_WR_CMD(0xBE); // Set VCOMH Voltage
    OLED_WR_DATA8(0x05);
    OLED_WR_CMD(0xA6); // Set Display Mode
    /*
    A4h: All OFF
    A5h: All ON (All pixels have GS63)
    A6h : Reset to normal display [reset]
    A7h: Inverse Display (GS0 -> GS63, GS1 -> GS62, ....)
    */
    OLED_WR_CMD(0xAF); // Set Sleep mode ON/OFF，AFh = Sleep mode OFF (Display ON)
    OLED_WR_CMD(0x5C); // Enable MCU to write Data into RAM
                       // After entering this single byte command, data entries will be written into the display RAM until another
                       // command is written. Address pointer is increased accordingly. This command must be sent before write data
                       // into RAM.
#endif

    OLED_Refresh();
}