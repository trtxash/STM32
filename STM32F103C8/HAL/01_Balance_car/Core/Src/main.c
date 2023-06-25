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
    printf("\r\n初始化完成...\r\n");
    sprintf(temp, "OK!");
    OLED_ShowString(0, 0, temp, 8, 1, WHITE);
    OLED_Refresh();
    HAL_ADCEx_Calibration_Start(&hadc1);                      // 启动ADC1校准，不校准将导致ADC测量不准确
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcx, ADC_Sec * 2); // 启动ADC+DMA

    while (1)
    {
        u16 j = Get_Adc_Average_DMA();
        printf("\r\ni = %d\r\n", i);
        printf("adc = %d\r\n", j);
        sprintf(temp, "i = %d", i);
        OLED_ShowString(0, 8, temp, 8, 1, WHITE);
        sprintf(temp, "adc = %d", j);
        OLED_ShowString(0, 16, temp, 8, 1, WHITE);
        OLED_Refresh();
        delay_ms(1000);
        i++;
    }
}
