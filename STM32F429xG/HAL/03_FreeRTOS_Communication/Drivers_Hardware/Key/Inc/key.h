#ifndef _KEY_H
#define _KEY_H

#include "main.h"

/* KEY GPIO */
#define KEY_UP_Port   GPIOA

/* KEY CLK */
#define KEY_UP_Port_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()

/* KEY PIN */
#define KEY_UP_PIN   GPIO_PIN_0

/* KEY 操作 */
#define KEY_UP_READ()   KEY_UP_Port->IDR &KEY_UP_PIN     // 0 or 1

// #define KEY_4x4_1_Port GPIOG
// #define KEY_4x4_1_Port_CLK_ENABLE() __HAL_RCC_GPIOG_CLK_ENABLE()
// #define KEY_4x4_1_PIN GPIO_PIN_7
// #define KEY_4x4_2_Port GPIOG
// #define KEY_4x4_2_Port_CLK_ENABLE() __HAL_RCC_GPIOG_CLK_ENABLE()
// #define KEY_4x4_2_PIN GPIO_PIN_6
// #define KEY_4x4_3_Port GPIOA
// #define KEY_4x4_3_Port_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
// #define KEY_4x4_3_PIN GPIO_PIN_12
// #define KEY_4x4_4_Port GPIOA
// #define KEY_4x4_4_Port_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
// #define KEY_4x4_4_PIN GPIO_PIN_11
// #define KEY_4x4_5_Port GPIOB
// #define KEY_4x4_5_Port_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
// #define KEY_4x4_5_PIN GPIO_PIN_0
// #define KEY_4x4_6_Port GPIOA
// #define KEY_4x4_6_Port_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
// #define KEY_4x4_6_PIN GPIO_PIN_4
// #define KEY_4x4_7_Port GPIOF
// #define KEY_4x4_7_Port_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
// #define KEY_4x4_7_PIN GPIO_PIN_10
// #define KEY_4x4_8_Port GPIOD
// #define KEY_4x4_8_Port_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
// #define KEY_4x4_8_PIN GPIO_PIN_4

/*

//注意注释反斜杠会导致下一行被注释

// IO方向设置
#define KEY_4x4_1_IN()                            \
    {                                             \
        KEY_4x4_1_Port->MODER &= ~(3 << (7 * 2)); \
        KEY_4x4_1_Port->MODER |= 0 << 7 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_1_OUT()                           \
    {                                             \
        KEY_4x4_1_Port->MODER &= ~(3 << (7 * 2)); \
        KEY_4x4_1_Port->MODER |= 1 << 7 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_2_IN()                            \
    {                                             \
        KEY_4x4_2_Port->MODER &= ~(3 << (6 * 2)); \
        KEY_4x4_2_Port->MODER |= 0 << 6 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_2_OUT()                           \
    {                                             \
        KEY_4x4_2_Port->MODER &= ~(3 << (6 * 2)); \
        KEY_4x4_2_Port->MODER |= 1 << 6 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_3_IN()                             \
    {                                              \
        KEY_4x4_3_Port->MODER &= ~(3 << (12 * 2)); \
        KEY_4x4_3_Port->MODER |= 0 << 12 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_3_OUT()                            \
    {                                              \
        KEY_4x4_3_Port->MODER &= ~(3 << (12 * 2)); \
        KEY_4x4_3_Port->MODER |= 1 << 12 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_4_IN()                             \
    {                                              \
        KEY_4x4_4_Port->MODER &= ~(3 << (11 * 2)); \
        KEY_4x4_4_Port->MODER |= 0 << 11 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_4_OUT()                            \
    {                                              \
        KEY_4x4_4_Port->MODER &= ~(3 << (11 * 2)); \
        KEY_4x4_4_Port->MODER |= 1 << 11 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_5_IN()                            \
    {                                             \
        KEY_4x4_5_Port->MODER &= ~(3 << (0 * 2)); \
        KEY_4x4_5_Port->MODER |= 0 << 0 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_5_OUT()                           \
    {                                             \
        KEY_4x4_5_Port->MODER &= ~(3 << (0 * 2)); \
        KEY_4x4_5_Port->MODER |= 1 << 0 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_6_IN()                             \
    {                                              \
        KEY_4x4_6_Port->MODER &= ~(3 << (10 * 2)); \
        KEY_4x4_6_Port->MODER |= 0 << 10 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_6_OUT()                            \
    {                                              \
        KEY_4x4_6_Port->MODER &= ~(3 << (10 * 2)); \
        KEY_4x4_6_Port->MODER |= 1 << 10 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_7_IN()                            \
    {                                             \
        KEY_4x4_7_Port->MODER &= ~(3 << (4 * 2)); \
        KEY_4x4_7_Port->MODER |= 0 << 4 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_7_OUT()                           \
    {                                             \
        KEY_4x4_7_Port->MODER &= ~(3 << (4 * 2)); \
        KEY_4x4_7_Port->MODER |= 1 << 4 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_8_IN()                            \
    {                                             \
        KEY_4x4_8_Port->MODER &= ~(3 << (4 * 2)); \
        KEY_4x4_8_Port->MODER |= 0 << 4 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_8_OUT()                           \
    {                                             \
        KEY_4x4_8_Port->MODER &= ~(3 << (4 * 2)); \
        KEY_4x4_8_Port->MODER |= 1 << 4 * 2;      \
    } // 括号内数字为几号管脚

*/

// #define KEY_4x4_1_Clr() KEY_4x4_1_Port->BSRR = (uint32_t)KEY_4x4_1_PIN << 16U
// #define KEY_4x4_1_Set() KEY_4x4_1_Port->BSRR = KEY_4x4_1_PIN

// #define KEY_4x4_2_Clr() KEY_4x4_2_Port->BSRR = (uint32_t)KEY_4x4_2_PIN << 16U
// #define KEY_4x4_2_Set() KEY_4x4_2_Port->BSRR = KEY_4x4_2_PIN

// #define KEY_4x4_3_Clr() KEY_4x4_3_Port->BSRR = (uint32_t)KEY_4x4_3_PIN << 16U
// #define KEY_4x4_3_Set() KEY_4x4_3_Port->BSRR = KEY_4x4_3_PIN

// #define KEY_4x4_4_Clr() KEY_4x4_4_Port->BSRR = (uint32_t)KEY_4x4_4_PIN << 16U
// #define KEY_4x4_4_Set() KEY_4x4_4_Port->BSRR = KEY_4x4_4_PIN

// #define KEY_4x4_5_Clr() KEY_4x4_5_Port->BSRR = (uint32_t)KEY_4x4_5_PIN << 16U
// #define KEY_4x4_5_Set() KEY_4x4_5_Port->BSRR = KEY_4x4_5_PIN

// #define KEY_4x4_6_Clr() KEY_4x4_6_Port->BSRR = (uint32_t)KEY_4x4_6_PIN << 16U
// #define KEY_4x4_6_Set() KEY_4x4_6_Port->BSRR = KEY_4x4_6_PIN

// #define KEY_4x4_7_Clr() KEY_4x4_7_Port->BSRR = (uint32_t)KEY_4x4_7_PIN << 16U
// #define KEY_4x4_7_Set() KEY_4x4_7_Port->BSRR = KEY_4x4_7_PIN

// #define KEY_4x4_8_Clr() KEY_4x4_8_Port->BSRR = (uint32_t)KEY_4x4_8_PIN << 16U
// #define KEY_4x4_8_Set() KEY_4x4_8_Port->BSRR = KEY_4x4_8_PIN

// #define KEY_4x4_1_READ() KEY_4x4_1_Port->IDR &KEY_4x4_1_PIN // 0 or 1
// #define KEY_4x4_2_READ() KEY_4x4_2_Port->IDR &KEY_4x4_2_PIN // 0 or 1
// #define KEY_4x4_3_READ() KEY_4x4_3_Port->IDR &KEY_4x4_3_PIN // 0 or 1
// #define KEY_4x4_4_READ() KEY_4x4_4_Port->IDR &KEY_4x4_4_PIN // 0 or 1
// #define KEY_4x4_5_READ() KEY_4x4_5_Port->IDR &KEY_4x4_5_PIN // 0 or 1
// #define KEY_4x4_6_READ() KEY_4x4_6_Port->IDR &KEY_4x4_6_PIN // 0 or 1
// #define KEY_4x4_7_READ() KEY_4x4_7_Port->IDR &KEY_4x4_7_PIN // 0 or 1
// #define KEY_4x4_8_READ() KEY_4x4_8_Port->IDR &KEY_4x4_8_PIN // 0 or 1

// extern u8 key_val;

typedef struct
{
    uint8_t K_stage;
    uint8_t K_value; // 值
    uint16_t LoopNumber;
    uint16_t S1_LoopNumber;
    uint16_t S2_LoopNumber;
    uint16_t S3_LoopNumber;
} KEY_DATA;

extern KEY_DATA KEY_UP_DATA;
extern KEY_DATA KEY_BOOT_DATA;

void KEY_UP_Init(void);
void KEY_BOOT_Init(void);
void KEY_Scan(KEY_DATA *KEY_DATA, uint8_t KEY_VAL);
// void KEY_4x4_Init_10(void);
// void KEY_4x4_Init_01(void);
// u8 read_key_val(void);

#endif
