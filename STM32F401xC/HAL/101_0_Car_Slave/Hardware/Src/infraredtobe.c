#include "infraredtobe.h"

//按键初始化函数
void Infraredtobe_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    GPIO_InitTypeDef GPIO_Initure_2;

    __HAL_RCC_GPIOB_CLK_ENABLE(); //开启GPIOB时钟
    __HAL_RCC_GPIOA_CLK_ENABLE(); //开启GPIOA时钟

    GPIO_Initure.Pin = GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15; // PB0 PB1 PB2 PB10
    GPIO_Initure.Mode = GPIO_MODE_INPUT;                                                                             //输入
    GPIO_Initure.Pull = GPIO_PULLUP;                                                                                 //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;                                                                            //高速
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);
    
    GPIO_Initure_2.Pin = GPIO_PIN_4;       // PB4 PB5 PB6 PB7
    GPIO_Initure_2.Mode = GPIO_MODE_INPUT; //输入
    GPIO_Initure_2.Pull = GPIO_PULLUP;     //上拉
    GPIO_Initure_2.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_Initure_2);
}

u8 Read_Infraredtobe_bits(void)
{
    u8 temp;
    static u8 temp_last = 0;
    // temp = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) + 10 * HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) + 100 * HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) + 1000 * HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10);
    temp = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) + 2 * HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) + 4 * HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) + 8 * HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) + 16 * HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) + 32 * HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) + 64 * HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) + 128 * HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);

    return temp;
}

int Read_Infraredtobe_sums(void)
{
    int temp = 0;
    static int temp_last = 0;

    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)) // 管1无红外
    {
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)) // 管2无红外
        {
            temp = 3;
        }
        else
        {
            temp = 2;
        }
    }
    else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)) // 管2无红外
    {
        temp = 1;
    }

    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)) // 管4无红外
    {
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)) // 管3无红外
        {
            temp = -3;
        }
        else
        {
            temp = -2;
        }
    }
    else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)) // 管3无红外
    {
        temp = -1;
    }

    if (temp == 0) // temp无更新，返回上次
    {
        return temp_last;
    }
    temp_last = temp; // 更新temp_last

    return temp;
}