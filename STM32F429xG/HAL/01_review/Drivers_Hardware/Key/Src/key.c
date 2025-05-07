#include "key.h"
// #include "log_rtt.h"
#include "stm32f4xx_hal_gpio.h"

KEY_DATA KEY_UP_DATA;

// 按键初始化函数
void KEY_UP_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure = {0};

    KEY_UP_Port_CLK_ENABLE();

    GPIO_Initure.Pin = KEY_UP_PIN;
    GPIO_Initure.Mode = GPIO_MODE_INPUT; // 输入
    GPIO_Initure.Pull = GPIO_NOPULL;
    GPIO_Initure.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(KEY_UP_Port, &GPIO_Initure);

    KEY_UP_DATA.S1_LoopNumber = 3; // 50ms
    KEY_UP_DATA.S2_LoopNumber = 10;
    KEY_UP_DATA.S3_LoopNumber = 4;
}

// void KEY_4x4_Init_10(void)
// {
//     GPIO_InitTypeDef GPIO_Initure = {0};

//     KEY_4x4_1_Port_CLK_ENABLE();
//     KEY_4x4_2_Port_CLK_ENABLE();
//     KEY_4x4_3_Port_CLK_ENABLE();
//     KEY_4x4_4_Port_CLK_ENABLE();
//     KEY_4x4_5_Port_CLK_ENABLE();
//     KEY_4x4_6_Port_CLK_ENABLE();
//     KEY_4x4_7_Port_CLK_ENABLE();
//     KEY_4x4_8_Port_CLK_ENABLE();

//     GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
//     GPIO_Initure.Pull = GPIO_PULLUP;
//     GPIO_Initure.Speed = GPIO_SPEED_LOW;
//     GPIO_Initure.Pin = KEY_4x4_1_PIN;
//     HAL_GPIO_Init(KEY_4x4_1_Port, &GPIO_Initure);
//     GPIO_Initure.Pin = KEY_4x4_2_PIN;
//     HAL_GPIO_Init(KEY_4x4_2_Port, &GPIO_Initure);
//     GPIO_Initure.Pin = KEY_4x4_3_PIN;
//     HAL_GPIO_Init(KEY_4x4_3_Port, &GPIO_Initure);
//     GPIO_Initure.Pin = KEY_4x4_4_PIN;
//     HAL_GPIO_Init(KEY_4x4_4_Port, &GPIO_Initure);
//     GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
//     GPIO_Initure.Pull = GPIO_NOPULL;
//     GPIO_Initure.Pin = KEY_4x4_5_PIN;
//     HAL_GPIO_Init(KEY_4x4_5_Port, &GPIO_Initure);
//     GPIO_Initure.Pin = KEY_4x4_6_PIN;
//     HAL_GPIO_Init(KEY_4x4_6_Port, &GPIO_Initure);
//     GPIO_Initure.Pin = KEY_4x4_7_PIN;
//     HAL_GPIO_Init(KEY_4x4_7_Port, &GPIO_Initure);
//     GPIO_Initure.Pin = KEY_4x4_8_PIN;
//     HAL_GPIO_Init(KEY_4x4_8_Port, &GPIO_Initure);

//     KEY_4x4_1_Set();
//     KEY_4x4_2_Set();
//     KEY_4x4_3_Set();
//     KEY_4x4_4_Set();
//     KEY_4x4_5_Clr();
//     KEY_4x4_6_Clr();
//     KEY_4x4_7_Clr();
//     KEY_4x4_8_Clr();
// }

// void KEY_4x4_Init_01(void)
// {
//     GPIO_InitTypeDef GPIO_Initure = {0};

//     KEY_4x4_1_Port_CLK_ENABLE();
//     KEY_4x4_2_Port_CLK_ENABLE();
//     KEY_4x4_3_Port_CLK_ENABLE();
//     KEY_4x4_4_Port_CLK_ENABLE();
//     KEY_4x4_5_Port_CLK_ENABLE();
//     KEY_4x4_6_Port_CLK_ENABLE();
//     KEY_4x4_7_Port_CLK_ENABLE();
//     KEY_4x4_8_Port_CLK_ENABLE();

//     GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
//     GPIO_Initure.Pull = GPIO_NOPULL; // 上拉
//     GPIO_Initure.Speed = GPIO_SPEED_LOW;
//     GPIO_Initure.Pin = KEY_4x4_1_PIN;
//     HAL_GPIO_Init(KEY_4x4_1_Port, &GPIO_Initure);
//     GPIO_Initure.Pin = KEY_4x4_2_PIN;
//     HAL_GPIO_Init(KEY_4x4_2_Port, &GPIO_Initure);
//     GPIO_Initure.Pin = KEY_4x4_3_PIN;
//     HAL_GPIO_Init(KEY_4x4_3_Port, &GPIO_Initure);
//     GPIO_Initure.Pin = KEY_4x4_4_PIN;
//     HAL_GPIO_Init(KEY_4x4_4_Port, &GPIO_Initure);
//     GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
//     GPIO_Initure.Pull = GPIO_PULLUP; // 上拉
//     GPIO_Initure.Pin = KEY_4x4_5_PIN;
//     HAL_GPIO_Init(KEY_4x4_5_Port, &GPIO_Initure);
//     GPIO_Initure.Pin = KEY_4x4_6_PIN;
//     HAL_GPIO_Init(KEY_4x4_6_Port, &GPIO_Initure);
//     GPIO_Initure.Pin = KEY_4x4_7_PIN;
//     HAL_GPIO_Init(KEY_4x4_7_Port, &GPIO_Initure);
//     GPIO_Initure.Pin = KEY_4x4_8_PIN;
//     HAL_GPIO_Init(KEY_4x4_8_Port, &GPIO_Initure);

//     KEY_4x4_1_Clr();
//     KEY_4x4_2_Clr();
//     KEY_4x4_3_Clr();
//     KEY_4x4_4_Clr();
//     KEY_4x4_5_Set();
//     KEY_4x4_6_Set();
//     KEY_4x4_7_Set();
//     KEY_4x4_8_Set();
// }

// u8 read_key_val(void)
// {
//     static u8 key_val_temp = 0;
//     static u32 timesec = 0;
//     static u8 flag = 0, temp = 0;
//     if (flag == 0)
//     {
//         KEY_4x4_Init_10();
//         if (time40sec(1))
//             flag = 1;
//     }
//     if (flag == 1)
//     {
//         if (KEY_4x4_5_READ() || KEY_4x4_6_READ() || KEY_4x4_7_READ() || KEY_4x4_8_READ())
//         {
//             if (KEY_4x4_5_READ())
//                 key_val_temp += 0;
//             else if (KEY_4x4_6_READ())
//                 key_val_temp += 1;
//             else if (KEY_4x4_7_READ())
//                 key_val_temp += 2;
//             else if (KEY_4x4_8_READ())
//                 key_val_temp += 3;
//             flag = 2;
//             temp = 0;
//         }
//     }
//     if (flag == 2)
//     {
//         KEY_4x4_Init_01();
//         if (time40sec(1))
//             flag = 3;
//     }
//     if (flag == 3)
//     {
//         if (KEY_4x4_1_READ() || KEY_4x4_2_READ() || KEY_4x4_3_READ() || KEY_4x4_4_READ())
//         {
//             if (KEY_4x4_1_READ())
//                 key_val_temp += 0;
//             else if (KEY_4x4_2_READ())
//                 key_val_temp += 4;
//             else if (KEY_4x4_3_READ())
//                 key_val_temp += 8;
//             else if (KEY_4x4_4_READ())
//                 key_val_temp += 12;
//             key_val = key_val_temp;
//             key_val_temp = 0;
//             flag = 0;
//             return 1;
//         }
//     }
//     return 0;
// }

// 按键扫描
void KEY_Scan(KEY_DATA *KEY_DATA, uint8_t KEY_VAL)
{
    static uint16_t KEY_OLD = 0;
    if (KEY_VAL == 1 || KEY_DATA->K_stage) // 按下或者进入按键检测状态
    {
        /* 按键流程 */
        if (KEY_DATA->K_stage == 0) // 0阶段,按下
        {
            // LOGW("s0");
            KEY_DATA->K_stage++;
            KEY_DATA->K_value = 1;
        }
        else if (KEY_DATA->K_stage == 1) // 1阶段，长按抽样100ms
        {
            KEY_DATA->LoopNumber++;
            if (KEY_VAL == 1)
            {
                KEY_OLD++;
            }
            if (KEY_DATA->LoopNumber == KEY_DATA->S1_LoopNumber || KEY_DATA->S1_LoopNumber == 0) // 延时
            {
                // LOGW("s1");
                KEY_DATA->LoopNumber = 0;
                if (KEY_VAL == 1 && KEY_OLD == KEY_DATA->S1_LoopNumber) // 长按抽样
                {
                    KEY_DATA->K_stage++;
                }
                else
                {
                    KEY_DATA->K_stage = 0;
                    KEY_OLD = 0;
                }
            }
        }
        else if (KEY_DATA->K_stage == 2) // 2阶段，长按处理
        {
            KEY_DATA->LoopNumber++;
            if (KEY_DATA->LoopNumber == KEY_DATA->S2_LoopNumber || KEY_DATA->S2_LoopNumber == 0) // 延时0.5s
            {
                // LOGW("s2");
                KEY_DATA->LoopNumber = 0;
                if (KEY_VAL == 1) // 短抽样
                {
                    KEY_DATA->K_stage++; // 3阶段，连按间隔
                }
                else
                {
                    KEY_DATA->K_stage = 0;
                }
            }
        }
        else if (KEY_DATA->K_stage == 3) // 3阶段，连按间隔200ms
        {
            KEY_DATA->LoopNumber++;
            /* 检测按键是否松开 */
            if (KEY_DATA->LoopNumber == KEY_DATA->S3_LoopNumber || KEY_DATA->S3_LoopNumber == 0) // 调整按键手感
            {
                KEY_DATA->LoopNumber = 0;
                if (KEY_VAL == 1) // 短抽样
                {
                    KEY_DATA->K_value = 1;
                }
                else
                {
                    KEY_DATA->K_stage = 0;
                }
            }
        }
    }
}
