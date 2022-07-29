#ifndef __MECANUMWHEEL_H
#define __MECANUMWHEEL_H

#include "sys.h"
#include "pid.h"

extern short Encoder_target_1;
extern short Encoder_target_2;
extern short Encoder_target_3;
extern short Encoder_target_4;
extern short Encoder_target;

void main_plan(void);
void plan_stop(void);
void plan_qibu(void);

#endif