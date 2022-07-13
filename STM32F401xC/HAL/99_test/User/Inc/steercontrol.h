#ifndef __STERCONTROL_H__
#define __STERCONTROL_H__

#include "bsp.h"

#define EPS_ZEROVALUE 1500
#define EPS_LVALUE 800
#define EPS_RVALUE 2200

#define EPS_RANGE 25

void EpsInit(void);
void ps2_epsSteerControl(uint8_t _Ps2PadXValue);
void auto_epsSteerControl(uint8_t _SteerDirection,uint8_t _SteerAngle);

#endif /*__STERCONTROL_H__*/