#include "positional_pid.h"
#include <math.h>

#define ABS(x) ((x > 0) ? x : -x)

static void positional_pid_params_init(positional_pid_params_t *positional_pid, float kp, float ki, float kd,
                                       float iout_max, float dead_zone, float output_max, float output_min)
{
    // 初始化 PID 参数
    positional_pid->kp = kp;
    positional_pid->ki = ki;
    positional_pid->kd = kd;

    // 初始化死区、输出上限和输出下限
    positional_pid->i_out_max = iout_max;
    positional_pid->dead_zone = dead_zone;
    positional_pid->output_max = output_max;
    positional_pid->output_min = output_min;

    // 初始化目标值和输出值
    positional_pid->target = 0;
    positional_pid->output = 0;
}

void positional_pid_set_value(positional_pid_params_t *positional_pid,
                              float kp, float ki, float kd)
{
    // 设置 PID 参数
    positional_pid->kp = kp;
    positional_pid->ki = ki;
    positional_pid->kd = kd;
}

float positional_pid_compute(positional_pid_params_t *positional_pid,
                             float target, float measure)
{
    if (positional_pid->control == PID_ENABLE)
    {
        // 设置目标值和测量值
        positional_pid->target = target;
        positional_pid->measure = measure;

        // 计算误差
        positional_pid->error =
            positional_pid->target - positional_pid->measure;

        if (ABS(positional_pid->error) > positional_pid->dead_zone)
        {
            // 计算比例项
            positional_pid->p_out = positional_pid->kp * positional_pid->error;
            // 计算积分项
            positional_pid->i_out += positional_pid->ki * positional_pid->error;
            // 积分限幅
            if (positional_pid->i_out >= 0)
            {
                if (positional_pid->i_out >= positional_pid->i_out_max)
                    positional_pid->i_out = positional_pid->i_out_max;
            }
            else if (positional_pid->i_out < 0)
            {
                if (positional_pid->i_out <= -positional_pid->i_out_max)
                    positional_pid->i_out = -positional_pid->i_out_max;
            }
        }
        // 计算微分项
        positional_pid->d_out =
            positional_pid->kd * (positional_pid->error - positional_pid->last_error);

        // 计算总输出
        positional_pid->output = positional_pid->p_out + positional_pid->i_out + positional_pid->d_out;

        // 限制输出在输出上限和输出下限之间
        if (positional_pid->output > positional_pid->output_max)
        {
            positional_pid->output = positional_pid->output_max;
        }
        if (positional_pid->output < (positional_pid->output_min))
        {
            positional_pid->output = positional_pid->output_min;
        }

        // 更新上一次测量值、输出值和误差值
        positional_pid->last_measure = positional_pid->measure;
        positional_pid->last_output = positional_pid->output;
        positional_pid->last_error = positional_pid->error;

        return positional_pid->output;
    }
    else
    {
        // 失能后，归零
        positional_pid->last_measure = 0;
        positional_pid->last_output = 0;
        positional_pid->last_error = 0;

        return 0.0f;
    }
}

void positional_pid_control(positional_pid_params_t *positional_pid, positional_pid_status status)
{
    // 控制 PID 的使能状态
    positional_pid->control = status;
}

void positional_pid_init(positional_pid_params_t *positional_pid, float kp, float ki, float kd,
                         float iout_max, float dead_zone, float output_max, float output_min)
{
    // 初始化 PID 控制器
    positional_pid->positional_pid_params_init = positional_pid_params_init;
    positional_pid->positional_pid_set_value = positional_pid_set_value;
    positional_pid->positional_pid_control = positional_pid_control;
    // 调用初始化函数设置参数
    positional_pid->positional_pid_params_init(positional_pid, kp, ki, kd, iout_max, dead_zone, output_max, output_min);
    // 默认使能 PID 控制器
    positional_pid->control = PID_ENABLE;
}
