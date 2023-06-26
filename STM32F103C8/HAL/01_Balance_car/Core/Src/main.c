/**
 * @file	01_Balance_car
 * @brief
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2023年6月25号18点33分
 */

#include "main.h"

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
    u8 i = 0;
    float j;
    double v;

    if (HAL_Init()) // 初始化HAL库
    {
        Error_Handler();
    }
    Stm32_Clock_Init(RCC_PLL_MUL9); // 设置时钟,72M
    delay_init(72);                 // 初始化延时函数
    uart_init(115200);              // 初始化串口

    printf("\r\n初始化中...\r\n");

    MX_DMA_Init();  // 要先初始化DMA
    MX_ADC1_Init(); // 初始化ADC1
    OLED_Init();
    sprintf(temp, "oled OK!");
    OLED_ShowString(0, 0, temp, 8, 1, WHITE);
    OLED_Refresh();
    while (1)
    {
        u8 mpu_state = mpu_dmp_init();
        if (mpu_state == 0)
        {
            sprintf(temp, "mpu OK!   ");
            OLED_ShowString(64, 0, temp, 8, 1, WHITE);
            OLED_Refresh();
            break;
        }
        else
        {
            sprintf(temp, "mpu fal:%d", mpu_state);
            OLED_ShowString(64, 0, temp, 8, 1, WHITE);
            OLED_Refresh();
        }
    }
    HAL_ADCEx_Calibration_Start(&hadc1);                      // 启动ADC1校准，不校准将导致ADC测量不准确
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcx, ADC_Sec * 2); // 启动ADC+DMA

    printf("\r\n初始化完成...\r\n");
    // while (1)
    // {
    //     u8 i = mpu_dmp_init();
    //     if (i)
    //     {
    //         sprintf(temp, "mpu fal:%d", i);
    //         OLED_ShowString(64, 0, temp, 8, 1, WHITE);
    //         OLED_Refresh();
    //     }
    //     else
    //     {
    //         sprintf(temp, "mpu OK!   ");
    //         OLED_ShowString(64, 0, temp, 8, 1, WHITE);
    //         OLED_Refresh();
    //         break;
    //     }
    // }

    while (1)
    {
        j = Get_Adc_Average_DMA() * 3.3 * 11 * 100 / 1.0 / 4096 / 100 / 3;
        v = 9413.0184 - 303.204 * j * j * j * j * j + 114.0062 * j * j * j * j * j * j * j - 44.7181 * j * j * j * j * j * j * j * j + 6.547055 * j * j * j * j * j * j * j * j * j - 0.336029617 * j * j * j * j * j * j * j * j * j * j;

        sprintf(temp, "%d", i);
        OLED_ShowString(108, 0, temp, 8, 1, WHITE);
        sprintf(temp, "adc: %0.2f", j);
        OLED_ShowString(0, 8, temp, 8, 1, WHITE);
        sprintf(temp, "v: %0.2f  ", v);
        OLED_ShowString(64, 8, temp, 8, 1, WHITE);

        Get_Angle(1);
        sprintf(temp, "t: %d", Temperature);
        OLED_ShowString(0, 16, temp, 8, 1, WHITE);
        sprintf(temp, "R: %0.4f", Roll);
        OLED_ShowString(0, 24, temp, 8, 1, WHITE);
        sprintf(temp, "P: %0.4f", Pitch);
        OLED_ShowString(0, 32, temp, 8, 1, WHITE);
        sprintf(temp, "Y: %0.4f", Yaw);
        OLED_ShowString(0, 40, temp, 8, 1, WHITE);

        OLED_Refresh();
        // OLED_Clear();
        delay_ms(5);
        i++;
    }
}
