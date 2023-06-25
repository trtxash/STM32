/**
 * @file	06_Balance_car
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
    OLED_Init();
    printf("\r\n初始化完成...\r\n");
    sprintf(temp, "OK!");
    OLED_ShowString(0, 0, temp, 8, 1, WHITE);

    while (1)
    {
        OLED_ShowNum(0, 8, i, 4, 8, 1, WHITE);
        delay_ms(1000);
        i++;
    }
}
