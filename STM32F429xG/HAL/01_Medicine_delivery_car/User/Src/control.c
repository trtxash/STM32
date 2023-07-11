#include "control.h"

positional_pid_params_t motor1_velocity;
positional_pid_params_t motor2_velocity;
positional_pid_params_t motor_turn;

u8 SUM[4] = {0}; // 存的是字符
short TARGET_V[2] = {0};
float TARGET_ANGLE = 0;
float Yaw_Start = 0;
u8 TASK = 0;
u8 GET_ROOM_FLAG = 0;
u8 FIND_ROOM_FLAG = 0;
u8 LOAD_FLAG = 0;
u8 AIM_SUM = 0;
u8 AIM_PLACE = 0;
u8 Do_count = 0;
u8 STOP_FLAG = 1, LINE_FLAG = 0, TURN_SUCCEED_FLAG = 0; // 初始车停下，不巡线
u8 LOR = 0;

// 主要任务
void MIAN_TASK(void)
{
    if (TASK == 0) // 等待识别目标房间
    {
        GET_SET_AIM_ROOM(); // 设置目标房间
        LOADORNOT();        // 检测药物
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
                case 0: // 直走
                    Car_GO(100);
                    Do_count++;
                    break;
                case 1:
                    if (Location_sum > 125536)
                        Do_count++;
                    break;
                case 2: // 低速检测数字
                    TARGET_V[0] = TARGET_V[1] = 50;
                    Do_count++;
                    break;
                case 3:
                    if (GET_NUM())
                        Do_count++;
                    break;
                case 4: // 设置房间
                    GET_SET_AIM_ROOM();
                    Do_count++;
                    break;
                case 5:
                    // if ()
                    // {
                    // }
                    break;
                case 6:
                    break;
                case 7:
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
            else
            {
            }
        }
    }
}

void GET_SET_AIM_ROOM(void)
{
    // 一开始识别目标房间
    if (GET_ROOM_FLAG == 0) // 等待
    {
        if (GET_NUM()) // 得到数字
        {
            GET_ROOM_FLAG = 1; // 标记得到目标房间
            AIM_SUM = SUM[0];  // 存下目标数字
        }
    }
    else if (GET_ROOM_FLAG == 1) // 识别开始房间
    {
        if (STOP_FLAG) // 分两种识别，开始识别（STOP）和路程中识别（LINE）
        {
            if (AIM_SUM == '1') // 设置目标房间
            {
                AIM_PLACE = 'A';
                FIND_ROOM_FLAG = 1;
            }
            else if (AIM_SUM == '2')
            {
                AIM_PLACE = 'B';
                FIND_ROOM_FLAG = 1;
            }
            else if (AIM_SUM > '2')
            {
                AIM_PLACE = 'C';
                FIND_ROOM_FLAG = 0; // 在中端找
            }
            TASK = 1;
        }
        else if (LINE_FLAG) // 路程中识别
        {

        }
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

// 从串口得到数字,返回1成功
u8 GET_NUM(void)
{
    if (USART_RX_STA & 0x8000) // 接受到数字
    {
        u8 i = 0;
        i = USART_RX_STA & 0X3FFF; // 接收次数

        if (i >= 3)
        {
            if (USART_RX_BUF[0] == '*' & USART_RX_BUF[i - 1] == '#') // 检验包头包尾
            {
                for (u8 j = 0; j < i - 2; j++) // 去掉包头包尾，存入数组
                {
                    SUM[j] = USART_RX_BUF[j + 1];
                }
                USART_RX_STA = 0; // 清楚标志位
                return 1;         // 接收成功
            }
        }
    }
    USART_RX_STA = 0; // 清楚标志位
    return 0;         // 接收失败
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

// 小车停止
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