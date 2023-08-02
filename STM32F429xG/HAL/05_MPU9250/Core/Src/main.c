/**
 * @file	05_MPU9250
 * @brief
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2023年7月26号17点6分
 */

// 未完，有时间再补

#include "main.h"

short vr = 0, vl = 0;
u8 time_flag = 0;

/**
 * @brief	对函数简要描述
 * @param 	参数说明，以’:’作为参数结束标志；
 * @arg		参数里面可选择参量列举，对于可数情况可进行参量列举，同样以’:’作为参数结束标志；
 * @note  	注释，配合brief一起使用可以达到很好的注释效果；
 * @retval	返回值说明。
 */
int main(void)
{
    u8 temp[21] = {0};

    if (HAL_Init()) // 初始化HAL库
    {
        Error_Handler();
    }
    Stm32_Clock_Init(240, 12, RCC_PLLP_DIV2, 8); // 设置时钟
    delay_init(240);                             // 初始化延时函数
    MX_DMA_Init();
    MX_SPI6_Init();
    LED_Init();
    OLED_Init();
    while (1)
    {
        u8 i = mpu_dmp_init();

        if (i)
        {
            sprintf(temp, "MPU Error:%d ", i);
            OLED_ShowString(0, 32, temp, 8, 1, WHITE);
        }
        else
            break;
    }

    MX_TIM6_Init((u16)(50000 - 1), (u16)(120 - 1)); // 定时器6初始化，周期50ms
    MX_TIM7_Init((u16)(1000 - 1), (u16)(120 - 1));  // 定时器7初始化，周期1ms

    while (1)
    {
        static u32 i = 0;

        sprintf(temp, "%d ", i);
        OLED_ShowString(0, 0, temp, 8, 1, WHITE);
        OLED_ShowString(0, 8, temp, 8, 1, RED);
        OLED_ShowString(0, 16, temp, 8, 1, GREEN);
        OLED_ShowString(0, 24, temp, 8, 1, BLUE);
        i++;
        u8 res = mpu_mpl_get_data(&pitch, &roll, &yaw);
        if (res == 0)
        {
            sprintf(temp, "S:%d    ", res);
            OLED_ShowString(0, 32, temp, 8, 1, WHITE);
            sprintf(temp, "P:%0.1f ", pitch);
            OLED_ShowString(0, 40, temp, 8, 1, WHITE);
            sprintf(temp, "R:%0.1f ", roll);
            OLED_ShowString(0, 48, temp, 8, 1, WHITE);
            sprintf(temp, "Y:%0.1f ", yaw);
            OLED_ShowString(0, 56, temp, 8, 1, WHITE);
        }
        else
        {
            sprintf(temp, "S:%d    ", res);
            OLED_ShowString(0, 32, temp, 8, 1, WHITE);
        }
        delay_ms(50);
    }
}