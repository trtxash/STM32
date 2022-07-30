#include "mecanumwheel.h"

char car_state = -1;   // -2:左转，-1:停止，1:直走，2:右转
u32 TIME_N5ms = 0;     // 5ms计数器
u32 TIME_N5ms_old = 0; // 5ms计数器

/* 任务计划 */
void main_plan(void)
{
    plan_qibu();
    while (TIME_N5ms - TIME_N5ms_old < 300) // 1.5s
    {
    }
    TIME_N5ms_old = TIME_N5ms;
    plan_you();
    while (TIME_N5ms - TIME_N5ms_old < 200) // 1s
    {
    }
    TIME_N5ms_old = TIME_N5ms;
    while (1)
    {
        plan_stop();
    }
}

/* 停止 */
void plan_stop(void)
{
    car_state = -1; // 停止
    Encoder_target = 0;
    Encoder_target_1 = 0;
    Encoder_target_2 = 0;
    Encoder_target_3 = 0;
    Encoder_target_4 = 0;
}

/* 起步 */
void plan_qibu(void)
{
    car_state = 1; // 直走
    Encoder_target_1 = Encoder_target_2 = Encoder_target_3 = Encoder_target_4 = Encoder_target = 1500;
}

/* 右转 */
void plan_you(void)
{
    car_state = 2; // 右转
    Encoder_target_1 = Encoder_target_2 = Encoder_target_3 = Encoder_target_4 = Encoder_target = 500;
}