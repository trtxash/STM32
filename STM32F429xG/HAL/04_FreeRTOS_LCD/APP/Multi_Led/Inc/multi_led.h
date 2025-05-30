/*
 * Copyright (c) 2016 Zibin Zheng <znbin@qq.com>
 * All rights reserved
 */

#ifndef _MULTI_LED_H_
#define _MULTI_LED_H_

#include <stdint.h>
#include <string.h>

// According to your need to modify the constants.
#define LED_TICKS_INTERVAL 50 // ms

typedef struct Led_t
{
    uint16_t ticks;
    uint16_t cycle;     // 多少ticks
    uint16_t dutycycle; // 前多少ticks active, dutycycle / cycle = 占空比
    uint8_t active_level : 1;
    uint8_t led_level : 1;
    uint8_t led_id;
    struct Led_t *next;
} Led_t;

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

/* user defined */

#endif
