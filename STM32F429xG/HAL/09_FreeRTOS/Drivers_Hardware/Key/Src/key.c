#include "key.h"

u8 key_val = 0;

// 按键初始化函数
void KEY0_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure = {0};

    KEY0_Port_CLK_ENABLE();

    GPIO_Initure.Pin = KEY0_PIN;
    GPIO_Initure.Mode = GPIO_MODE_INPUT; // 输入
    GPIO_Initure.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(KEY0_Port, &GPIO_Initure);
}

void KEY1_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    KEY1_Port_CLK_ENABLE();

    GPIO_Initure.Pin = KEY1_PIN;
    GPIO_Initure.Mode = GPIO_MODE_INPUT; // 输入
    GPIO_Initure.Pull = GPIO_PULLUP;     // 上拉
    GPIO_Initure.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(KEY1_Port, &GPIO_Initure);
}

void KEY_4x4_Init_10(void)
{
    GPIO_InitTypeDef GPIO_Initure = {0};

    KEY_4x4_1_Port_CLK_ENABLE();
    KEY_4x4_2_Port_CLK_ENABLE();
    KEY_4x4_3_Port_CLK_ENABLE();
    KEY_4x4_4_Port_CLK_ENABLE();
    KEY_4x4_5_Port_CLK_ENABLE();
    KEY_4x4_6_Port_CLK_ENABLE();
    KEY_4x4_7_Port_CLK_ENABLE();
    KEY_4x4_8_Port_CLK_ENABLE();

    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_LOW;
    GPIO_Initure.Pin = KEY_4x4_1_PIN;
    HAL_GPIO_Init(KEY_4x4_1_Port, &GPIO_Initure);
    GPIO_Initure.Pin = KEY_4x4_2_PIN;
    HAL_GPIO_Init(KEY_4x4_2_Port, &GPIO_Initure);
    GPIO_Initure.Pin = KEY_4x4_3_PIN;
    HAL_GPIO_Init(KEY_4x4_3_Port, &GPIO_Initure);
    GPIO_Initure.Pin = KEY_4x4_4_PIN;
    HAL_GPIO_Init(KEY_4x4_4_Port, &GPIO_Initure);
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_NOPULL;
    GPIO_Initure.Pin = KEY_4x4_5_PIN;
    HAL_GPIO_Init(KEY_4x4_5_Port, &GPIO_Initure);
    GPIO_Initure.Pin = KEY_4x4_6_PIN;
    HAL_GPIO_Init(KEY_4x4_6_Port, &GPIO_Initure);
    GPIO_Initure.Pin = KEY_4x4_7_PIN;
    HAL_GPIO_Init(KEY_4x4_7_Port, &GPIO_Initure);
    GPIO_Initure.Pin = KEY_4x4_8_PIN;
    HAL_GPIO_Init(KEY_4x4_8_Port, &GPIO_Initure);

    KEY_4x4_1_Set();
    KEY_4x4_2_Set();
    KEY_4x4_3_Set();
    KEY_4x4_4_Set();
    KEY_4x4_5_Clr();
    KEY_4x4_6_Clr();
    KEY_4x4_7_Clr();
    KEY_4x4_8_Clr();
}

void KEY_4x4_Init_01(void)
{
    GPIO_InitTypeDef GPIO_Initure = {0};

    KEY_4x4_1_Port_CLK_ENABLE();
    KEY_4x4_2_Port_CLK_ENABLE();
    KEY_4x4_3_Port_CLK_ENABLE();
    KEY_4x4_4_Port_CLK_ENABLE();
    KEY_4x4_5_Port_CLK_ENABLE();
    KEY_4x4_6_Port_CLK_ENABLE();
    KEY_4x4_7_Port_CLK_ENABLE();
    KEY_4x4_8_Port_CLK_ENABLE();

    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_NOPULL; // 上拉
    GPIO_Initure.Speed = GPIO_SPEED_LOW;
    GPIO_Initure.Pin = KEY_4x4_1_PIN;
    HAL_GPIO_Init(KEY_4x4_1_Port, &GPIO_Initure);
    GPIO_Initure.Pin = KEY_4x4_2_PIN;
    HAL_GPIO_Init(KEY_4x4_2_Port, &GPIO_Initure);
    GPIO_Initure.Pin = KEY_4x4_3_PIN;
    HAL_GPIO_Init(KEY_4x4_3_Port, &GPIO_Initure);
    GPIO_Initure.Pin = KEY_4x4_4_PIN;
    HAL_GPIO_Init(KEY_4x4_4_Port, &GPIO_Initure);
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLUP; // 上拉
    GPIO_Initure.Pin = KEY_4x4_5_PIN;
    HAL_GPIO_Init(KEY_4x4_5_Port, &GPIO_Initure);
    GPIO_Initure.Pin = KEY_4x4_6_PIN;
    HAL_GPIO_Init(KEY_4x4_6_Port, &GPIO_Initure);
    GPIO_Initure.Pin = KEY_4x4_7_PIN;
    HAL_GPIO_Init(KEY_4x4_7_Port, &GPIO_Initure);
    GPIO_Initure.Pin = KEY_4x4_8_PIN;
    HAL_GPIO_Init(KEY_4x4_8_Port, &GPIO_Initure);

    KEY_4x4_1_Clr();
    KEY_4x4_2_Clr();
    KEY_4x4_3_Clr();
    KEY_4x4_4_Clr();
    KEY_4x4_5_Set();
    KEY_4x4_6_Set();
    KEY_4x4_7_Set();
    KEY_4x4_8_Set();
}

u8 read_key_val(void)
{
    static u8 key_val_temp = 0;
    static u32 timesec = 0;
    static u8 flag = 0, temp = 0;
    if (flag == 0)
    {
        KEY_4x4_Init_10();
        if (time40sec(1))
            flag = 1;
    }
    if (flag == 1)
    {
        if (KEY_4x4_5_READ() || KEY_4x4_6_READ() || KEY_4x4_7_READ() || KEY_4x4_8_READ())
        {
            if (KEY_4x4_5_READ())
                key_val_temp += 0;
            else if (KEY_4x4_6_READ())
                key_val_temp += 1;
            else if (KEY_4x4_7_READ())
                key_val_temp += 2;
            else if (KEY_4x4_8_READ())
                key_val_temp += 3;
            flag = 2;
            temp = 0;
        }
    }
    if (flag == 2)
    {
        KEY_4x4_Init_01();
        if (time40sec(1))
            flag = 3;
    }
    if (flag == 3)
    {
        if (KEY_4x4_1_READ() || KEY_4x4_2_READ() || KEY_4x4_3_READ() || KEY_4x4_4_READ())
        {
            if (KEY_4x4_1_READ())
                key_val_temp += 0;
            else if (KEY_4x4_2_READ())
                key_val_temp += 4;
            else if (KEY_4x4_3_READ())
                key_val_temp += 8;
            else if (KEY_4x4_4_READ())
                key_val_temp += 12;
            key_val = key_val_temp;
            key_val_temp = 0;
            flag = 0;
            return 1;
        }
    }
    return 0;
}