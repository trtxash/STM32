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

void KEY_4x4_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    KEY_4x4_1_Port_CLK_ENABLE();
    KEY_4x4_2_Port_CLK_ENABLE();
    KEY_4x4_3_Port_CLK_ENABLE();
    KEY_4x4_4_Port_CLK_ENABLE();
    KEY_4x4_5_Port_CLK_ENABLE();
    KEY_4x4_6_Port_CLK_ENABLE();
    KEY_4x4_7_Port_CLK_ENABLE();
    KEY_4x4_8_Port_CLK_ENABLE();

    GPIO_Initure.Mode = GPIO_MODE_INPUT; // 输入
    GPIO_Initure.Pull = GPIO_PULLUP;     // 上拉
    GPIO_Initure.Speed = GPIO_SPEED_LOW;
    GPIO_Initure.Pin = KEY_4x4_1_PIN;
    HAL_GPIO_Init(KEY_4x4_1_Port, &GPIO_Initure);
    GPIO_Initure.Pin = KEY_4x4_2_PIN;
    HAL_GPIO_Init(KEY_4x4_2_Port, &GPIO_Initure);
    GPIO_Initure.Pin = KEY_4x4_3_PIN;
    HAL_GPIO_Init(KEY_4x4_3_Port, &GPIO_Initure);
    GPIO_Initure.Pin = KEY_4x4_4_PIN;
    HAL_GPIO_Init(KEY_4x4_4_Port, &GPIO_Initure);
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP; // 输出
    GPIO_Initure.Pull = GPIO_PULLUP;         // 上拉
    GPIO_Initure.Pin = KEY_4x4_5_PIN;
    HAL_GPIO_Init(KEY_4x4_5_Port, &GPIO_Initure);
    GPIO_Initure.Pin = KEY_4x4_6_PIN;
    HAL_GPIO_Init(KEY_4x4_6_Port, &GPIO_Initure);
    GPIO_Initure.Pin = KEY_4x4_7_PIN;
    HAL_GPIO_Init(KEY_4x4_7_Port, &GPIO_Initure);
    GPIO_Initure.Pin = KEY_4x4_8_PIN;
    HAL_GPIO_Init(KEY_4x4_8_Port, &GPIO_Initure);
}

u8 read_key_val(void)
{
    static u8 key_val_temp = 0;
    static u32 timesec = 0;
    u8 flag = 0;
    if (flag == 0)
    {
        KEY_4x4_5_IN();
        KEY_4x4_6_IN();
        KEY_4x4_7_IN();
        KEY_4x4_8_IN();
        KEY_4x4_1_OUT();
        KEY_4x4_2_OUT();
        KEY_4x4_3_OUT();
        KEY_4x4_4_OUT();
        KEY_4x4_1_Set();
        KEY_4x4_2_Set();
        KEY_4x4_3_Set();
        KEY_4x4_4_Set();
        if (KEY_4x4_5_READ() | KEY_4x4_6_READ() | KEY_4x4_7_READ() | KEY_4x4_8_READ())
        {
            if (KEY_4x4_5_READ())
                key_val_temp += 0;
            else if (KEY_4x4_6_READ())
                key_val_temp += 1;
            else if (KEY_4x4_7_READ())
                key_val_temp += 2;
            else if (KEY_4x4_8_READ())
                key_val_temp += 3;
            flag = 1;
        }
    }
    else if (flag == 1)
    {
        KEY_4x4_1_IN();
        KEY_4x4_2_IN();
        KEY_4x4_3_IN();
        KEY_4x4_4_IN();
        KEY_4x4_5_OUT();
        KEY_4x4_6_OUT();
        KEY_4x4_7_OUT();
        KEY_4x4_8_OUT();
        KEY_4x4_5_Set();
        KEY_4x4_6_Set();
        KEY_4x4_7_Set();
        KEY_4x4_8_Set();
        if (KEY_4x4_1_READ() | KEY_4x4_2_READ() | KEY_4x4_3_READ() | KEY_4x4_4_READ())
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
            flag = 2;
        }
    }
    else if (flag == 2 & time40flag)
    {
        time40flag = 0;
        timesec++;
        if (timesec > 25)
        {
            timesec = 0;
            flag = 0;
        }
    }
}