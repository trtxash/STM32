#ifndef _KEY_H
#define _KEY_H
#include "main.h"
#include "sys.h"

/* KEY GPIO */
#define KEY0_Port GPIOA
#define KEY1_Port GPIOE

/* KEY CLK */
#define KEY0_Port_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY1_Port_CLK_ENABLE() __HAL_RCC_GPIOE_CLK_ENABLE()

/* KEY PIN */
#define KEY0_PIN GPIO_PIN_0
#define KEY1_PIN GPIO_PIN_3

/* KEY 操作 */
#define KEY0_READ() KEY0_Port->IDR &KEY0_PIN // 0 or 1
#define KEY1_READ() KEY1_Port->IDR &KEY1_PIN // 0 or 1

#define KEY_4x4_1_Port GPIOA
#define KEY_4x4_1_Port_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY_4x4_1_PIN GPIO_PIN_0
#define KEY_4x4_2_Port GPIOA
#define KEY_4x4_2_Port_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY_4x4_2_PIN GPIO_PIN_0
#define KEY_4x4_3_Port GPIOA
#define KEY_4x4_3_Port_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY_4x4_3_PIN GPIO_PIN_0
#define KEY_4x4_4_Port GPIOA
#define KEY_4x4_4_Port_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY_4x4_4_PIN GPIO_PIN_0
#define KEY_4x4_5_Port GPIOA
#define KEY_4x4_5_Port_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY_4x4_5_PIN GPIO_PIN_0
#define KEY_4x4_6_Port GPIOA
#define KEY_4x4_6_Port_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY_4x4_6_PIN GPIO_PIN_0
#define KEY_4x4_7_Port GPIOA
#define KEY_4x4_7_Port_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY_4x4_7_PIN GPIO_PIN_0
#define KEY_4x4_8_Port GPIOA
#define KEY_4x4_8_Port_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY_4x4_8_PIN GPIO_PIN_0

// IO方向设置
#define KEY_4x4_1_IN()                            \
    {                                             \
        KEY_4x4_1_Port->MODER &= ~(3 << (1 * 2)); \
        KEY_4x4_1_Port->MODER |= 0 << 1 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_1_OUT()                           \
    {                                             \
        KEY_4x4_1_Port->MODER &= ~(3 << (1 * 2)); \
        KEY_4x4_1_Port->MODER |= 1 << 1 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_2_IN()                            \
    {                                             \
        KEY_4x4_2_Port->MODER &= ~(3 << (2 * 2)); \
        KEY_4x4_2_Port->MODER |= 0 << 2 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_2_OUT()                           \
    {                                             \
        KEY_4x4_2_Port->MODER &= ~(3 << (2 * 2)); \
        KEY_4x4_2_Port->MODER |= 1 << 2 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_3_IN()                            \
    {                                             \
        KEY_4x4_3_Port->MODER &= ~(3 << (3 * 2)); \
        KEY_4x4_3_Port->MODER |= 0 << 3 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_3_OUT()                           \
    {                                             \
        KEY_4x4_3_Port->MODER &= ~(3 << (3 * 2)); \
        KEY_4x4_3_Port->MODER |= 1 << 3 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_4_IN()                            \
    {                                             \
        KEY_4x4_4_Port->MODER &= ~(3 << (4 * 2)); \
        KEY_4x4_4_Port->MODER |= 0 << 4 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_4_OUT()                           \
    {                                             \
        KEY_4x4_4_Port->MODER &= ~(3 << (4 * 2)); \
        KEY_4x4_4_Port->MODER |= 1 << 4 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_5_IN()                            \
    {                                             \
        KEY_4x4_5_Port->MODER &= ~(3 << (5 * 2)); \
        KEY_4x4_5_Port->MODER |= 0 << 5 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_5_OUT()                           \
    {                                             \
        KEY_4x4_5_Port->MODER &= ~(3 << (5 * 2)); \
        KEY_4x4_5_Port->MODER |= 1 << 5 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_6_IN()                            \
    {                                             \
        KEY_4x4_6_Port->MODER &= ~(3 << (6 * 2)); \
        KEY_4x4_6_Port->MODER |= 0 << 6 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_6_OUT()                           \
    {                                             \
        KEY_4x4_6_Port->MODER &= ~(3 << (6 * 2)); \
        KEY_4x4_6_Port->MODER |= 1 << 6 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_7_IN()                            \
    {                                             \
        KEY_4x4_7_Port->MODER &= ~(3 << (7 * 2)); \
        KEY_4x4_7_Port->MODER |= 0 << 7 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_7_OUT()                           \
    {                                             \
        KEY_4x4_7_Port->MODER &= ~(3 << (7 * 2)); \
        KEY_4x4_7_Port->MODER |= 1 << 7 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_8_IN()                            \
    {                                             \
        KEY_4x4_8_Port->MODER &= ~(3 << (8 * 2)); \
        KEY_4x4_8_Port->MODER |= 0 << 8 * 2;      \
    } // 括号内数字为几号管脚
#define KEY_4x4_8_OUT()                           \
    {                                             \
        KEY_4x4_8_Port->MODER &= ~(3 << (8 * 2)); \
        KEY_4x4_8_Port->MODER |= 1 << 8 * 2;      \
    } // 括号内数字为几号管脚

#define KEY_4x4_1_Clr() KEY_4x4_1_Port->BSRR = (uint32_t)KEY_4x4_1_PIN << 16U
#define KEY_4x4_1_Set() KEY_4x4_1_Port->BSRR = KEY_4x4_1_PIN

#define KEY_4x4_2_Clr() KEY_4x4_2_Port->BSRR = (uint32_t)KEY_4x4_2_PIN << 16U
#define KEY_4x4_2_Set() KEY_4x4_2_Port->BSRR = KEY_4x4_2_PIN

#define KEY_4x4_3_Clr() KEY_4x4_3_Port->BSRR = (uint32_t)KEY_4x4_3_PIN << 16U
#define KEY_4x4_3_Set() KEY_4x4_3_Port->BSRR = KEY_4x4_3_PIN

#define KEY_4x4_4_Clr() KEY_4x4_4_Port->BSRR = (uint32_t)KEY_4x4_4_PIN << 16U
#define KEY_4x4_4_Set() KEY_4x4_4_Port->BSRR = KEY_4x4_4_PIN

#define KEY_4x4_5_Clr() KEY_4x4_5_Port->BSRR = (uint32_t)KEY_4x4_5_PIN << 16U
#define KEY_4x4_5_Set() KEY_4x4_5_Port->BSRR = KEY_4x4_5_PIN

#define KEY_4x4_6_Clr() KEY_4x4_6_Port->BSRR = (uint32_t)KEY_4x4_6_PIN << 16U
#define KEY_4x4_6_Set() KEY_4x4_6_Port->BSRR = KEY_4x4_6_PIN

#define KEY_4x4_7_Clr() KEY_4x4_7_Port->BSRR = (uint32_t)KEY_4x4_7_PIN << 16U
#define KEY_4x4_7_Set() KEY_4x4_7_Port->BSRR = KEY_4x4_7_PIN

#define KEY_4x4_8_Clr() KEY_4x4_8_Port->BSRR = (uint32_t)KEY_4x4_8_PIN << 16U
#define KEY_4x4_8_Set() KEY_4x4_8_Port->BSRR = KEY_4x4_8_PIN

#define KEY_4x4_1_READ() KEY_4x4_1_Port->IDR &KEY_4x4_1_PIN // 0 or 1
#define KEY_4x4_2_READ() KEY_4x4_2_Port->IDR &KEY_4x4_2_PIN // 0 or 1
#define KEY_4x4_3_READ() KEY_4x4_3_Port->IDR &KEY_4x4_3_PIN // 0 or 1
#define KEY_4x4_4_READ() KEY_4x4_4_Port->IDR &KEY_4x4_4_PIN // 0 or 1
#define KEY_4x4_5_READ() KEY_4x4_5_Port->IDR &KEY_4x4_5_PIN // 0 or 1
#define KEY_4x4_6_READ() KEY_4x4_6_Port->IDR &KEY_4x4_6_PIN // 0 or 1
#define KEY_4x4_7_READ() KEY_4x4_7_Port->IDR &KEY_4x4_7_PIN // 0 or 1
#define KEY_4x4_8_READ() KEY_4x4_8_Port->IDR &KEY_4x4_8_PIN // 0 or 1

extern u8 key_val;

void KEY0_Init(void);
void KEY1_Init(void);
void KEY_4x4_Init(void);

#endif
