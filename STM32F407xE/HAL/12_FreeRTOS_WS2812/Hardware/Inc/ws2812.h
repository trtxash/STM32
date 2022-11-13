#ifndef _WS2812_H
#define _WS2812_H
#endif

#include "timer.h"
#include "delay.h"

#define WS2812_PWM_TIM_Init TIM4_PWM_Init
#define WS2812_PWM_TIM_Hand TIM4_Handler
#define WS2812_PWM_TIM_Ch TIM_CHANNEL_1

#define sys_clock 240 // 240M
#define PIXEL_NUM 2
#define NUM (24 * PIXEL_NUM + 300)             // Reset 280us / 1.25us = 224
#define WS1 (u32)(sys_clock / 2 / 0.8 * 2 / 3) // 1码比较值为61-->850us
#define WS0 (u32)(sys_clock / 2 / 0.8) - WS1   // 0码比较值为28-->400us

extern u16 send_Buf[NUM];

void ws2812init();

void WS_Load(void);
void WS_WriteAll_RGB(uint8_t n_R, uint8_t n_G, uint8_t n_B);
void WS_CloseAll(void);

uint32_t WS281x_Color(uint8_t red, uint8_t green, uint8_t blue);
void WS281x_SetPixelColor(uint16_t n, uint32_t GRBColor);
void WS281x_SetPixelRGB(uint16_t n, uint8_t red, uint8_t green, uint8_t blue);

uint32_t Wheel(uint8_t WheelPos);
void rainbow(uint8_t wait);
void rainbowCycle(uint8_t wait);
