#include "control.h"

#define BUCHANG_L 210
#define BUCHANG_D 10
#define BUCHANG_A 1
#define JUDGE_A 1
#define JUDGE_V 4000
#define LEFT_GR gray_sensor[0]
#define RIGHT_GR gray_sensor[7]

positional_pid_params_t motor1_velocity;
positional_pid_params_t motor2_velocity;
positional_pid_params_t motor12_location;
positional_pid_params_t motor_turn;
positional_pid_params_t xunxian;

u8 SUM[4] = {0};
int TARGET_LOCATION = 0; // 目标位置
short TARGET_V = 1000;   // 目标速度
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
u8 OPMVDATA[21] = {0};
int OPMV_FLAG = 0;     // 1 直行，2 路口
int OPMV_LP = 0;       // 位置偏移量
int OPMV_AP = 0;       // 角度偏移量
int OPMV_STOPFLAG = 0; // 终止
u8 TWOCAR_FLAG = 0;
u8 TWOCAR_LINEFLAG = 0;

// 第一个岔口:2150，第二个岔口4850，第三个岔路口7500，近中端1400，远端2650
// 主要任务
void MIAN_TASK(void)
{
    if (TASK == 0) // 等待识别目标房间
    {
        GET_SET_AIM_ROOM();
        LOADORNOT(); // 检测药物
        if (LOAD_FLAG & GET_ROOM_FLAG)
            TASK = 1;
    }
    else if (TASK == 1)
    {
        if (LOAD_FLAG == 1) // 送药
        {
            if (TWOCAR_FLAG == 0) // 单车
            {
                if (AIM_PLACE == 'A')
                {
                    switch (Do_count)
                    {
                    case 0: // 直走
                        Car_GO(5000, 2150);
                        Do_count++;
                        break;
                    case 1:
                        if (Location_sum > 1100)
                        {
                            TARGET_V = JUDGE_V;
                            Do_count++;
                        }
                        break;
                    case 2:
                        if (gray_sensor_sum > 2)
                        {
                            Car_GO(JUDGE_V, BUCHANG_L);
                            STOP_FLAG = 1;
                            Do_count++;
                        }
                        break;
                    case 3:
                        if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                            Do_count++;
                        break;
                    case 4: // 左转向
                        Car_TURN(90);
                        Do_count++;
                        break;
                    case 5:
                        if (J_turn(BUCHANG_A)) // 转向角差5以内
                            TURN_SUCCEED_FLAG = 1;
                        if (TURN_SUCCEED_FLAG)
                            Do_count++;
                        break;
                    case 6: // 直走
                        Car_GO(5000, 1400);
                        Do_count++;
                        break;
                    case 7:
                        if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                            STOP_FLAG = 1;
                        if (STOP_FLAG & Location_sum > 1300)
                            Do_count++;
                        break;
                    case 8: // 卸药
                        Car_STOP();
                        LED0_Set();
                        Do_count++;
                        break;
                    case 9:
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
                        Car_GO(5000, 2150);
                        Do_count++;
                        break;
                    case 1:
                        if (Location_sum > 1100)
                        {
                            TARGET_V = JUDGE_V;
                            Do_count++;
                        }
                        break;
                    case 2:
                        if (gray_sensor_sum > 2)
                        {
                            Car_GO(JUDGE_V, BUCHANG_L);
                            STOP_FLAG = 1;
                            Do_count++;
                        }
                        break;
                    case 3:
                        if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                            Do_count++;
                        break;
                    case 4: // 转向
                        Car_TURN(-90);
                        Do_count++;
                        break;
                    case 5:
                        if (J_turn(BUCHANG_A)) // 转向角差5以内
                            TURN_SUCCEED_FLAG = 1;
                        if (TURN_SUCCEED_FLAG)
                            Do_count++;
                        break;
                    case 6: // 直走
                        Car_GO(5000, 1400);
                        Do_count++;
                        break;
                    case 7:
                        if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                            STOP_FLAG = 1;
                        if (STOP_FLAG & Location_sum > 1300)
                            Do_count++;
                        break;
                    case 8: // 卸药
                        Car_STOP();
                        LED0_Set();
                        Do_count++;
                        break;
                    case 9:
                        LOADORNOT();
                        break;
                    default:
                        break;
                    }
                }
                else // 中端病房和远端病房需要发送特定标志位
                {
                    if (AIM_PLACE == 'C')
                    {
                        switch (Do_count)
                        {
                        case 0: // 直走
                            Car_GO(8000, 4850);
                            Do_count++;
                            break;
                        case 1:
                            if (Location_sum > 3000)
                            {
                                TARGET_V = 5000;
                                Do_count++;
                            }
                            break;
                        case 2:
                            if (Location_sum > 3500)
                            {
                                TARGET_V = JUDGE_V;
                                Do_count++;
                            }
                            break;
                        case 3: // 识别数字
                            if (GET_SET_AIM_ROOM() == 0)
                            {
                                Do_count = 0;
                            }
                            break;
                        default:
                            break;
                        }
                    }
                    else if (AIM_PLACE == 'D')
                    {
                        switch (Do_count)
                        {
                        case 0: // 直走
                            Car_GO(JUDGE_V, 1350);
                            Do_count++;
                            break;
                        case 1:
                            if (gray_sensor_sum > 2)
                            {
                                Car_GO(JUDGE_V, BUCHANG_L);
                                STOP_FLAG = 1;
                                Do_count++;
                            }
                            break;
                        case 2: // 左转向
                            if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                                Do_count++;
                            break;
                        case 3: // 左转向
                            Car_TURN(90);
                            Do_count++;
                            break;
                        case 4:
                            if (J_turn(BUCHANG_A)) // 转向角差5以内
                                TURN_SUCCEED_FLAG = 1;
                            if (TURN_SUCCEED_FLAG)
                                Do_count++;
                            break;
                        case 5: // 直走
                            Car_GO(5000, 1400);
                            Do_count++;
                            break;
                        case 6:
                            if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                                STOP_FLAG = 1;
                            if (STOP_FLAG & Location_sum > 1350)
                                Do_count++;
                            break;
                        case 7: // 卸药
                            Car_STOP();
                            LED0_Set();
                            Do_count++;
                            break;
                        case 8:
                            LOADORNOT();
                            break;
                        default:
                            break;
                        }
                    }
                    else if (AIM_PLACE == 'E')
                    {
                        switch (Do_count)
                        {
                        case 0: // 直走
                            Car_GO(JUDGE_V, 1350);
                            Do_count++;
                            break;
                        case 1:
                            if (gray_sensor_sum > 2)
                            {
                                Car_GO(JUDGE_V, BUCHANG_L);
                                STOP_FLAG = 1;
                                Do_count++;
                            }
                            break;
                        case 2: // 左转向
                            if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                                Do_count++;
                            break;
                        case 3: // 转向
                            Car_TURN(-90);
                            Do_count++;
                            break;
                        case 4:
                            if (J_turn(BUCHANG_A)) // 转向角差5以内
                                TURN_SUCCEED_FLAG = 1;
                            if (TURN_SUCCEED_FLAG)
                                Do_count++;
                            break;
                        case 5: // 直走
                            Car_GO(5000, 1400);
                            Do_count++;
                            break;
                        case 6:
                            if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                                STOP_FLAG = 1;
                            if (STOP_FLAG & Location_sum > 1350)
                                Do_count++;
                            break;
                        case 7: // 卸药
                            Car_STOP();
                            LED0_Set();
                            Do_count++;
                            break;
                        case 8:
                            LOADORNOT();
                            break;
                        default:
                            break;
                        }
                    }
                    else if (AIM_PLACE == 'F')
                    {
                        switch (Do_count)
                        {
                        case 0: // 直走
                            Car_GO(5000, 2650);
                            Do_count++;
                            break;
                        case 1:
                            if (Location_sum > 1400)
                            {
                                TARGET_V = JUDGE_V;
                                Do_count++;
                            }
                            break;
                        case 2: // 识别数字
                            if (GET_SET_AIM_ROOM() == 0)
                            {
                                Do_count = 0;
                            }
                            break;
                        default:
                            break;
                        }
                    }
                    else if (AIM_PLACE == 'G')
                    {
                        switch (Do_count)
                        {
                        case 0: // 直走
                            Car_GO(JUDGE_V, 1300);
                            Do_count++;
                            break;
                        case 1:
                            if (gray_sensor_sum > 2)
                            {
                                Car_GO(JUDGE_V, BUCHANG_L);
                                STOP_FLAG = 1;
                                Do_count++;
                            }
                            break;
                        case 2: // 左转向
                            if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                                Do_count++;
                            break;
                        case 3: // 左转向
                            Car_TURN(90);
                            Do_count++;
                            break;
                        case 4:
                            if (J_turn(BUCHANG_A)) // 转向角差5以内
                                TURN_SUCCEED_FLAG = 1;
                            if (TURN_SUCCEED_FLAG)
                                Do_count++;
                            break;
                        case 5: // 直走
                            Car_GO(6000, 2650);
                            Do_count++;
                            break;
                        case 6:
                            if (Location_sum > 1300)
                            {
                                TARGET_V = JUDGE_V;
                                Do_count++;
                            }
                            break;
                        case 7: // 识别数字
                            if (GET_SET_AIM_ROOM() == 0)
                            {
                                Do_count = 0;
                            }
                            break;
                        default:
                            break;
                        }
                    }
                    else if (AIM_PLACE == 'H')
                    {
                        switch (Do_count)
                        {
                        case 0: // 直走
                            Car_GO(JUDGE_V, 1300);
                            Do_count++;
                            break;
                        case 1:
                            if (gray_sensor_sum > 2)
                            {
                                Car_GO(JUDGE_V, BUCHANG_L);
                                STOP_FLAG = 1;
                                Do_count++;
                            }
                            break;
                        case 2: // 转向
                            if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                                Do_count++;
                            break;
                        case 3: // 转向
                            Car_TURN(-90);
                            Do_count++;
                            break;
                        case 4:
                            if (J_turn(BUCHANG_A)) // 转向角差5以内
                                TURN_SUCCEED_FLAG = 1;
                            if (TURN_SUCCEED_FLAG)
                                Do_count++;
                            break;
                        case 5: // 直走
                            Car_GO(6000, 2650);
                            Do_count++;
                            break;
                        case 6:
                            if (Location_sum > 1300)
                            {
                                TARGET_V = JUDGE_V;
                                Do_count++;
                            }
                            break;
                        case 7: // 识别数字
                            if (GET_SET_AIM_ROOM() == 0)
                            {
                                Do_count = 0;
                            }
                            break;
                        default:
                            break;
                        }
                    }
                    else if (AIM_PLACE == 'I')
                    {
                        switch (Do_count)
                        {
                        case 0: // 直走
                            Car_GO(JUDGE_V, 1350);
                            Do_count++;
                            break;
                        case 1:
                            if (gray_sensor_sum > 2)
                            {
                                Car_GO(JUDGE_V, BUCHANG_L);
                                STOP_FLAG = 1;
                                Do_count++;
                            }
                            break;
                        case 2: // 转向
                            if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                                Do_count++;
                            break;
                        case 3: // 左转向
                            Car_TURN(85);
                            Do_count++;
                            break;
                        case 4:
                            if (J_turn(BUCHANG_A)) // 转向角差5以内
                                TURN_SUCCEED_FLAG = 1;
                            if (TURN_SUCCEED_FLAG)
                                Do_count++;
                            break;
                        case 5: // 直走
                            Car_GO(JUDGE_V, 1400);
                            Do_count++;
                            break;
                        case 6:
                            if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                                STOP_FLAG = 1;
                            if (STOP_FLAG & Location_sum > 1350)
                                Do_count++;
                            break;
                        case 7: // 卸药
                            Car_STOP();
                            LED0_Set();
                            Do_count++;
                            break;
                        case 8:
                            LOADORNOT();
                            break;
                        default:
                            break;
                        }
                    }
                    else if (AIM_PLACE == 'J')
                    {
                        switch (Do_count)
                        {
                        case 0: // 直走
                            Car_GO(JUDGE_V, 1350);
                            Do_count++;
                            break;
                        case 1:
                            if (gray_sensor_sum > 2)
                            {
                                Car_GO(JUDGE_V, BUCHANG_L);
                                STOP_FLAG = 1;
                                Do_count++;
                            }
                            break;
                        case 2: // 转向
                            if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                                Do_count++;
                            break;
                        case 3: // 转向
                            Car_TURN(-85);
                            Do_count++;
                            break;
                        case 4:
                            if (J_turn(BUCHANG_A)) // 转向角差5以内
                                TURN_SUCCEED_FLAG = 1;
                            if (TURN_SUCCEED_FLAG)
                                Do_count++;
                            break;
                        case 5: // 直走
                            Car_GO(JUDGE_V, 1400);
                            Do_count++;
                            break;
                        case 6:
                            if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                                STOP_FLAG = 1;
                            if (STOP_FLAG & Location_sum > 1350)
                                Do_count++;
                            break;
                        case 7: // 卸药
                            Car_STOP();
                            LED0_Set();
                            Do_count++;
                            break;
                        case 8:
                            LOADORNOT();
                            break;
                        default:
                            break;
                        }
                    }
                    else if (AIM_PLACE == 'K')
                    {
                        switch (Do_count)
                        {
                        case 0: // 直走
                            Car_GO(JUDGE_V, 1350);
                            Do_count++;
                            break;
                        case 1:
                            if (gray_sensor_sum >= 2)
                            {
                                Car_GO(JUDGE_V, BUCHANG_L);
                                STOP_FLAG = 1;
                                Do_count++;
                            }
                            break;
                        case 2: // 转向
                            if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                                Do_count++;
                            break;
                        case 3: // 左转向
                            Car_TURN(85);
                            Do_count++;
                            break;
                        case 4:
                            if (J_turn(BUCHANG_A)) // 转向角差5以内
                                TURN_SUCCEED_FLAG = 1;
                            if (TURN_SUCCEED_FLAG)
                                Do_count++;
                            break;
                        case 5: // 直走
                            Car_GO(JUDGE_V, 1400);
                            Do_count++;
                            break;
                        case 6:
                            if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                                STOP_FLAG = 1;
                            if (STOP_FLAG & Location_sum > 1350)
                                Do_count++;
                            break;
                        case 7: // 卸药
                            Car_STOP();
                            LED0_Set();
                            Do_count++;
                            break;
                        case 8:
                            LOADORNOT();
                            break;
                        default:
                            break;
                        }
                    }
                    else if (AIM_PLACE == 'L')
                    {
                        switch (Do_count)
                        {
                        case 0: // 直走
                            Car_GO(JUDGE_V, 1350);
                            Do_count++;
                            break;
                        case 1:
                            if (gray_sensor_sum > 2)
                            {
                                Car_GO(JUDGE_V, BUCHANG_L);
                                STOP_FLAG = 1;
                                Do_count++;
                            }
                            break;
                        case 2: // 转向
                            if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                                Do_count++;
                            break;
                        case 3: // 转向
                            Car_TURN(-85);
                            Do_count++;
                            break;
                        case 4:
                            if (J_turn(BUCHANG_A)) // 转向角差5以内
                                TURN_SUCCEED_FLAG = 1;
                            if (TURN_SUCCEED_FLAG)
                                Do_count++;
                            break;
                        case 5: // 直走
                            Car_GO(JUDGE_V, 1400);
                            Do_count++;
                            break;
                        case 6:
                            if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                                STOP_FLAG = 1;
                            if (STOP_FLAG & Location_sum > 1350)
                                Do_count++;
                            break;
                        case 7: // 卸药
                            Car_STOP();
                            LED0_Set();
                            Do_count++;
                            break;
                        case 8:
                            LOADORNOT();
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
            else if (TWOCAR_FLAG == 1) // 双车
            {
                if (AIM_PLACE == 'A')
                {
                    switch (Do_count)
                    {
                    case 0: // 直走
                        Car_GO(5000, 2150);
                        Do_count++;
                        break;
                    case 1:
                        if (Location_sum > 1100)
                        {
                            TARGET_V = JUDGE_V;
                            Do_count++;
                        }
                        break;
                    case 2:
                        if (gray_sensor_sum > 2)
                        {
                            Car_GO(JUDGE_V, BUCHANG_L);
                            STOP_FLAG = 1;
                            Do_count++;
                        }
                        break;
                    case 3:
                        if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                            Do_count++;
                        break;
                    case 4: // 左转向
                        Car_TURN(90);
                        Do_count++;
                        break;
                    case 5:
                        if (J_turn(BUCHANG_A)) // 转向角差5以内
                            TURN_SUCCEED_FLAG = 1;
                        if (TURN_SUCCEED_FLAG)
                            Do_count++;
                        break;
                    case 6: // 直走
                        Car_GO(5000, 1400);
                        Do_count++;
                        break;
                    case 7:
                        if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                            STOP_FLAG = 1;
                        if (STOP_FLAG & Location_sum > 1300)
                            Do_count++;
                        break;
                    case 8: // 卸药
                        Car_STOP();
                        LED0_Set();
                        Do_count++;
                        break;
                    case 9:
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
                        Car_GO(5000, 2150);
                        Do_count++;
                        break;
                    case 1:
                        if (Location_sum > 1100)
                        {
                            TARGET_V = JUDGE_V;
                            Do_count++;
                        }
                        break;
                    case 2:
                        if (gray_sensor_sum > 2)
                        {
                            Car_GO(JUDGE_V, BUCHANG_L);
                            STOP_FLAG = 1;
                            Do_count++;
                        }
                        break;
                    case 3:
                        if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                            Do_count++;
                        break;
                    case 4: // 转向
                        Car_TURN(-90);
                        Do_count++;
                        break;
                    case 5:
                        if (J_turn(BUCHANG_A)) // 转向角差5以内
                            TURN_SUCCEED_FLAG = 1;
                        if (TURN_SUCCEED_FLAG)
                            Do_count++;
                        break;
                    case 6: // 直走
                        Car_GO(5000, 1400);
                        Do_count++;
                        break;
                    case 7:
                        if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                            STOP_FLAG = 1;
                        if (STOP_FLAG & Location_sum > 1300)
                            Do_count++;
                        break;
                    case 8: // 卸药
                        Car_STOP();
                        LED0_Set();
                        Do_count++;
                        break;
                    case 9:
                        LOADORNOT();
                        break;
                    default:
                        break;
                    }
                }
                else // 中端病房和远端病房需要发送特定标志位
                {
                    if (AIM_PLACE == 'C')
                    {
                        switch (Do_count)
                        {
                        case 0: // 直走
                            Car_GO(8000, 4850);
                            Do_count++;
                            break;
                        case 1:
                            if (Location_sum > 3000)
                            {
                                TARGET_V = 5000;
                                Do_count++;
                            }
                            break;
                        case 2:
                            if (Location_sum > 3500)
                            {
                                TARGET_V = JUDGE_V;
                                Do_count++;
                            }
                            break;
                        case 3: // 识别数字
                            if (GET_SET_AIM_ROOM() == 0)
                            {
                                Do_count = 0;
                            }
                            break;
                        default:
                            break;
                        }
                    }
                    else if (AIM_PLACE == 'D')
                    {
                        switch (Do_count)
                        {
                        case 0: // 直走
                            Car_GO(JUDGE_V, 1350);
                            Do_count++;
                            break;
                        case 1:
                            if (gray_sensor_sum > 2)
                            {
                                Car_GO(JUDGE_V, BUCHANG_L);
                                STOP_FLAG = 1;
                                Do_count++;
                            }
                            break;
                        case 2: // 左转向
                            if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                                Do_count++;
                            break;
                        case 3: // 左转向
                            Car_TURN(90);
                            Do_count++;
                            break;
                        case 4:
                            if (J_turn(BUCHANG_A)) // 转向角差5以内
                                TURN_SUCCEED_FLAG = 1;
                            if (TURN_SUCCEED_FLAG)
                                Do_count++;
                            break;
                        case 5: // 直走
                            Car_GO(5000, 1400);
                            Do_count++;
                            break;
                        case 6:
                            if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                                STOP_FLAG = 1;
                            if (STOP_FLAG & Location_sum > 1350)
                                Do_count++;
                            break;
                        case 7: // 卸药
                            Car_STOP();
                            LED0_Set();
                            Do_count++;
                            break;
                        case 8:
                            LOADORNOT();
                            break;
                        default:
                            break;
                        }
                    }
                    else if (AIM_PLACE == 'E')
                    {
                        switch (Do_count)
                        {
                        case 0: // 直走
                            Car_GO(JUDGE_V, 1350);
                            Do_count++;
                            break;
                        case 1:
                            if (gray_sensor_sum > 2)
                            {
                                Car_GO(JUDGE_V, BUCHANG_L);
                                STOP_FLAG = 1;
                                Do_count++;
                            }
                            break;
                        case 2: // 左转向
                            if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                                Do_count++;
                            break;
                        case 3: // 转向
                            Car_TURN(-90);
                            Do_count++;
                            break;
                        case 4:
                            if (J_turn(BUCHANG_A)) // 转向角差5以内
                                TURN_SUCCEED_FLAG = 1;
                            if (TURN_SUCCEED_FLAG)
                                Do_count++;
                            break;
                        case 5: // 直走
                            Car_GO(5000, 1400);
                            Do_count++;
                            break;
                        case 6:
                            if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                                STOP_FLAG = 1;
                            if (STOP_FLAG & Location_sum > 1350)
                                Do_count++;
                            break;
                        case 7: // 卸药
                            Car_STOP();
                            LED0_Set();
                            Do_count++;
                            break;
                        case 8:
                            LOADORNOT();
                            break;
                        default:
                            break;
                        }
                    }
                    else if (AIM_PLACE == 'F')
                    {
                        switch (Do_count)
                        {
                        case 0: // 直走
                            Car_GO(5000, 2650);
                            Do_count++;
                            break;
                        case 1:
                            if (Location_sum > 1400)
                            {
                                TARGET_V = JUDGE_V;
                                Do_count++;
                            }
                            break;
                        case 2: // 识别数字
                            if (GET_SET_AIM_ROOM() == 0)
                            {
                                Do_count = 0;
                            }
                            break;
                        default:
                            break;
                        }
                    }
                    else if (AIM_PLACE == 'G')
                    {
                        switch (Do_count)
                        {
                        case 0: // 直走
                            Car_GO(JUDGE_V, 1300);
                            Do_count++;
                            break;
                        case 1:
                            if (gray_sensor_sum > 2)
                            {
                                Car_GO(JUDGE_V, BUCHANG_L);
                                STOP_FLAG = 1;
                                Do_count++;
                            }
                            break;
                        case 2: // 左转向
                            if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                                Do_count++;
                            break;
                        case 3: // 左转向
                            Car_TURN(90);
                            Do_count++;
                            break;
                        case 4:
                            if (J_turn(BUCHANG_A)) // 转向角差5以内
                                TURN_SUCCEED_FLAG = 1;
                            if (TURN_SUCCEED_FLAG)
                                Do_count++;
                            break;
                        case 5: // 直走
                            Car_GO(6000, 2650);
                            Do_count++;
                            break;
                        case 6:
                            if (Location_sum > 1300)
                            {
                                TARGET_V = JUDGE_V;
                                Do_count++;
                            }
                            break;
                        case 7: // 识别数字
                            if (GET_SET_AIM_ROOM() == 0)
                            {
                                Do_count = 0;
                            }
                            break;
                        default:
                            break;
                        }
                    }
                    else if (AIM_PLACE == 'H')
                    {
                        switch (Do_count)
                        {
                        case 0: // 直走
                            Car_GO(JUDGE_V, 1300);
                            Do_count++;
                            break;
                        case 1:
                            if (gray_sensor_sum > 2)
                            {
                                Car_GO(JUDGE_V, BUCHANG_L);
                                STOP_FLAG = 1;
                                Do_count++;
                            }
                            break;
                        case 2: // 转向
                            if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                                Do_count++;
                            break;
                        case 3: // 转向
                            Car_TURN(-90);
                            Do_count++;
                            break;
                        case 4:
                            if (J_turn(BUCHANG_A)) // 转向角差5以内
                                TURN_SUCCEED_FLAG = 1;
                            if (TURN_SUCCEED_FLAG)
                                Do_count++;
                            break;
                        case 5: // 直走
                            Car_GO(6000, 2650);
                            Do_count++;
                            break;
                        case 6:
                            if (Location_sum > 1300)
                            {
                                TARGET_V = JUDGE_V;
                                Do_count++;
                            }
                            break;
                        case 7: // 识别数字
                            if (GET_SET_AIM_ROOM() == 0)
                            {
                                Do_count = 0;
                            }
                            break;
                        default:
                            break;
                        }
                    }
                    else if (AIM_PLACE == 'I')
                    {
                        switch (Do_count)
                        {
                        case 0: // 直走
                            Car_GO(JUDGE_V, 1350);
                            Do_count++;
                            break;
                        case 1:
                            if (gray_sensor_sum > 2)
                            {
                                Car_GO(JUDGE_V, BUCHANG_L);
                                STOP_FLAG = 1;
                                Do_count++;
                            }
                            break;
                        case 2: // 转向
                            if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                                Do_count++;
                            break;
                        case 3: // 左转向
                            Car_TURN(85);
                            Do_count++;
                            break;
                        case 4:
                            if (J_turn(BUCHANG_A)) // 转向角差5以内
                                TURN_SUCCEED_FLAG = 1;
                            if (TURN_SUCCEED_FLAG)
                                Do_count++;
                            break;
                        case 5: // 直走
                            Car_GO(JUDGE_V, 1400);
                            Do_count++;
                            break;
                        case 6:
                            if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                                STOP_FLAG = 1;
                            if (STOP_FLAG & Location_sum > 1350)
                                Do_count++;
                            break;
                        case 7: // 卸药
                            Car_STOP();
                            LED0_Set();
                            Do_count++;
                            break;
                        case 8:
                            LOADORNOT();
                            break;
                        default:
                            break;
                        }
                    }
                    else if (AIM_PLACE == 'J')
                    {
                        switch (Do_count)
                        {
                        case 0: // 直走
                            Car_GO(JUDGE_V, 1350);
                            Do_count++;
                            break;
                        case 1:
                            if (gray_sensor_sum > 2)
                            {
                                Car_GO(JUDGE_V, BUCHANG_L);
                                STOP_FLAG = 1;
                                Do_count++;
                            }
                            break;
                        case 2: // 转向
                            if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                                Do_count++;
                            break;
                        case 3: // 转向
                            Car_TURN(-85);
                            Do_count++;
                            break;
                        case 4:
                            if (J_turn(BUCHANG_A)) // 转向角差5以内
                                TURN_SUCCEED_FLAG = 1;
                            if (TURN_SUCCEED_FLAG)
                                Do_count++;
                            break;
                        case 5: // 直走
                            Car_GO(JUDGE_V, 1400);
                            Do_count++;
                            break;
                        case 6:
                            if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                                STOP_FLAG = 1;
                            if (STOP_FLAG & Location_sum > 1350)
                                Do_count++;
                            break;
                        case 7: // 卸药
                            Car_STOP();
                            LED0_Set();
                            Do_count++;
                            break;
                        case 8:
                            LOADORNOT();
                            break;
                        default:
                            break;
                        }
                    }
                    else if (AIM_PLACE == 'K')
                    {
                        switch (Do_count)
                        {
                        case 0: // 直走
                            Car_GO(JUDGE_V, 1350);
                            Do_count++;
                            break;
                        case 1:
                            if (gray_sensor_sum >= 2)
                            {
                                Car_GO(JUDGE_V, BUCHANG_L);
                                STOP_FLAG = 1;
                                Do_count++;
                            }
                            break;
                        case 2: // 转向
                            if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                                Do_count++;
                            break;
                        case 3: // 左转向
                            Car_TURN(85);
                            Do_count++;
                            break;
                        case 4:
                            if (J_turn(BUCHANG_A)) // 转向角差5以内
                                TURN_SUCCEED_FLAG = 1;
                            if (TURN_SUCCEED_FLAG)
                                Do_count++;
                            break;
                        case 5: // 直走
                            Car_GO(JUDGE_V, 1400);
                            Do_count++;
                            break;
                        case 6:
                            if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                                STOP_FLAG = 1;
                            if (STOP_FLAG & Location_sum > 1350)
                                Do_count++;
                            break;
                        case 7: // 卸药
                            Car_STOP();
                            LED0_Set();
                            Do_count++;
                            break;
                        case 8:
                            LOADORNOT();
                            break;
                        default:
                            break;
                        }
                    }
                    else if (AIM_PLACE == 'L')
                    {
                        switch (Do_count)
                        {
                        case 0: // 直走
                            Car_GO(JUDGE_V, 1350);
                            Do_count++;
                            break;
                        case 1:
                            if (gray_sensor_sum > 2)
                            {
                                Car_GO(JUDGE_V, BUCHANG_L);
                                STOP_FLAG = 1;
                                Do_count++;
                            }
                            break;
                        case 2: // 转向
                            if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                                Do_count++;
                            break;
                        case 3: // 转向
                            Car_TURN(-85);
                            Do_count++;
                            break;
                        case 4:
                            if (J_turn(BUCHANG_A)) // 转向角差5以内
                                TURN_SUCCEED_FLAG = 1;
                            if (TURN_SUCCEED_FLAG)
                                Do_count++;
                            break;
                        case 5: // 直走
                            Car_GO(JUDGE_V, 1400);
                            Do_count++;
                            break;
                        case 6:
                            if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                                STOP_FLAG = 1;
                            if (STOP_FLAG & Location_sum > 1350)
                                Do_count++;
                            break;
                        case 7: // 卸药
                            Car_STOP();
                            LED0_Set();
                            Do_count++;
                            break;
                        case 8:
                            LOADORNOT();
                            break;
                        default:
                            break;
                        }
                    }
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
                    Car_TURN(-180);
                    LED0_Clr();
                    Do_count++;
                    break;
                case 1:
                    if (J_turn(BUCHANG_A)) // 转向角差5以内
                        TURN_SUCCEED_FLAG = 1;
                    if (TURN_SUCCEED_FLAG)
                        Do_count++;
                    break;
                case 2: // 直走
                    Car_GO(JUDGE_V, 1400);
                    Do_count++;
                    break;
                case 3:
                    if (gray_sensor_sum > 2)
                    {
                        Car_GO(JUDGE_V, BUCHANG_L);
                        STOP_FLAG = 1;
                        Do_count++;
                    }
                    break;
                case 4: // 转向
                    if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                        Do_count++;
                    break;
                case 5: // 转向
                    Car_TURN(-85);
                    Do_count++;
                    break;
                case 6:
                    if (J_turn(BUCHANG_A)) // 转向角差5以内
                        TURN_SUCCEED_FLAG = 1;
                    if (TURN_SUCCEED_FLAG)
                        Do_count++;
                    break;
                case 7: // 直走
                    Car_GO(5000, 2150);
                    Do_count++;
                    break;
                case 8:
                    if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                        STOP_FLAG = 1;
                    if (STOP_FLAG & Location_sum > 2100)
                        Do_count++;
                    break;
                case 9: // 停止
                    Car_STOP();
                    LED1_Set();
                    Do_count++;
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
                case 1:
                    if (J_turn(BUCHANG_A)) // 转向角差5以内
                        TURN_SUCCEED_FLAG = 1;
                    if (TURN_SUCCEED_FLAG)
                        Do_count++;
                    break;
                case 2: // 直走
                    Car_GO(JUDGE_V, 1400);
                    Do_count++;
                    break;
                case 3:
                    if (gray_sensor_sum > 2)
                    {
                        Car_GO(JUDGE_V, BUCHANG_L);
                        STOP_FLAG = 1;
                        Do_count++;
                    }
                    break;
                case 4:
                    if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                        Do_count++;
                    break;
                case 5: // 转向
                    Car_TURN(85);
                    Do_count++;
                    break;
                case 6:
                    if (J_turn(BUCHANG_A)) // 转向角差5以内
                        TURN_SUCCEED_FLAG = 1;
                    if (TURN_SUCCEED_FLAG)
                        Do_count++;
                    break;
                case 7: // 直走
                    Car_GO(5000, 2150);
                    Do_count++;
                    break;
                case 8:
                    if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                        STOP_FLAG = 1;
                    if (STOP_FLAG & Location_sum > 2100)
                        Do_count++;
                    break;
                case 9: // 停止
                    Car_STOP();
                    LED1_Set();
                    Do_count++;
                    break;
                default:
                    break;
                }
            }
            else
            {
                if (AIM_PLACE == 'D')
                {
                    switch (Do_count)
                    {
                    case 0: // 转向
                        Car_TURN(-180);
                        LED0_Clr();
                        Do_count++;
                        break;
                    case 1:
                        if (J_turn(BUCHANG_A)) // 转向角差5以内
                            TURN_SUCCEED_FLAG = 1;
                        if (TURN_SUCCEED_FLAG)
                            Do_count++;
                        break;
                    case 2: // 直走
                        Car_GO(JUDGE_V, 1400);
                        Do_count++;
                        break;
                    case 3:
                        if (gray_sensor_sum > 2 & RIGHT_GR)
                        {
                            Car_GO(JUDGE_V, BUCHANG_L);
                            STOP_FLAG = 1;
                            Do_count++;
                        }
                        break;
                    case 4:
                        if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                            Do_count++;
                        break;
                    case 5: // 转向
                        Car_TURN(-85);
                        Do_count++;
                        break;
                    case 6:
                        if (J_turn(BUCHANG_A)) // 转向角差5以内
                            TURN_SUCCEED_FLAG = 1;
                        if (TURN_SUCCEED_FLAG)
                            Do_count++;
                        break;
                    case 7: // 直走
                        Car_GO(8000, 4850);
                        Do_count++;
                        break;
                    case 8:
                        if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                            STOP_FLAG = 1;
                        if (STOP_FLAG & Location_sum > 4800)
                            Do_count++;
                        break;
                    case 9: // 停止
                        Car_STOP();
                        LED1_Set();
                        Do_count++;
                        break;
                    default:
                        break;
                    }
                }
                else if (AIM_PLACE == 'E')
                {
                    switch (Do_count)
                    {
                    case 0: // 转向
                        Car_TURN(180);
                        LED0_Clr();
                        Do_count++;
                        break;
                    case 1:
                        if (J_turn(BUCHANG_A)) // 转向角差5以内
                            TURN_SUCCEED_FLAG = 1;
                        if (TURN_SUCCEED_FLAG)
                            Do_count++;
                        break;
                    case 2: // 直走
                        Car_GO(JUDGE_V, 1400);
                        Do_count++;
                        break;
                    case 3:
                        if (gray_sensor_sum > 2 & LEFT_GR)
                        {
                            Car_GO(JUDGE_V, BUCHANG_L);
                            STOP_FLAG = 1;
                            Do_count++;
                        }
                        break;
                    case 4:
                        if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                            Do_count++;
                        break;
                    case 5: // 转向
                        Car_TURN(85);
                        Do_count++;
                        break;
                    case 6:
                        if (J_turn(BUCHANG_A)) // 转向角差5以内
                            TURN_SUCCEED_FLAG = 1;
                        if (TURN_SUCCEED_FLAG)
                            Do_count++;
                        break;
                    case 7: // 直走
                        Car_GO(8000, 4850);
                        Do_count++;
                        break;
                    case 8:
                        if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                            STOP_FLAG = 1;
                        if (STOP_FLAG & Location_sum > 4800)
                            Do_count++;
                        break;
                    case 9: // 停止
                        Car_STOP();
                        LED1_Set();
                        Do_count++;
                        break;
                    default:
                        break;
                    }
                }
                else if (AIM_PLACE == 'I')
                {
                    switch (Do_count)
                    {
                    case 0: // 转向
                        if (Yaw > JUDGE_A)
                            Car_TURN(-180.0);
                        else if (Yaw < -JUDGE_A)
                            Car_TURN(180.0);
                        else
                            break;
                        LED0_Clr();
                        Do_count++;
                        break;
                    case 1:
                        if (J_turn(BUCHANG_A)) // 转向角差x以内
                            TURN_SUCCEED_FLAG = 1;
                        if (TURN_SUCCEED_FLAG)
                            Do_count++;
                        break;
                    case 2: // 直走
                        Car_GO(JUDGE_V, 1400);
                        Do_count++;
                        break;
                    case 3:
                        if (RIGHT_GR & Location_sum > 1000)
                        {
                            Car_GO(JUDGE_V, BUCHANG_L);
                            STOP_FLAG = 1;
                            Do_count++;
                        }
                        break;
                    case 4:
                        if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                            Do_count++;
                        break;
                    case 5: // 转向
                        Car_TURN(-90);
                        Do_count++;
                        break;
                    case 6:
                        if (J_turn(BUCHANG_A)) // 转向角差5以内
                            TURN_SUCCEED_FLAG = 1;
                        if (TURN_SUCCEED_FLAG)
                            Do_count++;
                        break;
                    case 7: // 直走
                        Car_GO(JUDGE_V, 2650);
                        Do_count++;
                        break;
                    case 8:
                        if (RIGHT_GR & Location_sum > 2250)
                        {
                            Car_GO(JUDGE_V, BUCHANG_L);
                            STOP_FLAG = 1;
                            Do_count++;
                        }
                        break;
                    case 9:
                        if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                            Do_count++;
                        break;
                    case 10: // 转向
                        Car_TURN(-85);
                        Do_count++;
                        break;
                    case 11:
                        if (J_turn(BUCHANG_A)) // 转向角差5以内
                            TURN_SUCCEED_FLAG = 1;
                        if (TURN_SUCCEED_FLAG)
                            Do_count++;
                        break;
                    case 12: // 直走
                        Car_GO(5000, 7500);
                        Do_count++;
                        break;
                    case 13:
                        if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                            STOP_FLAG = 1;
                        if (STOP_FLAG & Location_sum > 7450)
                            Do_count++;
                        break;
                    case 14: // 停止
                        Car_STOP();
                        LED1_Set();
                        Do_count++;
                        break;
                    default:
                        break;
                    }
                }
                else if (AIM_PLACE == 'J')
                {
                    switch (Do_count)
                    {
                    case 0: // 转向
                        Car_TURN(175);
                        LED0_Clr();
                        Do_count++;
                        break;
                    case 1:
                        if (J_turn(BUCHANG_A)) // 转向角差x以内
                            TURN_SUCCEED_FLAG = 1;
                        if (TURN_SUCCEED_FLAG)
                            Do_count++;
                        break;
                    case 2: // 直走
                        Car_GO(JUDGE_V, 1400);
                        Do_count++;
                        break;
                    case 3:
                        if (LEFT_GR & Location_sum > 1000)
                        {
                            Car_GO(JUDGE_V, BUCHANG_L);
                            STOP_FLAG = 1;
                            Do_count++;
                        }
                        break;
                    case 4:
                        if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                            Do_count++;
                        break;
                    case 5: // 转向
                        if (Yaw > JUDGE_A)
                            Car_TURN(-270.0);
                        else if (Yaw < -JUDGE_A)
                            Car_TURN(90.0);
                        else
                            break;
                        Do_count++;
                        break;
                    case 6:
                        if (J_turn(BUCHANG_A)) // 转向角差5以内
                            TURN_SUCCEED_FLAG = 1;
                        if (TURN_SUCCEED_FLAG)
                            Do_count++;
                        break;
                    case 7: // 直走
                        Car_GO(JUDGE_V, 2650);
                        Do_count++;
                        break;
                    case 8:
                        if (RIGHT_GR & Location_sum > 2250)
                        {
                            Car_GO(JUDGE_V, BUCHANG_L);
                            STOP_FLAG = 1;
                            Do_count++;
                        }
                        break;
                    case 9:
                        if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                            Do_count++;
                        break;
                    case 10: // 转向
                        Car_TURN(-85);
                        Do_count++;
                        break;
                    case 11:
                        if (J_turn(BUCHANG_A)) // 转向角差5以内
                            TURN_SUCCEED_FLAG = 1;
                        if (TURN_SUCCEED_FLAG)
                            Do_count++;
                        break;
                    case 12: // 直走
                        Car_GO(5000, 7500);
                        Do_count++;
                        break;
                    case 13:
                        if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                            STOP_FLAG = 1;
                        if (STOP_FLAG & Location_sum > 7450)
                            Do_count++;
                        break;
                    case 14: // 停止
                        Car_STOP();
                        LED1_Set();
                        Do_count++;
                        break;
                    default:
                        break;
                    }
                }
                else if (AIM_PLACE == 'K')
                {
                    switch (Do_count)
                    {
                    case 0: // 转向
                        Car_TURN(175);
                        LED0_Clr();
                        Do_count++;
                        break;
                    case 1:
                        if (J_turn(BUCHANG_A)) // 转向角差x以内
                            TURN_SUCCEED_FLAG = 1;
                        if (TURN_SUCCEED_FLAG)
                            Do_count++;
                        break;
                    case 2: // 直走
                        Car_GO(JUDGE_V, 1400);
                        Do_count++;
                        break;
                    case 3:
                        if (RIGHT_GR & Location_sum > 1000)
                        {
                            Car_GO(JUDGE_V, BUCHANG_L);
                            STOP_FLAG = 1;
                            Do_count++;
                        }
                        break;
                    case 4:
                        if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                            Do_count++;
                        break;
                    case 5: // 转向
                        if (Yaw > JUDGE_A)
                            Car_TURN(-90.0);
                        else if (Yaw < -JUDGE_A)
                            Car_TURN(270.0);
                        else
                            break;
                        Do_count++;
                        break;
                    case 6:
                        if (J_turn(BUCHANG_A)) // 转向角差5以内
                            TURN_SUCCEED_FLAG = 1;
                        if (TURN_SUCCEED_FLAG)
                            Do_count++;
                        break;
                    case 7: // 直走
                        Car_GO(JUDGE_V, 2650);
                        Do_count++;
                        break;
                    case 8:
                        if (LEFT_GR & Location_sum > 2250)
                        {
                            Car_GO(JUDGE_V, BUCHANG_L);
                            STOP_FLAG = 1;
                            Do_count++;
                        }
                        break;
                    case 9:
                        if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                            Do_count++;
                        break;
                    case 10: // 转向
                        Car_TURN(85);
                        Do_count++;
                        break;
                    case 11:
                        if (J_turn(BUCHANG_A)) // 转向角差5以内
                            TURN_SUCCEED_FLAG = 1;
                        if (TURN_SUCCEED_FLAG)
                            Do_count++;
                        break;
                    case 12: // 直走
                        Car_GO(5000, 7500);
                        Do_count++;
                        break;
                    case 13:
                        if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                            STOP_FLAG = 1;
                        if (STOP_FLAG & Location_sum > 7450)
                            Do_count++;
                        break;
                    case 14: // 停止
                        Car_STOP();
                        LED1_Set();
                        Do_count++;
                        break;
                    default:
                        break;
                    }
                }
                else if (AIM_PLACE == 'L')
                {
                    switch (Do_count)
                    {
                    case 0: // 转向
                        if (Yaw > JUDGE_A)
                            Car_TURN(-180.0);
                        else if (Yaw < -JUDGE_A)
                            Car_TURN(180.0);
                        else
                            break;
                        LED0_Clr();
                        Do_count++;
                        break;
                    case 1:
                        if (J_turn(BUCHANG_A)) // 转向角差x以内
                            TURN_SUCCEED_FLAG = 1;
                        if (TURN_SUCCEED_FLAG)
                            Do_count++;
                        break;
                    case 2: // 直走
                        Car_GO(JUDGE_V, 1400);
                        Do_count++;
                        break;
                    case 3:
                        if (LEFT_GR & Location_sum > 1000)
                        {
                            Car_GO(JUDGE_V, BUCHANG_L);
                            STOP_FLAG = 1;
                            Do_count++;
                        }
                        break;
                    case 4:
                        if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                            Do_count++;
                        break;
                    case 5: // 转向
                        Car_TURN(90);
                        Do_count++;
                        break;
                    case 6:
                        if (J_turn(BUCHANG_A)) // 转向角差5以内
                            TURN_SUCCEED_FLAG = 1;
                        if (TURN_SUCCEED_FLAG)
                            Do_count++;
                        break;
                    case 7: // 直走
                        Car_GO(JUDGE_V, 2650);
                        Do_count++;
                        break;
                    case 8:
                        if (LEFT_GR & Location_sum > 2250)
                        {
                            Car_GO(JUDGE_V, BUCHANG_L);
                            STOP_FLAG = 1;
                            Do_count++;
                        }
                        break;
                    case 9:
                        if (STOP_FLAG & ((BUCHANG_L - Location_sum) < BUCHANG_D))
                            Do_count++;
                        break;
                    case 10: // 转向
                        Car_TURN(85);
                        Do_count++;
                        break;
                    case 11:
                        if (J_turn(BUCHANG_A)) // 转向角差5以内
                            TURN_SUCCEED_FLAG = 1;
                        if (TURN_SUCCEED_FLAG)
                            Do_count++;
                        break;
                    case 12: // 直走
                        Car_GO(5000, 7500);
                        Do_count++;
                        break;
                    case 13:
                        if ((gray_sensor_sum > 2) | (gray_sensor_sum == 0))
                            STOP_FLAG = 1;
                        if (STOP_FLAG & Location_sum > 7450)
                            Do_count++;
                        break;
                    case 14: // 停止
                        Car_STOP();
                        LED1_Set();
                        Do_count++;
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
}

// 得到和识别数字，成功返回0，识别失败1，设置失败2
u8 GET_SET_AIM_ROOM(void)
{
    // 一开始识别目标房间
    if (GET_ROOM_FLAG == 0) // 等待
    {
        if (SUM[0])
        {
            if (J_sum(SUM[0], 3))
            {
                AIM_SUM = SUM[0];
                GET_ROOM_FLAG = 1;
                return 0;
            }
        }
        else
            return 1;
    }
    else if (GET_ROOM_FLAG == 1) // 识别开始房间
    {
        if (STOP_FLAG) // 分两种设置，开始识别（STOP）后设置和路程中识别（LINE）
        {
            if (AIM_SUM == 1) // 设置目标房间
            {
                AIM_PLACE = 'A';
                FIND_ROOM_FLAG = 1;
            }
            else if (AIM_SUM == 2)
            {
                AIM_PLACE = 'B';
                FIND_ROOM_FLAG = 1;
            }
            else if (AIM_SUM >= 3)
            {
                AIM_PLACE = 'C';
                FIND_ROOM_FLAG = 0; // 在中端找
            }
            return 0;
        }
        else if (LINE_FLAG) // 路程中识别
        {
            if (AIM_PLACE == 'C') // 中端路程
            {
                if (SUM[0] && SUM[1])
                {
                    if (AIM_SUM == SUM[0])
                    {
                        AIM_PLACE = 'D';
                        FIND_ROOM_FLAG = 1;
                    }
                    else if (AIM_SUM == SUM[1])
                    {
                        AIM_PLACE = 'E';
                        FIND_ROOM_FLAG = 1;
                    }
                    else
                    {
                        AIM_PLACE = 'F';
                        FIND_ROOM_FLAG = 0; // 在远端找
                    }
                    return 0;
                }
                else
                    return 1;
            }
            else if (AIM_PLACE == 'F') // 远端路程
            {
                if (SUM[0] + SUM[3] == 0)
                {
                    if (AIM_SUM == SUM[1])
                    {
                        AIM_PLACE = 'G';
                        FIND_ROOM_FLAG = 1;
                    }
                    else if (AIM_SUM == SUM[2])
                    {
                        AIM_PLACE = 'H';
                        FIND_ROOM_FLAG = 1;
                    }
                    else
                    {
                        AIM_PLACE = 'G';    // 默认左边
                        FIND_ROOM_FLAG = 0; // 在远端循环找
                    }
                    return 0;
                }
                else
                    return 1;
            }
            else if (AIM_PLACE == 'G') // 岔口
            {
                if (SUM[0] && SUM[1])
                {
                    if (AIM_SUM == SUM[0])
                    {
                        AIM_PLACE = 'I';
                        FIND_ROOM_FLAG = 1;
                    }
                    else if (AIM_SUM == SUM[1])
                    {
                        AIM_PLACE = 'J';
                        FIND_ROOM_FLAG = 1;
                    }
                    else
                    {
                        AIM_PLACE = 'H';
                        FIND_ROOM_FLAG = 0; // 在远端找
                    }
                    return 0;
                }
                else
                    return 1;
            }
            else if (AIM_PLACE == 'H') // 岔口
            {
                if (SUM[0] && SUM[1])
                {
                    if (AIM_SUM == SUM[0])
                    {
                        AIM_PLACE = 'K';
                        FIND_ROOM_FLAG = 1;
                    }
                    else if (AIM_SUM == SUM[1])
                    {
                        AIM_PLACE = 'L';
                        FIND_ROOM_FLAG = 1;
                    }
                    else
                    {
                        AIM_PLACE = 'G';
                        FIND_ROOM_FLAG = 0; // 在远端找
                    }
                    return 0;
                }
                else
                    return 1;
            }
        }
    }
}

// 检测药物
void LOADORNOT(void)
{
    if (KEY0_READ() | rxvaluepack.bools[0])
    {
        if (LOAD_FLAG == 1)
        {
            LOAD_FLAG = 2;
            Do_count = 0;
        }
        else
        {
            LOAD_FLAG = 1;
        }
        delay_ms(1000);
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
                for (u8 j = 0; j < 4; j++)
                    SUM[j] = 0;
                for (u8 j = 0; j < i - 2; j++) // 去掉包头包尾，存入数组
                {
                    SUM[j] = USART_RX_BUF[j + 1] - '0';
                }
                USART_RX_STA = 0; // 清楚标志位
                return 1;         // 接收成功
            }
        }
    }
    USART_RX_STA = 0; // 清楚标志位
    return 0;         // 接收失败
}

// 从串口得到数字,返回1成功
u8 GET_OPENMV(void)
{
    if (USART_RX_STA_C & 0x8000) // 接受到数字
    {
        u8 i = 0;
        i = USART_RX_STA_C & 0X3FFF; // 接收次数

        if (i >= 3)
        {
            if (USART_RX_BUF_C[0] == '*' & USART_RX_BUF_C[i - 1] == '#') // 检验包头包尾
            {
                for (u8 j = 0; j < 21; j++)
                    OPMVDATA[j] = 0;
                for (u8 j = 0; j < i - 2; j++) // 去掉包头包尾，存入数组
                {
                    OPMVDATA[j] = USART_RX_BUF_C[j + 1];
                }
                USART_RX_STA_C = 0; // 清楚标志位
                sscanf(OPMVDATA, "%d %d %d %d", &OPMV_FLAG, &OPMV_LP, &OPMV_AP, &OPMV_STOPFLAG);
                return 1; // 接收成功
            }
        }
    }
    USART_RX_STA_C = 0; // 清楚标志位
    return 0;           // 接收失败
}

// 小车直走
void Car_GO(short v, int targetl)
{
    TARGET_LOCATION = targetl;
    Location_sum = 0; // 路程计数归零
    LINE_FLAG = 1;
    STOP_FLAG = 0;
    TURN_SUCCEED_FLAG = 0;
    TARGET_V = v;

    motor1_velocity.control = PID_ENABLE;
    motor2_velocity.control = PID_ENABLE;
    motor12_location.control = PID_ENABLE;
    motor_turn.control = PID_DISABLE;
}

// 小车转向,有度数要求
void Car_TURN(float turn_angle)
{
    TARGET_LOCATION = 0;
    Location_sum = 0; // 路程计数归零
    LINE_FLAG = 0;
    STOP_FLAG = 0;
    // Yaw_Start = Yaw;
    // TARGET_ANGLE = turn_angle + Yaw_Start;
    Yaw_Start += turn_angle;
    TARGET_ANGLE = Yaw_Start;

    TARGET_V = 0; // 目标速度归零

    motor1_velocity.control = PID_ENABLE;
    motor2_velocity.control = PID_ENABLE;
    motor12_location.control = PID_DISABLE;
    motor_turn.control = PID_ENABLE; // 转向环开启
}

// 小车停止
void Car_STOP(void)
{
    TARGET_LOCATION = 0;
    Location_sum = 0; // 路程计数归零
    LINE_FLAG = 0;
    STOP_FLAG = 1;

    TARGET_V = 0; // 目标速度归零

    motor1_velocity.control = PID_ENABLE;
    motor2_velocity.control = PID_ENABLE;
    motor12_location.control = PID_DISABLE;
    motor_turn.control = PID_DISABLE;
}

u8 J_turn(float angle)
{
    if (TARGET_ANGLE - Yaw >= 0)
    {
        if (TARGET_ANGLE - Yaw <= angle)
            return 1;
        else
            return 0;
    }
    else
    {
        if (-(TARGET_ANGLE - Yaw) <= angle)
            return 1;
        else
            return 0;
    }
}

u8 J_sum(u8 sum, u8 sec)
{
    static u8 sec_temp = 0;
    static u8 sum_old = 0;

    if (sum_old == sum)
        sec_temp++;
    else
    {
        sum_old = sum;
        sec_temp = 0;
    }

    if (sec_temp >= sec)
    {
        sec_temp = sum_old = 0;
        return 1;
    }
    else
        return 0;
}

u8 J_lukou(void)
{
    if (LOAD_FLAG == 1) // 送药
    {
#if FINDWAY == 0 // 灰度
        if (LEFT_GR == 1)
        {
        }
#endif
    }
    else if (LOAD_FLAG == 0)
    {
    }
}