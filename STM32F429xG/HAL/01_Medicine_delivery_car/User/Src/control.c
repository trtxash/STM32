#include "control.h"

positional_pid_params_t motor1_velocity;
positional_pid_params_t motor2_velocity;
positional_pid_params_t motor_turn;

u8 SUM[4][2] = {0};
short TARGET_V[2] = {0};
float TARGET_ANGLE = 0;
float Yaw_Start = 0;
u8 TASK = 0;
u8 FIND_ROOM_FLAG = 0;
u8 LOAD_FLAG = 0;
u8 AIM_PLACE = 0;
u8 Do_count = 0;
u8 STOP_FLAG = 0, LINE_FLAG = 0, TURN_SUCCEED_FLAG = 0;
u8 LOR = 0;

// 主要任务
void MIAN_TASK(void)
{
    if (TASK == 0) // 等待识别目标房间
    {
        SET_AIM_ROOM(); // 设置目标房间
        LOADORNOT();    // 检测药物
    }
    else if (TASK == 1)
    {
        if (LOAD_FLAG == 1) // 送药
        {
            if (AIM_PLACE == 'A')
            {
                switch (Do_count)
                {
                case 0: // 直走
                    Car_GO(100);
                    Do_count++;
                    break;
                case 1:
                    if (Location_sum > 65536)
                        TARGET_V[0] = TARGET_V[1] = 50;
                    if (GRAYSCALE0_READ() + GRAYSCALE1_READ() + GRAYSCALE2_READ() + GRAYSCALE3_READ() + GRAYSCALE4_READ() + GRAYSCALE5_READ() + GRAYSCALE6_READ() > 5)
                        STOP_FLAG = 1; // 亮了6个以上，说明经过了线
                    if (STOP_FLAG)
                        Do_count++;
                    break;
                case 2: // 转向
                    Car_TURN(-90);
                    Do_count++;
                    break;
                case 3:
                    if (J_turn(5)) // 转向角差5以内
                        TURN_SUCCEED_FLAG = 1;
                    if (TURN_SUCCEED_FLAG)
                        Do_count++;
                    break;
                case 4: // 直走
                    Car_GO(100);
                    Do_count++;
                    break;
                case 5:
                    if (Location_sum > 65536)
                        TARGET_V[0] = TARGET_V[1] = 50;
                    if (GRAYSCALE0_READ() + GRAYSCALE1_READ() + GRAYSCALE2_READ() + GRAYSCALE3_READ() + GRAYSCALE4_READ() + GRAYSCALE5_READ() + GRAYSCALE6_READ() > 2 |
                        GRAYSCALE0_READ() + GRAYSCALE1_READ() + GRAYSCALE2_READ() + GRAYSCALE3_READ() + GRAYSCALE4_READ() + GRAYSCALE5_READ() + GRAYSCALE6_READ() == 0)
                        STOP_FLAG = 1; // 终点
                    if (STOP_FLAG)
                        Do_count++;
                    break;
                case 6: // 卸药
                    Car_STOP();
                    LED0_Set();
                    Do_count++;
                    break;
                case 7:
                    LOADORNOT();
                    break;
                default:
                    break;
                }
            }
            else if (AIM_PLACE == 'B')
            {
                switch (Do_count)
                {
                case 0: // 直走
                    Car_GO(100);
                    Do_count++;
                    break;
                case 1:
                    if (Location_sum > 65536)
                        TARGET_V[0] = TARGET_V[1] = 50;
                    if (GRAYSCALE0_READ() + GRAYSCALE1_READ() + GRAYSCALE2_READ() + GRAYSCALE3_READ() + GRAYSCALE4_READ() + GRAYSCALE5_READ() + GRAYSCALE6_READ() > 5)
                        STOP_FLAG = 1; // 亮了6个以上，说明经过了线
                    if (STOP_FLAG)
                        Do_count++;
                    break;
                case 2: // 转向
                    Car_TURN(90);
                    Do_count++;
                    break;
                case 3:
                    if (J_turn(5)) // 转向角差5以内
                        TURN_SUCCEED_FLAG = 1;
                    if (TURN_SUCCEED_FLAG)
                        Do_count++;
                    break;
                case 4: // 直走
                    Car_GO(100);
                    Do_count++;
                    break;
                case 5:
                    if (Location_sum > 65536)
                        TARGET_V[0] = TARGET_V[1] = 50;
                    if (GRAYSCALE0_READ() + GRAYSCALE1_READ() + GRAYSCALE2_READ() + GRAYSCALE3_READ() + GRAYSCALE4_READ() + GRAYSCALE5_READ() + GRAYSCALE6_READ() > 2 |
                        GRAYSCALE0_READ() + GRAYSCALE1_READ() + GRAYSCALE2_READ() + GRAYSCALE3_READ() + GRAYSCALE4_READ() + GRAYSCALE5_READ() + GRAYSCALE6_READ() == 0)
                        STOP_FLAG = 1; // 终点
                    if (STOP_FLAG)
                        Do_count++;
                    break;
                case 6: // 卸药
                    Car_STOP();
                    LED0_Set();
                    Do_count++;
                    break;
                case 7:
                    LOADORNOT();
                    break;
                default:
                    break;
                }
            }
            else // 中端病房和远端病房需要发送特定标志位
            {
                switch (Do_count)
                {
                case 0:
                    break;
                case 1:
                    break;
                default:
                    break;
                }
            }
        }
        else if (LOAD_FLAG == 2) // 返回
        {
            if (AIM_PLACE == 'A')
            {
                switch (Do_count)
                {
                case 0: // 转向
                    Car_TURN(180);
                    LED0_Clr();
                    Do_count++;
                    break;
                case 1: // 直走
                    Car_GO(100);
                    Do_count++;
                    break;
                case 2:
                    if (Location_sum > 65536)
                        TARGET_V[0] = TARGET_V[1] = 50;
                    if (GRAYSCALE0_READ() + GRAYSCALE1_READ() + GRAYSCALE2_READ() + GRAYSCALE3_READ() + GRAYSCALE4_READ() + GRAYSCALE5_READ() + GRAYSCALE6_READ() > 5)
                        STOP_FLAG = 1; // 亮了6个以上，说明经过了线
                    if (STOP_FLAG)
                        Do_count++;
                    break;
                case 3: // 转向
                    Car_TURN(90);
                    Do_count++;
                    break;
                case 4:
                    if (J_turn(5)) // 转向角差5以内
                        TURN_SUCCEED_FLAG = 1;
                    if (TURN_SUCCEED_FLAG)
                        Do_count++;
                    break;
                case 5: // 直走
                    Car_GO(100);
                    Do_count++;
                    break;
                case 6:
                    if (Location_sum > 65536)
                        TARGET_V[0] = TARGET_V[1] = 50;
                    if (GRAYSCALE0_READ() + GRAYSCALE1_READ() + GRAYSCALE2_READ() + GRAYSCALE3_READ() + GRAYSCALE4_READ() + GRAYSCALE5_READ() + GRAYSCALE6_READ() > 2 |
                        GRAYSCALE0_READ() + GRAYSCALE1_READ() + GRAYSCALE2_READ() + GRAYSCALE3_READ() + GRAYSCALE4_READ() + GRAYSCALE5_READ() + GRAYSCALE6_READ() == 0)
                        STOP_FLAG = 1; // 终点
                    if (STOP_FLAG)
                        Do_count++;
                    break;
                case 7: // 停止
                    Car_STOP();
                    LED1_Set();
                    Do_count++;
                    break;
                case 8:
                    break;
                default:
                    break;
                }
            }
            else if (AIM_PLACE == 'B')
            {
                switch (Do_count)
                {
                case 0: // 转向
                    Car_TURN(180);
                    LED0_Clr();
                    Do_count++;
                    break;
                case 1: // 直走
                    Car_GO(100);
                    Do_count++;
                    break;
                case 2:
                    if (Location_sum > 65536)
                        TARGET_V[0] = TARGET_V[1] = 50;
                    if (GRAYSCALE0_READ() + GRAYSCALE1_READ() + GRAYSCALE2_READ() + GRAYSCALE3_READ() + GRAYSCALE4_READ() + GRAYSCALE5_READ() + GRAYSCALE6_READ() > 5)
                        STOP_FLAG = 1; // 亮了6个以上，说明经过了线
                    if (STOP_FLAG)
                        Do_count++;
                    break;
                case 3: // 转向
                    Car_TURN(-90);
                    Do_count++;
                    break;
                case 4:
                    if (J_turn(5)) // 转向角差5以内
                        TURN_SUCCEED_FLAG = 1;
                    if (TURN_SUCCEED_FLAG)
                        Do_count++;
                    break;
                case 5: // 直走
                    Car_GO(100);
                    Do_count++;
                    break;
                case 6:
                    if (Location_sum > 65536)
                        TARGET_V[0] = TARGET_V[1] = 50;
                    if (GRAYSCALE0_READ() + GRAYSCALE1_READ() + GRAYSCALE2_READ() + GRAYSCALE3_READ() + GRAYSCALE4_READ() + GRAYSCALE5_READ() + GRAYSCALE6_READ() > 2 |
                        GRAYSCALE0_READ() + GRAYSCALE1_READ() + GRAYSCALE2_READ() + GRAYSCALE3_READ() + GRAYSCALE4_READ() + GRAYSCALE5_READ() + GRAYSCALE6_READ() == 0)
                        STOP_FLAG = 1; // 终点
                    if (STOP_FLAG)
                        Do_count++;
                    break;
                case 7: // 停止
                    Car_STOP();
                    LED1_Set();
                    Do_count++;
                    break;
                case 8:
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void SET_AIM_ROOM(void)
{
    // 一开始识别目标房间
    if (FIND_ROOM_FLAG == 0) // 等待
    {
        GET_NUM();
    }
    else if (FIND_ROOM_FLAG == 1) // 识别开始房间
    {
        if (SUM[0][0] == 1) // 设置目标房间
        {
            AIM_PLACE = 'A';
        }
        else if (SUM[0][0] == 2)
        {
            AIM_PLACE = 'B';
        }
        else if (SUM[0][0] > 2)
        {
            AIM_PLACE = 'C';
            FIND_ROOM_FLAG = 0; // 在中端找
        }
        TASK = 1;
    }
}

// 检测药物
void LOADORNOT(void)
{
    if (KEY0_READ())
    {
        if (STOP_FLAG)
        {
            delay_ms(10); // 消抖
            if (KEY0_READ())
            {
                LOAD_FLAG = 2;
                Do_count = 0;
            }
            else
                LOAD_FLAG = 1;
            delay_ms(10); // 消抖
        }
        else
        {
            delay_ms(10); // 消抖
            if (KEY0_READ())
                LOAD_FLAG = 1;
            else
                LOAD_FLAG = 0;
            delay_ms(10); // 消抖
        }
    }
}

// 从串口得到数字
void GET_NUM(void)
{
    if (USART_RX_STA & 0x8000) // 接受到数字
    {
        static int i = 0;

        i = USART_RX_STA & 0X3FFF; // 接收次数
        u8 temp[4] = {0};
        for (; i >= 0; i--)
        {
            temp[i] = USART_RX_BUF[i];
        }
        USART_RX_STA = 0; // 清楚标志位
        FIND_ROOM_FLAG = 1;
    }
    else
        FIND_ROOM_FLAG = 0;
}

// 小车直走
void Car_GO(short v)
{
    LINE_FLAG = 1;
    STOP_FLAG = 0;
    TURN_SUCCEED_FLAG = 0;
    TARGET_V[0] = TARGET_V[1] = v;

    motor1_velocity.control = PID_ENABLE;
    motor2_velocity.control = PID_ENABLE;
    motor_turn.control = PID_DISABLE;
}

// 小车转向
void Car_TURN(float turn_angle)
{
    LINE_FLAG = 0;
    STOP_FLAG = 0;
    Yaw_Start = Yaw;
    TARGET_ANGLE = turn_angle + Yaw_Start;

    TARGET_V[0] = TARGET_V[1] = 0; // 目标速度归零
    Location_sum = 0;              // 路程计数归零

    motor1_velocity.control = PID_DISABLE;
    motor2_velocity.control = PID_DISABLE;
    motor_turn.control = PID_ENABLE; // 转向环开启
}

// 小车转向
void Car_STOP(void)
{
    LINE_FLAG = 0;
    STOP_FLAG = 1;

    TARGET_V[0] = TARGET_V[1] = 0; // 目标速度归零
    Location_sum = 0;              // 路程计数归零

    motor1_velocity.control = PID_ENABLE;
    motor2_velocity.control = PID_ENABLE;
    motor_turn.control = PID_DISABLE;
}

u8 J_turn(float angle)
{
    if (TARGET_ANGLE - Yaw < angle)
        return 1;
    else
        return 0;
}