#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "sys.h"


#define PS2_SPEEDPULSE_ZERO 1000
#define PS2_SPEEDPULSE_MAX 1600
#define AUTO_SPEED_MAX 600
#define PS2_SPEED_MAX 500
#define TIRE_SPACE 200
#define WHEELBASE 185

#define ANGULAR_SPEED_MAX 200//0.01rad/s

void MotorControl_Ps2(uint8_t _Ps2PadXValue,uint8_t  _Ps2PadYValue);
//void MotorControl(uint8_t _ucAngularDirection, uint16_t _ucAngularSpeed, uint8_t _ucMotionDirection, uint16_t _usMotionSpeed);
//void MotorControl_Auto(uint8_t _SteerDirection,uint8_t _SteerAngle,uint8_t _MotionDirection,uint16_t _MotionSpeed);
void MotorControl_Auto(uint8_t _angular_Direction,uint8_t _angular_speed,uint8_t _MotionDirection,uint16_t _MotionSpeed);
void ble_motorControl(uint8_t _angular_Direction,uint8_t _angular_speed,uint8_t _MotionDirection,uint16_t _MotionSpeed);
#endif /*__MOTOR_H__*/