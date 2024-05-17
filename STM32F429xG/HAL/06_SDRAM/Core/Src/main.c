/**
 * @file	05_MPU9250
 * @brief
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2023年7月26号17点6分
 */

// 未完，有时间再补

#include "main.h"

// u32 time40flag = 0;

/**
 * @brief	对函数简要描述
 * @param 	参数说明，以’:’作为参数结束标志；
 * @arg		参数里面可选择参量列举，对于可数情况可进行参量列举，同样以’:’作为参数结束标志；
 * @note  	注释，配合brief一起使用可以达到很好的注释效果；
 * @retval	返回值说明。
 */
int main(void)
{
    // u8 temp[21] = {0};

    if (HAL_Init()) // 初始化HAL库
    {
        Error_Handler();
    }
    Stm32_Clock_Init(180, 12, RCC_PLLP_DIV2, 8); // 设置时钟
    // delay_init(180);                             // 初始化延时函数
    // MX_DMA_Init();
    // MX_SPI6_Init();
    // OLED_Init();
    // uart_init(115200);   // 舵机
    // uart2_init(1384200); // OpenMV
    // uart6_init(1384200); // 蓝牙
    // LED_Init();
    // KEY0_Init();
    // MX_TIM5_Init((u16)(20000 - 1), (u16)(90 - 1)); // 定时器5初始化，周期20ms
    // MX_TIM6_Init((u16)(20000 - 1), (u16)(90 - 1)); // 定时器6初始化，周期20ms
    // MX_TIM7_Init((u16)(1000 - 1), (u16)(90 - 1));  // 定时器7初始化，周期1ms
    // HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);      // 开启定时器1通道1的PWM输出
    // HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);      // 开启定时器1通道4的PWM输出
    // positional_pid_init(&motor1_velocity, 0.15, 0.0285, 0.0, 1250, 0, 1250, -1250);
    // positional_pid_init(&motor2_velocity, 0.15, 0.0285, 0.0, 1250, 0, 1250, -1250);
    // motor1_velocity.control = DISABLE;
    // motor2_velocity.control = DISABLE;
    // red_init();
    // Kalman_Init();

    while (1)
    {
        // static u32 sec = 0;

        // sprintf(temp, "E1:%d,E2:%d,key:%d ", RED_XY[0], RED_XY[1], key_val);
        // OLED_ShowString(0, 0, temp, 8, 1, WHITE);
        // sprintf(temp, "TASK:%d,TEMP:%d,DO:%d ", TASK, TASK_TEMP, Do_count);
        // OLED_ShowString(0, 8, temp, 8, 1, WHITE);
        // sprintf(temp, "0:%d,0:%d ", HEIKUANG[0], HEIKUANG[1]);
        // OLED_ShowString(0, 16, temp, 8, 1, WHITE);
        // sprintf(temp, "1:%d,1:%d ", HEIKUANG[2], HEIKUANG[3]);
        // OLED_ShowString(0, 24, temp, 8, 1, WHITE);
        // sprintf(temp, "2:%d,2:%d ", HEIKUANG[4], HEIKUANG[5]);
        // OLED_ShowString(0, 32, temp, 8, 1, WHITE);
        // sprintf(temp, "3:%d,3:%d ", HEIKUANG[6], HEIKUANG[7]);
        // OLED_ShowString(0, 40, temp, 8, 1, WHITE);

        // // 上位机调整参数
        // motor1_velocity.kp = (float)parListForTest[0] / 100;
        // motor1_velocity.ki = (float)parListForTest[1] / 100;
        // motor1_velocity.kd = (float)parListForTest[2] / 100;
        // motor2_velocity.kp = (float)parListForTest[3] / 100;
        // motor2_velocity.ki = (float)parListForTest[4] / 100;
        // motor2_velocity.kd = (float)parListForTest[5] / 100;
        // 上位机上传数据处理
        // databuf[0] = BYTE0(RED_XY[0]);
        // databuf[1] = BYTE1(RED_XY[0]);
        // databuf[2] = BYTE2(RED_XY[0]);
        // databuf[3] = BYTE3(RED_XY[0]);
        // databuf[4] = BYTE0(RED_XY[1]);
        // databuf[5] = BYTE1(RED_XY[1]);
        // databuf[6] = BYTE2(RED_XY[1]);
        // databuf[7] = BYTE3(RED_XY[1]);
        // databuf[8] = BYTE0(TARGET_RED_XY[0]);
        // databuf[9] = BYTE1(TARGET_RED_XY[0]);
        // databuf[10] = BYTE2(TARGET_RED_XY[0]);
        // databuf[11] = BYTE3(TARGET_RED_XY[0]);
        // databuf[12] = BYTE0(TARGET_RED_XY[1]);
        // databuf[13] = BYTE1(TARGET_RED_XY[1]);
        // databuf[14] = BYTE2(TARGET_RED_XY[1]);
        // databuf[15] = BYTE3(TARGET_RED_XY[1]);

        // M1M2RESET();
        // MAIN_TASK();
    }
}