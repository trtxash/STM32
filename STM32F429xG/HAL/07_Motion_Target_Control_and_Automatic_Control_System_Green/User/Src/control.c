#include "control.h"

#define XYD 5
#define MINXY 10
#define MAXXY 290
#define MIDXY 150
#define KAIDELAY 10
#define BIHEIXIADUAN_S_NUM 4
#define BIHEIXIADUAN_L_NUM 6

positional_pid_params_t motor1_velocity;
positional_pid_params_t motor2_velocity;

u8 TASK = 14, TASK_TEMP = 0, Do_count = 0;
u8 finreset = 0;
u16 PWMA = 0, PWMB = 0;
u32 RED_XY[2] = {0}, RED_XY_TEMP[2] = {0}, GREEN_XY[2] = {0}, RED_XY_OLD[2] = {0};
u32 TARGET_RED_XY[2] = {150, 150}, TARGET_GREEN_XY[2] = {150, 150};
u16 XSET = 1484, YSET = 2121, STOP[2] = {0};
u16 HEIKUANG_TEMP[16] = {0};
u16 HEIKUANG[8] = {0};
u16 HEIKUANG_LOCK[8] = {0};
u16 HEIXIANDUAN_XY[2 * BIHEIXIADUAN_S_NUM + 2 * BIHEIXIADUAN_L_NUM][2] = {0}; // 点的坐标
float K0 = 0, B0 = 0, K1 = 0, B1 = 0, K2 = 0, B2 = 0, K3 = 0, B3 = 0;

void Set_angle(u16 pwm1, u16 pwm2)
{
    if (pwm1 < 500 - 1)
        pwm1 = 500;
    if (pwm1 > 2500 - 1)
        pwm1 = 2500;
    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, pwm1); // PWMA

    if (pwm2 < 500 - 1)
        pwm2 = 500;
    if (pwm2 > 2500 - 1)
        pwm2 = 2500;
    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, pwm2); // PWMA

    // if (pwm1 > 1000 - 1)
    //     pwm1 = 1000;
    // if (pwm2 > 1000 - 1)
    //     pwm2 = 1000;
    // LobotSerialServoMove(0, PWMA, 500);
    // LobotSerialServoMove(1, PWMB, 0);
}

// 从串口得到xy,返回0成功
u8 get_pi_xy(void)
{
    if (USART_RX_STA_C & 0x8000) // 接受到数字
    {
        u8 i = 0;
        i = USART_RX_STA_C & 0X3FFF; // 接收次数

        if (USART_RX_BUF_C[0] == '*' & USART_RX_BUF_C[i - 1] == '#') // 检验包头包尾
        {
            sscanf(USART_RX_BUF_C, "*%d %d,%d %d#", &RED_XY[0], &RED_XY[1], &GREEN_XY[0], &GREEN_XY[1]);

            // sscanf(USART_RX_BUF_C, "*%d %d,%d %d %d %d %d %d %d %d,%d %d %d %d %d %d %d %d#", &RED_XY[0], &RED_XY[1], &HEIKUANG_TEMP[0], &HEIKUANG_TEMP[1], &HEIKUANG_TEMP[2], &HEIKUANG_TEMP[3], &HEIKUANG_TEMP[4], &HEIKUANG_TEMP[5], &HEIKUANG_TEMP[6], &HEIKUANG_TEMP[7], &HEIKUANG_TEMP[8], &HEIKUANG_TEMP[9], &HEIKUANG_TEMP[10], &HEIKUANG_TEMP[11], &HEIKUANG_TEMP[12], &HEIKUANG_TEMP[13], &HEIKUANG_TEMP[14], &HEIKUANG_TEMP[15]);
            // u8 x = 0;
            // // 融合
            // for (u8 j = 1; j < 8; j++) // 找最近点
            // {
            //     if ((HEIKUANG_TEMP[0] - HEIKUANG_TEMP[2 * j]) * (HEIKUANG_TEMP[0] - HEIKUANG_TEMP[2 * j]) + (HEIKUANG_TEMP[1] - HEIKUANG_TEMP[2 * j + 1]) * (HEIKUANG_TEMP[1] - HEIKUANG_TEMP[2 * j + 1]) < 200) // 小于一个值为同一个点
            //         x = j;                                                                                                                                                                                       // 记录位置
            // }
            // HEIKUANG[0] = (HEIKUANG_TEMP[0] + HEIKUANG_TEMP[2 * x]) / 2; // 赋值
            // HEIKUANG[1] = (HEIKUANG_TEMP[1] + HEIKUANG_TEMP[2 * x + 1]) / 2;
            // HEIKUANG_TEMP[2 * x] = HEIKUANG_TEMP[2]; // 将第2个值赋到找到值位置
            // HEIKUANG_TEMP[2 * x + 1] = HEIKUANG_TEMP[3];
            // for (u8 j = 3; j < 8; j++) // 找最近点
            // {
            //     if ((HEIKUANG_TEMP[4] - HEIKUANG_TEMP[2 * j]) * (HEIKUANG_TEMP[4] - HEIKUANG_TEMP[2 * j]) + (HEIKUANG_TEMP[5] - HEIKUANG_TEMP[2 * j + 1]) * (HEIKUANG_TEMP[5] - HEIKUANG_TEMP[2 * j + 1]) < 200) // 小于一个值为同一个点
            //         x = j;                                                                                                                                                                                       // 记录位置
            // }
            // HEIKUANG[2] = (HEIKUANG_TEMP[4] + HEIKUANG_TEMP[2 * x]) / 2; // 赋值
            // HEIKUANG[3] = (HEIKUANG_TEMP[5] + HEIKUANG_TEMP[2 * x + 1]) / 2;
            // HEIKUANG_TEMP[2 * x] = HEIKUANG_TEMP[6]; // 将第4个值赋到找到值位置
            // HEIKUANG_TEMP[2 * x + 1] = HEIKUANG_TEMP[7];
            // for (u8 j = 5; j < 8; j++) // 找最近点
            // {
            //     if ((HEIKUANG_TEMP[8] - HEIKUANG_TEMP[2 * j]) * (HEIKUANG_TEMP[8] - HEIKUANG_TEMP[2 * j]) + (HEIKUANG_TEMP[9] - HEIKUANG_TEMP[2 * j + 1]) * (HEIKUANG_TEMP[9] - HEIKUANG_TEMP[2 * j + 1]) < 200) // 小于一个值为同一个点
            //         x = j;                                                                                                                                                                                       // 记录位置
            // }
            // HEIKUANG[4] = (HEIKUANG_TEMP[8] + HEIKUANG_TEMP[2 * x]) / 2; // 赋值
            // HEIKUANG[5] = (HEIKUANG_TEMP[9] + HEIKUANG_TEMP[2 * x + 1]) / 2;
            // HEIKUANG_TEMP[2 * x] = HEIKUANG_TEMP[10]; // 将第6个值赋到找到值位置
            // HEIKUANG_TEMP[2 * x + 1] = HEIKUANG_TEMP[11];
            // HEIKUANG[6] = (HEIKUANG_TEMP[12] + HEIKUANG_TEMP[14]) / 2; // 最后一个点
            // HEIKUANG[7] = (HEIKUANG_TEMP[13] + HEIKUANG_TEMP[15]) / 2;

            USART_RX_STA_C = 0; // 清楚标志位
            return 0;           // 接收成功
        }
    }
    USART_RX_STA_C = 0; // 清楚标志位
    return 1;           // 接收失败
}

// 主要任务
void MAIN_TASK(void)
{
    if (TASK == 0) // 闭环复位
    {
        switch (Do_count)
        {
        case 0: // 复位
            control_red(1);
            motor1_velocity.control = DISABLE;
            motor2_velocity.control = DISABLE;
            if (redJabsl(150, 150, 50))
                Do_count++;
            else if (redJabsl(150, 150, 150))
            {
                TARGET_RED_XY[0] = 150;
                TARGET_RED_XY[1] = 150;
            }
            break;
        case 1:
            motor1_velocity.control = ENABLE;
            motor2_velocity.control = ENABLE;
            Do_count++;
            break;
        case 2:
            if (redJabsl(150, 150, 5) & time40sec(5))
            {
                motor1_velocity.control = DISABLE;
                motor2_velocity.control = DISABLE;
                finreset = 1;
                Do_count++;
            }
            break;
        default:
            break;
        }
    }
    else if (TASK == 1) // 开环复位
    {
        switch (Do_count)
        {
        case 0:
            control_red(1);
            motor1_velocity.control = DISABLE;
            motor2_velocity.control = DISABLE;
            Set_angle(XSET + 50, YSET + 50);
            if (time40sec(20))
                Do_count++;
            break;
        case 1:
            Set_angle(XSET, YSET);
            break;
        default:
            break;
        }
    }
    else if (TASK == 2) // 闭环描屏幕
    {
        switch (Do_count)
        {
        case 0:
            control_red(1);
            TARGET_RED_XY[0] = MIDXY;
            TARGET_RED_XY[1] = (MIDXY + MINXY) / 2;
            motor1_velocity.control = ENABLE;
            motor2_velocity.control = ENABLE;
            if (redJabsl(TARGET_RED_XY[0], TARGET_RED_XY[1], XYD))
                Do_count++;
            break;
        case 1:
            TARGET_RED_XY[0] = MIDXY;
            TARGET_RED_XY[1] = MINXY;
            if (redJabsl(TARGET_RED_XY[0], TARGET_RED_XY[1], XYD))
                Do_count++;
            break;
        case 2:
            TARGET_RED_XY[0] = (MIDXY + MAXXY) / 2;
            TARGET_RED_XY[1] = MINXY;
            if (redJabsl(TARGET_RED_XY[0], TARGET_RED_XY[1], XYD))
                Do_count++;
            break;
        case 3:
            TARGET_RED_XY[0] = MAXXY;
            TARGET_RED_XY[1] = MINXY;
            if (redJabsl(TARGET_RED_XY[0], TARGET_RED_XY[1], XYD))
                Do_count++;
            break;
        case 4:
            TARGET_RED_XY[0] = MAXXY;
            TARGET_RED_XY[1] = (MINXY + MAXXY) / 2;
            if (redJabsl(TARGET_RED_XY[0], TARGET_RED_XY[1], XYD))
                Do_count++;
            break;
        case 5:
            TARGET_RED_XY[0] = MAXXY;
            TARGET_RED_XY[1] = MIDXY;
            if (redJabsl(TARGET_RED_XY[0], TARGET_RED_XY[1], XYD))
                Do_count++;
            break;
        case 6:
            TARGET_RED_XY[0] = MAXXY;
            TARGET_RED_XY[1] = (MIDXY + MAXXY) / 2;
            if (redJabsl(TARGET_RED_XY[0], TARGET_RED_XY[1], XYD))
                Do_count++;
            break;
        case 7:
            TARGET_RED_XY[0] = MAXXY;
            TARGET_RED_XY[1] = MAXXY;
            if (redJabsl(TARGET_RED_XY[0], TARGET_RED_XY[1], XYD))
                Do_count++;
            break;
        case 8:
            TARGET_RED_XY[0] = (MIDXY + MAXXY) / 2;
            TARGET_RED_XY[1] = MAXXY;
            if (redJabsl(TARGET_RED_XY[0], TARGET_RED_XY[1], XYD))
                Do_count++;
            break;
        case 9:
            TARGET_RED_XY[0] = MIDXY;
            TARGET_RED_XY[1] = MAXXY;
            if (redJabsl(TARGET_RED_XY[0], TARGET_RED_XY[1], XYD))
                Do_count++;
            break;
        case 10:
            TARGET_RED_XY[0] = (MIDXY + MINXY) / 2;
            TARGET_RED_XY[1] = MAXXY;
            if (redJabsl(TARGET_RED_XY[0], TARGET_RED_XY[1], XYD))
                Do_count++;
            break;
        case 11:
            TARGET_RED_XY[0] = MINXY;
            TARGET_RED_XY[1] = MAXXY;
            if (redJabsl(TARGET_RED_XY[0], TARGET_RED_XY[1], XYD))
                Do_count++;
            break;
        case 12:
            TARGET_RED_XY[0] = MINXY;
            TARGET_RED_XY[1] = (MAXXY + MIDXY) / 2;
            if (redJabsl(TARGET_RED_XY[0], TARGET_RED_XY[1], XYD))
                Do_count++;
            break;
        case 13:
            TARGET_RED_XY[0] = MINXY;
            TARGET_RED_XY[1] = MIDXY;
            if (redJabsl(TARGET_RED_XY[0], TARGET_RED_XY[1], XYD))
                Do_count++;
            break;
        case 14:
            TARGET_RED_XY[0] = MINXY;
            TARGET_RED_XY[1] = (MIDXY + MINXY) / 2;
            if (redJabsl(TARGET_RED_XY[0], TARGET_RED_XY[1], XYD))
                Do_count++;
            break;
        case 15:
            TARGET_RED_XY[0] = MINXY;
            TARGET_RED_XY[1] = MINXY;
            if (redJabsl(TARGET_RED_XY[0], TARGET_RED_XY[1], XYD))
                Do_count++;
            break;
        case 16:
            TARGET_RED_XY[0] = (MINXY + MIDXY) / 2;
            TARGET_RED_XY[1] = MINXY;
            if (redJabsl(TARGET_RED_XY[0], TARGET_RED_XY[1], XYD))
                Do_count = 1;
            break;
        default:
            break;
        }
    }
    else if (TASK == 3) // 开环描屏幕
    {
        switch (Do_count)
        {
        case 0: // 右上
            Set_angle(XSET - 154, YSET + 135);
            if (time40sec(KAIDELAY))
                Do_count++;
            break;
        case 1: // 右下
            Set_angle(XSET - 153, YSET - 144);
            if (time40sec(KAIDELAY))
                Do_count++;
            break;
        case 2: // 左下
            Set_angle(XSET + 157, YSET - 145);
            if (time40sec(KAIDELAY))
                Do_count++;
            break;
        case 3: // 左上
            Set_angle(XSET + 159, YSET + 130);
            if (time40sec(KAIDELAY))
                Do_count = 0;
            break;
        default:
            break;
        }
    }
    else if (TASK == 4) // 闭环正黑线
    {
        static u8 XY_ii = 0;
        switch (Do_count)
        {
        case 0: // 锁死坐标
            control_red(0);
            motor1_velocity.control = DISABLE;
            motor2_velocity.control = DISABLE;
            if (time40sec(25))
            {
                if (HEIKUANG[0] && HEIKUANG[1] && HEIKUANG[2] && HEIKUANG[3] && HEIKUANG[4] && HEIKUANG[5] && HEIKUANG[6] && HEIKUANG[7])
                {
                    for (u8 i = 0; i < 8; i++)
                    {
                        HEIKUANG_LOCK[i] = HEIKUANG[i];
                    }
                    control_red(1);
                    Do_count++;
                }
            }
            break;
        case 1: // 计算坐标
            K0 = (float)(HEIKUANG_LOCK[1] - HEIKUANG_LOCK[3]) / (float)(HEIKUANG_LOCK[0] - HEIKUANG_LOCK[2]);
            K1 = (float)(HEIKUANG_LOCK[3] - HEIKUANG_LOCK[5]) / (float)(HEIKUANG_LOCK[2] - HEIKUANG_LOCK[4]);
            K2 = (float)(HEIKUANG_LOCK[5] - HEIKUANG_LOCK[7]) / (float)(HEIKUANG_LOCK[4] - HEIKUANG_LOCK[6]);
            K3 = (float)(HEIKUANG_LOCK[7] - HEIKUANG_LOCK[1]) / (float)(HEIKUANG_LOCK[6] - HEIKUANG_LOCK[0]);
            B0 = (float)(HEIKUANG_LOCK[1]) - K0 * (float)(HEIKUANG_LOCK[0]);
            B1 = (float)(HEIKUANG_LOCK[3]) - K1 * (float)(HEIKUANG_LOCK[2]);
            B2 = (float)(HEIKUANG_LOCK[5]) - K2 * (float)(HEIKUANG_LOCK[4]);
            B3 = (float)(HEIKUANG_LOCK[7]) - K3 * (float)(HEIKUANG_LOCK[6]);

            short L = 0; // 两者x差
            L = (HEIKUANG_LOCK[2] - HEIKUANG_LOCK[0]) / BIHEIXIADUAN_S_NUM;
            for (u16 i = 0; i < BIHEIXIADUAN_S_NUM; i++)
            {
                HEIXIANDUAN_XY[i][0] = HEIKUANG_LOCK[0] + L * i;       // x为x+n*差
                HEIXIANDUAN_XY[i][1] = K0 * HEIXIANDUAN_XY[i][0] + B0; // y套方程
            }
            L = (HEIKUANG_LOCK[4] - HEIKUANG_LOCK[2]) / BIHEIXIADUAN_L_NUM;
            for (u16 i = 0; i < BIHEIXIADUAN_L_NUM; i++)
            {
                HEIXIANDUAN_XY[i + BIHEIXIADUAN_S_NUM][0] = HEIKUANG_LOCK[2] + L * i;                            // x为x+n*差
                HEIXIANDUAN_XY[i + BIHEIXIADUAN_S_NUM][1] = K1 * HEIXIANDUAN_XY[i + BIHEIXIADUAN_S_NUM][0] + B1; // y套方程
            }
            L = (HEIKUANG_LOCK[6] - HEIKUANG_LOCK[4]) / BIHEIXIADUAN_S_NUM;
            for (u16 i = 0; i < BIHEIXIADUAN_S_NUM; i++)
            {
                HEIXIANDUAN_XY[i + BIHEIXIADUAN_S_NUM + BIHEIXIADUAN_L_NUM][0] = HEIKUANG_LOCK[4] + L * i;                                                 // x为x+n*差
                HEIXIANDUAN_XY[i + BIHEIXIADUAN_S_NUM + BIHEIXIADUAN_L_NUM][1] = K2 * HEIXIANDUAN_XY[i + BIHEIXIADUAN_S_NUM + BIHEIXIADUAN_L_NUM][0] + B2; // y套方程
            }
            L = (HEIKUANG_LOCK[0] - HEIKUANG_LOCK[6]) / BIHEIXIADUAN_L_NUM;
            for (u16 i = 0; i < BIHEIXIADUAN_L_NUM; i++)
            {
                HEIXIANDUAN_XY[i + 2 * BIHEIXIADUAN_S_NUM + BIHEIXIADUAN_L_NUM][0] = HEIKUANG_LOCK[6] + L * i;                                                     // x为x+n*差
                HEIXIANDUAN_XY[i + 2 * BIHEIXIADUAN_S_NUM + BIHEIXIADUAN_L_NUM][1] = K3 * HEIXIANDUAN_XY[i + 2 * BIHEIXIADUAN_S_NUM + BIHEIXIADUAN_L_NUM][0] + B3; // y套方程
            }
            Do_count++;
            break;
        case 2:
            TARGET_RED_XY[0] = HEIXIANDUAN_XY[0][0];
            TARGET_RED_XY[1] = HEIXIANDUAN_XY[0][1];
            motor1_velocity.control = ENABLE;
            motor2_velocity.control = ENABLE;
            Do_count++;
            break;
        case 3:
            TARGET_RED_XY[0] = HEIXIANDUAN_XY[XY_ii][0];
            TARGET_RED_XY[1] = HEIXIANDUAN_XY[XY_ii][1];
            if (redJabsl(TARGET_RED_XY[0], TARGET_RED_XY[1], XYD))
            {
                XY_ii++;
                if (XY_ii >= 2 * BIHEIXIADUAN_S_NUM + 2 * BIHEIXIADUAN_L_NUM)
                    XY_ii = 0;
            }
            break;
        default:
            break;
        }
    }
    else if (TASK == 12) // 黑线跟踪
    {
        switch (Do_count)
        {
        case 0: // 复位
            control_red(0);
            if (redgreenJabsl(RED_XY[0], RED_XY[1], GREEN_XY[0], GREEN_XY[1], 600))
            {
                positional_pid_set_value(&motor1_velocity, 0.1, 0.03, 0.0);
                positional_pid_set_value(&motor2_velocity, 0.1, 0.03, 0.0);
                Do_count++;
            }
            else
            {
                positional_pid_set_value(&motor1_velocity, 0.5, 0.08, 0.0);
                positional_pid_set_value(&motor2_velocity, 0.5, 0.08, 0.0);
            }
            break;
        case 1:
            if (RED_XY[0] != 0 & RED_XY[1] != 0 & GREEN_XY[0] != 0 & GREEN_XY[1] != 0)
            {
                Do_count++;
            }
            break;
        case 2:
            control_red(1);
            Do_count++;
            break;
        default:
            break;
        }
    }
    else if (TASK == 13) // 急停
    {
        if (RED_XY[0] == 0 & RED_XY[1] == 0 & GREEN_XY[0] == 0 & GREEN_XY[1] == 0)
            control_red(1);
        else
        {
            if (redgreenJabsl(RED_XY[0], RED_XY[1], GREEN_XY[0], GREEN_XY[1], 800)) // 关闭红色不叫
                control_red(1);
            else
                control_red(0);
        }
    }
    else if (TASK == 14) // 跟踪
    {
        switch (Do_count)
        {
        case 0: // 复位
            control_red(0);
            motor1_velocity.control = DISABLE;
            motor2_velocity.control = DISABLE;
            Set_angle(XSET - 20, YSET + 75);
            if (time40sec(1))
                Do_count++;
        case 1:
            motor1_velocity.control = ENABLE;
            motor2_velocity.control = ENABLE;
            Do_count++;
            break;
        case 2:
            if (redgreenJabsl(RED_XY[0], RED_XY[1], GREEN_XY[0], GREEN_XY[1], 600))
            {
                if (RED_XY[0] != 0 & RED_XY[1] != 0 & GREEN_XY[0] != 0 & GREEN_XY[1] != 0)
                {
                    Do_count++;
                }
            }
            break;
        case 3:
            if (RED_XY[0] == 0 | RED_XY[1] == 0 | GREEN_XY[0] == 0 | GREEN_XY[1] == 0)
            {
                control_red(1);
                finreset = 1;
                TASK = 13;
            }
            break;
        default:
            break;
        }
    }
    else if (TASK == 15) // 上位机
    {
        // 上位机调整参数
        motor1_velocity.kp = (float)parListForTest[0] / 100;
        motor1_velocity.ki = (float)parListForTest[1] / 100;
        motor1_velocity.kd = (float)parListForTest[2] / 100;
        motor2_velocity.kp = (float)parListForTest[3] / 100;
        motor2_velocity.ki = (float)parListForTest[4] / 100;
        motor2_velocity.kd = (float)parListForTest[5] / 100;
        // TARGET_RED_XY[0] = parListForTest[6];
        // TARGET_RED_XY[1] = parListForTest[7];
        // XSET = parListForTest[8];
        // YSET = parListForTest[9];
        // TASK = parListForTest[10];}
    }
}

// 判断red离点距离，成功返回1
u8 redJabsl(u16 x, u16 y, u16 l)
{
    return (abs(RED_XY[0] - x) < l & abs(RED_XY[1] - y) < l);
}

// 判断red离点距离，成功返回1
u8 redgreenJabsl(u16 x0, u16 y0, u16 x1, u16 y1, u16 l)
{
    u16 temp = (x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1);
    if (temp <= l)
        return 1;
    else
        return 0;
}

// 复位切换
void M1M2RESET(void)
{
    static u8 keychangeflag = 0;

    if (KEY0_READ())
    {
        if (TASK == 12)
        {
            TASK = 13;
            Do_count = 4;
        }
        else
        {
            TASK = key_val = 12;
            Do_count = 0;
            positional_pid_set_value(&motor1_velocity, 0.1, 0.03, 0.0);
            positional_pid_set_value(&motor2_velocity, 0.1, 0.03, 0.0);
        }
        delay_ms(100);
    }
    // if (KEY_4x4_8_READ() == 0)
    // {
    //     TASK = 13;
    //     Do_count = 4;
    // }

    // if (KEY0_READ() & TASK != 13)
    // {
    //     TASK = key_val = 13;
    //     Do_count = 0;
    // }

    // if (read_key_val())
    // {
    //     TASK = key_val;
    //     finreset = 0;
    //     Do_count = 0;
    // }
}

// 不堵塞延时sec*40ms,0失败，1成功
u8 time40sec(u8 sec)
{
    static u32 timesec = 0;
    if (time40flag)
    {
        time40flag = 0;
        timesec++;
        if (timesec > sec)
        {
            timesec = 0;
            return 1;
        }
    }
    return 0;
}

void red_init(void)
{
    GPIO_InitTypeDef GPIO_Initure = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_Initure.Pin = GPIO_PIN_4;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);
}

void control_red(u8 kaiguan)
{
    if (kaiguan)
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
    }
    else
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
}