#include "control.h"

positional_pid_params_t motor1_velocity;
positional_pid_params_t motor2_velocity;
positional_pid_params_t motor_turn;

short TARGET_V[2] = {0};
u8 TASK = 0;
u8 FIND_ROOM_FLAG = 0;
u8 LOAD_FLAG = 0;
u8 ROOM_NUM = 0, AIM_PLACE;
u8 Do_count = 0;
u8 STOP_FLAG = 0;

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
        if (LOAD_FLAG)
        {
            if (AIM_PLACE == 'A')
            {
                switch (Do_count)
                {
                case 0:
                    if (STOP_FLAG)
                        Do_count++;
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
    if (FIND_ROOM_FLAG == 0) // 等待，不接受串口数据
    {
    }
    else if (FIND_ROOM_FLAG == 1) // 识别开始房间
    {
        GET_NUM();
    }

    if (ROOM_NUM == 1) // 设置目标房间
    {
        AIM_PLACE = 'A';
        TASK = 1;
    }
    else if (ROOM_NUM == 2)
    {
        AIM_PLACE = 'B';
        TASK = 1;
    }
    else if (ROOM_NUM > 2)
    {
        AIM_PLACE = 'C';
        TASK = 1;
    }
}

// 检测药物
void LOADORNOT(void)
{
    if (KEY0_Init())
    {
        LOAD_FLAG = 1;
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
    }
}

// 小车直走
void Car_GO(u8 count)
{
}

float MOTOR_VELOCITY(void)
{
    // positional_pid_compute(&motor1_velocity, , );
}