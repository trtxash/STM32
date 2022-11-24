#ifndef _WS2812_H
#define _WS2812_H
#endif

#include "tim.h"
#include "delay.h"

#define WS2812_PWM_TIM_Init MX_TIM4_Init
#define WS2812_PWM_TIM_Hand htim4
#define WS2812_PWM_TIM_Ch TIM_CHANNEL_1

#define sys_clock 240 // 240M
#define PIXEL_NUM 64
#define NUM (24 * PIXEL_NUM + 300)             // Reset 280us / 1.25us = 224
#define WS1 (u32)(sys_clock / 2 / 0.8 * 2 / 3) // 1码比较值
#define WS0 (u32)(sys_clock / 2 / 0.8) - WS1   // 0码比较值

extern u16 send_Buf[NUM];

void ws2812init();

void WS_Load(void);
void WS_WriteAll_RGB(u8 n_R, u8 n_G, u8 n_B);
void WS_CloseAll(void);

u32 WS281x_Color(u8 red, u8 green, u8 blue);
void WS281x_SetPixelColor(uint16_t n, u32 GRBColor);
void WS281x_SetPixelRGB(uint16_t n, u8 red, u8 green, u8 blue);

u32 Wheel(u8 WheelPos);
void rgb_cycle(void);
void rainbow(u8 wait);
void rainbowCycle(u8 wait);
