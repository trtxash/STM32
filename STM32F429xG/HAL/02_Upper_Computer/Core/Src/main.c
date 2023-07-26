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
    uart6_init(1384200);
    while (1)
    {
        if (!mpu_dmp_init())
            break;
    }
    gw_grayscale_sensor_init();
    Tim_Encoder_Init();
    TB6612_init();
    MX_TIM6_Init((u16)(50000 - 1), (u16)(120 - 1)); // 定时器6初始化，周期50ms
    MX_TIM7_Init((u16)(1000 - 1), (u16)(120 - 1));  // 定时器7初始化，周期1ms
    KEY0_Init();
    positional_pid_init(&motor1_velocity, 0.35, 0.21, 0.04, 7199, 0, 7199, -7199);
    positional_pid_init(&motor2_velocity, 0.35, 0.21, 0.04, 7199, 0, 7199, -7199);
    positional_pid_init(&motor12_location, 30, 0.0, 0, 7199, 0, 100, -100);
    positional_pid_init(&motor_turn, 120.0, 0.0, 0.0, 7199, 0, 7199, -7199);
    positional_pid_init(&xunxian, 400.0, 0.0, 0.0, 7199, 0, 7199, -7199);

    motor1_velocity.control = PID_ENABLE;
    motor2_velocity.control = PID_ENABLE;
    motor12_location.control = PID_ENABLE;
    motor_turn.control = PID_DISABLE;
    xunxian.control = PID_ENABLE;
    while (1)
    {
        GET_NUM();
        Get_Angle(1);           // 读取角度
        Get_GW_Grayscale_Val(); // 读取灰度
        // Get_Grayscale_Val(); // 读取灰度

        // 1ms的周期任务
        if (time_flag)
        {
            time_flag = 0;

            // OLED显示
            sprintf(temp, "R:%0.1f,P:%0.1f,Y:%0.1f ", Roll, Pitch, Yaw);
            OLED_ShowString(0, 0, temp, 8, 1, WHITE);

            sprintf(temp, "e1:%d,e2:%d ", Encoder[0], Encoder[1]);
            OLED_ShowString(0, 8, temp, 8, 1, WHITE);
            sprintf(temp, "l:%d ", Location_sum);
            OLED_ShowString(0, 16, temp, 8, 1, WHITE);
            sprintf(temp, "ERR:%d ", Error_sum);
            OLED_ShowString(64, 16, temp, 8, 1, WHITE);

            // sprintf(temp, "G:%d%d%d%d%d,S:%d ", Grayscale_Val[0], Grayscale_Val[1], Grayscale_Val[2], Grayscale_Val[3], Grayscale_Val[4], Grayscale_truesum);
            // OLED_ShowString(0, 24, temp, 8, 1, WHITE);
            sprintf(temp, "G:%d%d%d%d%d%d%d%d,S:%0.2f ", gray_sensor[0], gray_sensor[1], gray_sensor[2], gray_sensor[3], gray_sensor[4], gray_sensor[5], gray_sensor[6], gray_sensor[7], gray_sensor_sum_val);
            OLED_ShowString(0, 24, temp, 8, 1, WHITE);

            sprintf(temp, "LF:%d,DC:%d,TA:%d ", LOAD_FLAG, Do_count, TASK);
            OLED_ShowString(0, 32, temp, 8, 1, WHITE);

            sprintf(temp, "S:%d%d%d%d,AS:%d,SF:%d,P:%c ", SUM[0], SUM[1], SUM[2], SUM[3], AIM_SUM, GET_ROOM_FLAG, AIM_PLACE);
            OLED_ShowString(0, 40, temp, 8, 1, WHITE);

            // 上位机调整参数
            motor1_velocity.kp = (float)parListForTest[0] / 100;
            motor1_velocity.ki = (float)parListForTest[1] / 100;
            motor1_velocity.kd = (float)parListForTest[2] / 100;
            motor2_velocity.kp = (float)parListForTest[3] / 100;
            motor2_velocity.ki = (float)parListForTest[4] / 100;
            motor2_velocity.kd = (float)parListForTest[5] / 100;
            motor12_location.kp = (float)parListForTest[6] / 100;
            motor12_location.ki = (float)parListForTest[7] / 100;
            motor12_location.kd = (float)parListForTest[8] / 100;
            motor_turn.kp = (float)parListForTest[9] / 100;
            motor_turn.ki = (float)parListForTest[10] / 100;
            motor_turn.kd = (float)parListForTest[11] / 100;
            motor1_velocity.control = parListForTest[12];
            motor2_velocity.control = parListForTest[13];
            motor12_location.control = parListForTest[14];
            motor_turn.control = parListForTest[15];
            TARGET_V = parListForTest[16];
            TARGET_LOCATION = parListForTest[17];
            TARGET_ANGLE = (float)parListForTest[18] / 100;
            xunxian.kp = (float)parListForTest[19] / 100;
            xunxian.ki = (float)parListForTest[20] / 100;
            xunxian.kd = (float)parListForTest[21] / 100;
            // 上位机上传数据处理
            databuf[0] = BYTE0(Encoder[0]);
            databuf[1] = BYTE1(Encoder[0]);
            databuf[2] = BYTE0(Encoder[1]);
            databuf[3] = BYTE1(Encoder[1]);
            databuf[4] = BYTE0(TARGET_V);
            databuf[5] = BYTE1(TARGET_V);
            databuf[6] = BYTE0(Location_sum);
            databuf[7] = BYTE1(Location_sum);
            databuf[8] = BYTE2(Location_sum);
            databuf[9] = BYTE3(Location_sum);
            databuf[10] = BYTE0(TARGET_LOCATION);
            databuf[11] = BYTE1(TARGET_LOCATION);
            databuf[12] = BYTE2(TARGET_LOCATION);
            databuf[13] = BYTE3(TARGET_LOCATION);
            databuf[14] = BYTE0(Yaw);
            databuf[15] = BYTE1(Yaw);
            databuf[16] = BYTE2(Yaw);
            databuf[17] = BYTE3(Yaw);
            databuf[18] = BYTE0(TARGET_ANGLE);
            databuf[19] = BYTE1(TARGET_ANGLE);
            databuf[20] = BYTE2(TARGET_ANGLE);
            databuf[21] = BYTE3(TARGET_ANGLE);
            databuf[22] = BYTE0(Grayscale_truesum_val);
            databuf[23] = BYTE1(Grayscale_truesum_val);
            databuf[24] = BYTE2(Grayscale_truesum_val);
            databuf[25] = BYTE3(Grayscale_truesum_val);
        }

        // MIAN_TASK();
    }
}