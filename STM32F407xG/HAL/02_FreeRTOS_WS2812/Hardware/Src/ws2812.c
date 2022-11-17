#include "WS2812.h"

u16 send_Buf[NUM];

// 用了TIM8产生PWM波
void ws2812init()
{
    WS2812_PWM_TIM_Init((u32)(sys_clock / 2 / 0.8) - 1, 1 - 1, 0B0001); // PWM 频率 800K,通道1
}

// 启动DMA载入数据
void WS_Load(void)
{
    HAL_TIM_PWM_Start_DMA(&WS2812_PWM_TIM_Hand, WS2812_PWM_TIM_Ch, (u32 *)send_Buf, NUM);
}

// 关闭所有LED灯
void WS_CloseAll(void)
{
    u16 i;

    for (i = 0; i < PIXEL_NUM * 24; i++)
        send_Buf[i] = WS0; // 写入逻辑0的占空比

    for (i = PIXEL_NUM * 24; i < NUM; i++)
        send_Buf[i] = 0; // 占空比比为0，全为低电平
}

/**
 * @brief 全部led灯设置成一样的亮度，其中RGB分别设置亮度
 * WS2812的写入顺序是GRB，高位在前面
 */
void WS_WriteAll_RGB(u8 n_R, u8 n_G, u8 n_B)
{
    u16 i, j;
    u16 dat[24];

    // 将RGB数据进行转换
    for (i = 0; i < 8; i++)
    {
        dat[i] = ((n_G & 0x80) ? WS1 : WS0);
        n_G <<= 1;
    }
    for (i = 0; i < 8; i++)
    {
        dat[i + 8] = ((n_R & 0x80) ? WS1 : WS0);
        n_R <<= 1;
    }
    for (i = 0; i < 8; i++)
    {
        dat[i + 16] = ((n_B & 0x80) ? WS1 : WS0);
        n_B <<= 1;
    }

    for (i = 0; i < PIXEL_NUM; i++)
    {
        for (j = 0; j < 24; j++)
        {
            send_Buf[i * 24 + j] = dat[j];
        }
    }

    for (i = PIXEL_NUM * 24; i < NUM; i++)
    {
        send_Buf[i] = 0; // 占空比比为0，全为低电平
    }
}

u32 WS281x_Color(u8 red, u8 green, u8 blue)
{
    return green << 16 | red << 8 | blue;
}

/**
 * @brief  Set the color of No.n light in 24bit-format .
 * @note   None
 * @param  n: No.n lamp light, ranges from 0 to (PIXEL_NUM-1)
 * @param  GRBColor: color value of No.n light in 24bit-format(8 bit G + 8 bit R +8 bit B)
 * @retval None
 */
void WS281x_SetPixelColor(u16 n, u32 GRBColor)
{
    u8 i;
    if (n < PIXEL_NUM)
    {
        for (i = 0; i < 24; ++i)
        {
            send_Buf[24 * n + i] = (((GRBColor << i) & 0X800000) ? WS1 : WS0);
        }
    }
}

/**
 * @brief  Set the color of No.n light in RGB-format .
 * @note   None
 * @param  n: No.n lamp light, ranges from 0 to (PIXEL_NUM-1)
 * @param  red: red color value of No.n light in RGB-format
 * @param  green: greeen color value of No.n light in RGB-format
 * @param  blue: blue color value of No.n light in RGB-format
 * @retval None
 */
void WS281x_SetPixelRGB(u16 n, u8 red, u8 green, u8 blue)
{
    u8 i;

    if (n < PIXEL_NUM)
    {
        for (i = 0; i < 24; ++i)
        {
            send_Buf[24 * n + i] = (((WS281x_Color(red, green, blue) << i) & 0X800000) ? WS1 : WS0);
        }
    }
}

u32 Wheel(u8 WheelPos)
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85)
    {
        return WS281x_Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170)
    {
        WheelPos -= 85;
        return WS281x_Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return WS281x_Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void rainbow(u8 wait)
{
    u32 timestamp = HAL_GetTick();
    u16 i;
    static u8 j;
    static u32 next_time = 0;

    u32 flag = 0;
    if (next_time < wait)
    {
        if ((uint64_t)timestamp + wait - next_time > 0)
            flag = 1;
    }
    else if (timestamp > next_time)
    {
        flag = 1;
    }
    if (flag) // && (timestamp - next_time < wait*5))
    {
        j++;
        next_time = timestamp + wait;
        for (i = 0; i < PIXEL_NUM; i++)
        {
            WS281x_SetPixelColor(i, Wheel((i + j) & 255));
        }
    }
}

void rainbowCycle(u8 wait)
{
    u32 timestamp = HAL_GetTick();
    u16 i;
    static u8 j;
    static u32 next_time = 0;

    static u8 loop = 0;
    if (loop == 0)
        next_time = timestamp;
    loop = 1; // 首次调用初始化

    if ((timestamp > next_time)) // && (timestamp - next_time < wait*5))
    {
        j++;
        next_time = timestamp + wait;
        for (i = 0; i < PIXEL_NUM; i++)
        {
            WS281x_SetPixelColor(i, Wheel(((i * 256 / (PIXEL_NUM)) + j) & 255));
        }
    }
}
