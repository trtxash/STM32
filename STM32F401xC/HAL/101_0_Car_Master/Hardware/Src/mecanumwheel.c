#include "mecanumwheel.h"

char car_state = -1;   // -2:左转，-1:停止，1:直走，2:右转
u32 TIME_N5ms = 0;     // 5ms计数器
u32 TIME_N5ms_old = 0; // 5ms计数器

/* 任务计划 */
void main_plan(void)
{
    plan_qibu();
    while (TIME_N5ms - TIME_N5ms_old < 400) // 2s
    {
    }
    TIME_N5ms_old = TIME_N5ms;

    plan_v();
    while (TIME_N5ms - TIME_N5ms_old < 600) // 3s
    {
        if (Now_pos_num == 5)
        {
            delay_ms(10);
            plan_stop();
            delay_ms(10);
            break;
        }
    }
    TIME_N5ms_old = TIME_N5ms;

    plan_you();
    while (1)
    {
        if (Now_pos_num == 0)
        {
            plan_stop();
            delay_ms(10);
            break;
        }
    }
    TIME_N5ms_old = TIME_N5ms;

    plan_qibu();
    while (TIME_N5ms - TIME_N5ms_old < 400) // 2s
    {
    }
    TIME_N5ms_old = TIME_N5ms;

    plan_v();
    Encoder_target_1 = Encoder_target_2 = Encoder_target_3 = Encoder_target_4 = Encoder_target = 250;
    while (TIME_N5ms - TIME_N5ms_old < 400) // 2s
    {
        if (Now_pos_num == 5 || Now_pos_num == 9 || Now_pos_num == -5)
        {
            TIM_SetTIM4Compare_n(500, 2);
            TIM_SetTIM4Compare_n(500, 4);
            TIM_SetTIM5Compare_n(500, 1);
            TIM_SetTIM5Compare_n(500, 3);
            plan_stop();
            delay_ms(10);
            break;
        }
    }
    TIME_N5ms_old = TIME_N5ms;

    while (1)
    {
    }
}

/* 停止 */
void plan_stop(void)
{
    car_state = -1; // 停止
}

/* 起步 */
void plan_qibu(void)
{
    car_state = 1; // 直走
    Encoder_target_1 = Encoder_target_2 = Encoder_target_3 = Encoder_target_4 = Encoder_target = 1000;
}

/* 匀速 */
void plan_v(void)
{
    car_state = 1; // 直走
    Encoder_target_1 = Encoder_target_2 = Encoder_target_3 = Encoder_target_4 = Encoder_target = 500;
}

/* 右转 */
void plan_you(void)
{
    car_state = 2; // 右转
}