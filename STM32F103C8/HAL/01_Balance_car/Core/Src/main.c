/**
 * @file	01_Balance_car
 * @brief
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2023年6月25号18点33分
 */

#include "main.h"

positional_pid_params_t motor1_blance_pid;
// positional_pid_params_t motor1_blance_a_pid;
positional_pid_params_t motor1_velocity_pid;

float battery_v;      // 单电池电压
double battery_v_100; // 电池容量百分比

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
    Stm32_Clock_Init(RCC_PLL_MUL9); // 设置时钟,72M
    delay_init(72);                 // 初始化延时函数
    uart_init(115200);              // 初始化串口

    printf("\r\n初始化中...\r\n");

    MX_DMA_Init();  // 要先初始化DMA
    MX_ADC1_Init(); // 初始化ADC1
    OLED_Init();
    sprintf(temp, "Init...");
    OLED_ShowString(0, 0, temp, 8, 1, WHITE);
    OLED_Refresh();
    initValuePack(9600); // Valuepack初始化，用了uart3+DMA
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
    Tim_Encoder_Init();                                       // 编码器初始化，用了tim2，tim4
    TB6612_init();                                            // tb6612驱动芯片初始化
    EXTI_Init();                                              // 外部中断，由mpu6050 int脚触发，5ms周期
    HAL_ADCEx_Calibration_Start(&hadc1);                      // 启动ADC1校准，不校准将导致ADC测量不准确
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcx, ADC_Sec * 2); // 启动ADC+DMA
    positional_pid_init(&motor1_blance_pid, 0, 0, 0, 1000, 0, 3600, -3600);
    // positional_pid_init(&motor1_blance_a_pid, 0, 0, 0, 1000, 0, 3600, -3600);
    positional_pid_init(&motor1_velocity_pid, 0, 0, 0, 1000, 0, 3600, -3600);

    sprintf(temp, "Init ok!");
    OLED_ShowString(0, 0, temp, 8, 1, WHITE);
    printf("\r\n初始化完成...\r\n");

    while (1)
    {
        readValuePack(&rxvaluepack); // 读取串口3接收到的数据
        positional_pid_set_value(&motor1_blance_pid, rxvaluepack.shorts[0] / 10, rxvaluepack.shorts[1] / 10, rxvaluepack.shorts[2] / 10);
        // positional_pid_set_value(&motor1_blance_a_pid, 0, rxvaluepack.shorts[1] / 10, 0);
        positional_pid_set_value(&motor1_velocity_pid, rxvaluepack.shorts[3] / 10, rxvaluepack.shorts[4] / 10, rxvaluepack.shorts[5] / 10);
        // positional_pid_set_value(&motor1_blance_pid, rxvaluepack.shorts[0], rxvaluepack.shorts[1], rxvaluepack.shorts[2]);

        battery_v = Get_Adc_Average_DMA() * 3.3 * 11 * 100 / 1.0 / 4096 / 100 / 3;
        battery_v_100 = 9413.0184 - 303.204 * battery_v * battery_v * battery_v * battery_v * battery_v + 114.0062 * battery_v * battery_v * battery_v * battery_v * battery_v * battery_v * battery_v - 44.7181 * battery_v * battery_v * battery_v * battery_v * battery_v * battery_v * battery_v * battery_v + 6.547055 * battery_v * battery_v * battery_v * battery_v * battery_v * battery_v * battery_v * battery_v * battery_v - 0.336029617 * battery_v * battery_v * battery_v * battery_v * battery_v * battery_v * battery_v * battery_v * battery_v * battery_v;

        sprintf(temp, "v: %0.2f ", battery_v_100);
        OLED_ShowString(0, 8, temp, 8, 1, WHITE);

        sprintf(temp, "t: %0.2f ", (float)Temperature / 100);
        OLED_ShowString(64, 8, temp, 8, 1, WHITE);
        sprintf(temp, "R: %0.2f ", Roll);
        OLED_ShowString(0, 16, temp, 8, 1, WHITE);
        sprintf(temp, "P: %0.2f ", Pitch);
        OLED_ShowString(64, 16, temp, 8, 1, WHITE);
        sprintf(temp, "Y: %0.2f ", Yaw);
        OLED_ShowString(0, 24, temp, 8, 1, WHITE);

        sprintf(temp, "AB: %0.2f ", Angle_Balance);
        OLED_ShowString(0, 32, temp, 8, 1, WHITE);
        sprintf(temp, "GB: %0.2f ", Gyro_Balance);
        OLED_ShowString(64, 32, temp, 8, 1, WHITE);

        OLED_ShowNum(0, 40, Encoder_val[0], 5, 8, 1, WHITE);
        OLED_ShowNum(0, 48, Encoder_val[1], 5, 8, 1, WHITE);

        OLED_Refresh();
    }
}