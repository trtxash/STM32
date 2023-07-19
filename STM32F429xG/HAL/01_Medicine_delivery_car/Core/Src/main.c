/**
 * @file	01_Medicine_delivery_car
 * @brief
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2023年7月9号13点22分
 */

#include "main.h"

short vr = 0, vl = 0;
float v_p = 1.0;
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
    uart_init(115200);
    initValuePack(115200);
    while (1)
    {
        if (!mpu_dmp_init())
            break;
    }
    Tim_Encoder_Init();
    TB6612_init();
    MX_TIM6_Init((u16)(10000 - 1), (u16)(120 - 1)); // 定时器6初始化，周期10ms
    KEY0_Init();
    positional_pid_init(&motor1_velocity, 0.16, 0.14, 0, 7199, 0, 7199, -7199);
    positional_pid_init(&motor2_velocity, 0.16, 0.14, 0, 7199, 0, 7199, -7199);
    positional_pid_init(&motor12_location, 10, 0.0, 0, 7199, 0, 100, -100);
    positional_pid_init(&motor_turn, 40.0, 0.005, 0.0, 7199, 0, 7199, -7199);

    motor1_velocity.control = PID_ENABLE;
    motor2_velocity.control = PID_ENABLE;
    motor12_location.control = PID_ENABLE;
    motor_turn.control = PID_DISABLE; // 转向环开启
    while (1)
    {
        GET_NUM();
        Get_Angle(1);        // 读取角度
        Get_Grayscale_Val(); // 读取灰度

        // 100ms的周期任务
        if (time_flag)
        {
            time_flag = 0;
            // PID设置
            // positional_pid_set_value(&motor1_velocity, (float)rxvaluepack.shorts[0] / 100, (float)rxvaluepack.shorts[1] / 100, (float)rxvaluepack.shorts[2] / 100);
            // positional_pid_set_value(&motor2_velocity, (float)rxvaluepack.shorts[3] / 100, (float)rxvaluepack.shorts[4] / 100, (float)rxvaluepack.shorts[5] / 100);
            // positional_pid_set_value(&motor12_location, (float)rxvaluepack.shorts[6] / 100, (float)rxvaluepack.shorts[7] / 100, (float)rxvaluepack.shorts[8] / 100);
            // positional_pid_set_value(&motor_turn, (float)rxvaluepack.shorts[9] / 100, (float)rxvaluepack.shorts[10] / 100, (float)rxvaluepack.shorts[11] / 100);
            // OLED显示
            sprintf(temp, "R:%0.1f,P:%0.1f,Y:%0.1f ", Roll, Pitch, Yaw);
            OLED_ShowString(0, 0, temp, 8, 1, WHITE);

            sprintf(temp, "e1:%d,e2:%d ", Encoder[0], Encoder[1]);
            OLED_ShowString(0, 8, temp, 8, 1, WHITE);
            sprintf(temp, "l:%d ", Location_sum);
            OLED_ShowString(0, 16, temp, 8, 1, WHITE);
            sprintf(temp, "ERR:%d ", Error_sum);
            OLED_ShowString(64, 16, temp, 8, 1, WHITE);

            sprintf(temp, "G:%d%d%d%d%d,S:%d ", Grayscale_Val[0], Grayscale_Val[1], Grayscale_Val[2], Grayscale_Val[3], Grayscale_Val[4], Grayscale_truesum);
            OLED_ShowString(0, 24, temp, 8, 1, WHITE);

            sprintf(temp, "LF:%d,DC:%d,TA:%d ", LOAD_FLAG, Do_count, TASK);
            OLED_ShowString(0, 32, temp, 8, 1, WHITE);

            sprintf(temp, "S:%d%d%d%d,AS:%d,SF:%d,P:%c ", SUM[0], SUM[1], SUM[2], SUM[3], AIM_SUM, GET_ROOM_FLAG, AIM_PLACE);
            OLED_ShowString(0, 40, temp, 8, 1, WHITE);
        }

        MIAN_TASK();
    }
}