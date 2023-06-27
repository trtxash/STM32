#ifndef _POSITIONAL_PID_H
#define _POSITIONAL_PID_H

#include "stdint.h"

typedef enum
{
    PID_DISABLE, /* PID失能 */
    PID_ENABLE,  /* PID失能 */
} positional_pid_status;

typedef struct _positional_pid_params_t
{

    char control;

    float kp;
    float ki;
    float kd;

    float target;
    float measure;
    float last_measure;

    float error;
    float last_error;

    float p_out;
    float i_out;
    float d_out;

    float output;
    float last_output;

    float output_max;
    float output_min;

    float integral_limit;

    float dead_zone;

    void (*positional_pid_params_init)(
        struct _positional_pid_params_t *positional_pid, float kp, float ki,
        float kd, float dead_zone, float output_max, float output_min);
    void (*positional_pid_set_value)(
        struct _positional_pid_params_t *positional_pid, float kp, float ki,
        float kd);
    void (*positional_pid_control)(
        struct _positional_pid_params_t *positional_pid,
        positional_pid_status status);
} positional_pid_params_t;

void positional_pid_init(positional_pid_params_t *positional_pid, float kp,
                         float ki, float kd,

                         float dead_zone, float output_max, float output_min);
float positional_pid_compute(positional_pid_params_t *positional_pid,
                             float target, float measure);

void positional_pid_set_value(positional_pid_params_t *positional_pid,
                              float kp, float ki, float kd);

#endif
