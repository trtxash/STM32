/**
 * @file	01_Medicine_delivery_car
 * @brief
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2023年7月9号13点22分
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
    uart_init(115200);
    while (1)
    {
        u8 mpu_state = mpu_dmp_init();
        if (mpu_state == 0)
        {
            break;
        }
    }
    Tim_Encoder_Init();
    TB6612_init();
    MX_TIM6_Init((u16)(10000 - 1), (u16)(120 - 1)); // 定时器6初始化，周期10ms
    positional_pid_init(&motor1_velocity, 10, 0.1, 0, 1000, 0, 7199, -7199);
    positional_pid_init(&motor2_velocity, 10, 0.1, 0, 1000, 0, 7199, -7199);

    TARGET_V[0] = TARGET_V[1] = 100;
    while (1)
    {
        Get_Angle(1); // 读取角度
        sprintf(temp, "R:%0.2f ", Roll);
        OLED_ShowString(0, 0, temp, 8, 1, WHITE);
        sprintf(temp, "P:%0.2f ", Pitch);
        OLED_ShowString(64, 0, temp, 8, 1, WHITE);
        sprintf(temp, "Y:%0.2f ", Yaw);
        OLED_ShowString(0, 8, temp, 8, 1, WHITE);

        sprintf(temp, "l:%d ", Location_sum);
        OLED_ShowString(64, 8, temp, 8, 1, WHITE);
        sprintf(temp, "e1:%d ", Encoder[0]);
        OLED_ShowString(0, 16, temp, 8, 1, WHITE);
        sprintf(temp, "e2:%d ", Encoder[1]);
        OLED_ShowString(64, 16, temp, 8, 1, WHITE);

        // sprintf(temp, "%d ", );
        // OLED_ShowString(0, 24, temp, 8, 1, WHITE);
        // sprintf(temp, "%d ", Encoder[1]);
        // OLED_ShowString(64, 24, temp, 8, 1, WHITE);
    }
}