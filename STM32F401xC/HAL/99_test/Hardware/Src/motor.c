/*
*********************************************************************************************************
*
*	模块名称 : 电机控制
*	文件名称 : moto.c
*	版    本 : V1.0
*	说    明 :
*
*   Copyright (C), 2019-2030, 武汉酷点机器人科技有限公司
*   淘宝店铺地址：https://shop559826635.taobao.com/
*********************************************************************************************************
*/
#include "motor.h"

#define MOTOR_SUM_2
// #define MOTOR_SUM_4

extern uint16_t g_WheelSpeed[4];

#ifdef MOTOR_SUM_4 /*4驱*/
/*
*********************************************************************************************************
*	函 数 名: MotorControl
*	功能说明: 电机运动控制
*	形    参: _ucAngularDirection转向方向,_ucAngularSpeed转向速度, _ucMotionDirection运动方向, _usMotionSpeed运动速度
*	返 回 值: 无
*********************************************************************************************************
*/
void MotorControl(uint8_t _ucAngularDirection, uint16_t _ucAngularSpeed, uint8_t _ucMotionDirection, uint16_t _usMotionSpeed)
{
    uint16_t _usMotorA_Speed = 0;
    uint16_t _usMotorB_Speed = 0;
    uint16_t _usMotorC_Speed = 0;
    uint16_t _usMotorD_Speed = 0;

    uint16_t _usMotorA_PWM = 0;
    uint16_t _usMotorB_PWM = 0;
    uint16_t _usMotorC_PWM = 0;
    uint16_t _usMotorD_PWM = 0;

    uint8_t _ucMotorA_Direction = 0;
    uint8_t _ucMotorB_Direction = 0;
    uint8_t _ucMotorC_Direction = 0;
    uint8_t _ucMotorD_Direction = 0;

    uint16_t _usLinearSpeed = 0; //根据角速度计算的线速度

    if ((_ucAngularSpeed == 0) && (_usMotionSpeed == 0)) //当设定的角速度或者运动速度为0时 对PID的累计误差进行清0
    {
        PID_Param_SetZero();
    }

    if (_usMotionSpeed >= AUTO_SPEED_MAX) //行驶速度限制
    {
        _usMotionSpeed = AUTO_SPEED_MAX;
    }

    if (_ucAngularSpeed == 0) //直线行驶
    {
        if (_ucMotionDirection == 0)
        {
            _ucMotorA_Direction = 0;
            _ucMotorB_Direction = 0;
            _ucMotorC_Direction = 1;
            _ucMotorD_Direction = 1;
        }
        else
        {
            _ucMotorA_Direction = 1;
            _ucMotorB_Direction = 1;
            _ucMotorC_Direction = 0;
            _ucMotorD_Direction = 0;
        }
        _usMotorA_Speed = _usMotionSpeed;
        _usMotorB_Speed = _usMotionSpeed;
        _usMotorC_Speed = _usMotionSpeed;
        _usMotorD_Speed = _usMotionSpeed;
    }
    else //非直线行驶
    {
        _usLinearSpeed = (uint16_t)(_ucAngularSpeed * 0.01 * (TIRE_SPACE * TIRE_SPACE + WHEELBASE * WHEELBASE) / (2 * TIRE_SPACE));

        if (_ucAngularDirection == 0) //左转弯
        {
            if (_ucMotionDirection == 0) //前进运动
            {
                _ucMotorC_Direction = 1;
                _ucMotorD_Direction = _ucMotorC_Direction; //同一侧轮子运动方向相同

                _usMotorC_Speed = _usLinearSpeed + _usMotionSpeed;

                _usMotorD_Speed = _usMotorC_Speed; //同一侧轮子运动速度相等

                if (_usMotionSpeed >= _usLinearSpeed)
                {
                    _ucMotorA_Direction = 0;
                    _ucMotorB_Direction = _ucMotorA_Direction;
                    _usMotorA_Speed = _usMotionSpeed - _usLinearSpeed;
                    _usMotorB_Speed = _usMotorA_Speed;
                }
                else
                {
                    _ucMotorA_Direction = 1;
                    _ucMotorB_Direction = _ucMotorA_Direction;
                    _usMotorA_Speed = _usLinearSpeed - _usMotionSpeed;
                    _usMotorB_Speed = _usMotorA_Speed;
                }
            }
            else //倒车运动
            {
                _ucMotorC_Direction = 0;
                _ucMotorD_Direction = _ucMotorC_Direction; //同一侧轮子运动方向相同
                _usMotorC_Speed = _usLinearSpeed + _usMotionSpeed;
                _usMotorD_Speed = _usMotorC_Speed; //同一侧轮子运动速度相等

                if (_usMotionSpeed >= _usLinearSpeed)
                {
                    _ucMotorA_Direction = 1;
                    _ucMotorB_Direction = _ucMotorA_Direction;
                    _usMotorA_Speed = _usMotionSpeed - _usLinearSpeed;
                    _usMotorB_Speed = _usMotorA_Speed;
                }
                else
                {
                    _ucMotorA_Direction = 0;
                    _ucMotorB_Direction = _ucMotorA_Direction;
                    _usMotorA_Speed = _usLinearSpeed - _usMotionSpeed;
                    _usMotorB_Speed = _usMotorA_Speed;
                }
            }
        }
        else //右转弯
        {
            if (_ucMotionDirection == 0) //前进运动
            {
                _ucMotorA_Direction = 0;
                _ucMotorB_Direction = _ucMotorA_Direction; //同一侧轮子运动方向相同
                _usMotorA_Speed = _usLinearSpeed + _usMotionSpeed;
                _usMotorB_Speed = _usMotorA_Speed; //同一侧轮子运动速度相等
                if (_usMotionSpeed >= _usLinearSpeed)
                {
                    _ucMotorC_Direction = 1;
                    _ucMotorD_Direction = _ucMotorC_Direction;
                    _usMotorC_Speed = _usMotionSpeed - _usLinearSpeed;
                    _usMotorD_Speed = _usMotorC_Speed;
                }
                else
                {
                    _ucMotorC_Direction = 0;
                    _ucMotorD_Direction = _ucMotorC_Direction;
                    _usMotorC_Speed = _usLinearSpeed - _usMotionSpeed;
                    _usMotorD_Speed = _usMotorC_Speed;
                }
            }
            else //倒车运动
            {
                _ucMotorA_Direction = 1;
                _ucMotorB_Direction = _ucMotorA_Direction; //同一侧轮子运动方向相同
                _usMotorA_Speed = _usLinearSpeed + _usMotionSpeed;
                _usMotorB_Speed = _usMotorA_Speed; //同一侧轮子运动速度相等

                if (_usMotionSpeed >= _usLinearSpeed)
                {
                    _ucMotorC_Direction = 0;
                    _ucMotorD_Direction = _ucMotorC_Direction;
                    _usMotorC_Speed = _usMotionSpeed - _usLinearSpeed;
                    _usMotorD_Speed = _usMotorC_Speed;
                }
                else
                {
                    _ucMotorC_Direction = 1;
                    _ucMotorD_Direction = _ucMotorC_Direction;
                    _usMotorC_Speed = _usLinearSpeed - _usMotionSpeed;
                    _usMotorD_Speed = _usMotorC_Speed;
                }
            }
        }
    }
    _usMotorA_PWM = wheelSpeedPidCalc(0, g_WheelSpeed[0], _usMotorA_Speed); //执行PID计算
    _usMotorB_PWM = _usMotorA_PWM;                                          //同一侧轮子速度相等
    _usMotorC_PWM = wheelSpeedPidCalc(2, g_WheelSpeed[2], _usMotorC_Speed); //执行PID计算
    _usMotorD_PWM = _usMotorC_PWM;                                          //同一侧轮子速度相等
    printf("%d   %d   %d  %d   %d   %d   %d  %d   %d   %d   %d  %d\r\n", _usMotorA_Speed, _usMotorB_Speed, _usMotorC_Speed, _usMotorD_Speed, _usMotorA_PWM, _usMotorB_PWM, _usMotorC_PWM, _usMotorD_PWM, g_WheelSpeed[0], g_WheelSpeed[1], g_WheelSpeed[2], g_WheelSpeed[3]);

    //电机速度目标速度为0时，设置对应PWM=0
    if (_usMotorA_Speed == 0)
    {
        _usMotorA_PWM = 0;
    }
    if (_usMotorB_Speed == 0)
    {
        _usMotorB_PWM = 0;
    }
    if (_usMotorC_Speed == 0)
    {
        _usMotorC_PWM = 0;
    }
    if (_usMotorD_Speed == 0)
    {
        _usMotorD_PWM = 0;
    }

    //根据PID计算的PWM值驱动电机
    MotoASetSpeed(_ucMotorA_Direction, _usMotorA_PWM);
    MotoBSetSpeed(_ucMotorB_Direction, _usMotorB_PWM);
    MotoCSetSpeed(_ucMotorC_Direction, _usMotorC_PWM);
    MotoDSetSpeed(_ucMotorD_Direction, _usMotorD_PWM);
}
/*
*********************************************************************************************************
*	函 数 名: MotorControl_Ps2
*	功能说明: 手柄数据转换为实际运动速度
*	形    参: _Ps2PadXValue手柄X方向的值 _Ps2PadYValue手柄Y方向的值
*	返 回 值: 无
*********************************************************************************************************
*/
void MotorControl_Ps2(uint8_t _Ps2PadXValue, uint8_t _Ps2PadYValue)
{
    uint8_t _ps2AngularDirection = 0;
    uint16_t _ps2AngularSpeed = 0;
    uint8_t _ps2MotionDirection = 0;
    uint16_t _ps2MotionSpeed = 0;

    if (_Ps2PadXValue <= 138 & _Ps2PadXValue >= 118) //直线行驶
    {

        _ps2AngularDirection = 0;
        _ps2AngularSpeed = 0;
    }
    if (_Ps2PadXValue < 118) //左转弯行驶
    {
        _ps2AngularDirection = 0;

        _ps2AngularSpeed = (uint8_t)((127 - _Ps2PadXValue) * ANGULAR_SPEED_MAX / 127);
    }
    if (_Ps2PadXValue > 138) //右转弯行驶
    {
        _ps2AngularDirection = 1;
        _ps2AngularSpeed = (_Ps2PadXValue - 127) * ANGULAR_SPEED_MAX / 127;
    }
    if (_Ps2PadYValue <= 138 & _Ps2PadYValue >= 118) //直线行驶
    {

        _ps2MotionDirection = 0;
        _ps2MotionSpeed = 0;
    }
    if (_Ps2PadYValue < 118) //前进
    {
        _ps2MotionDirection = 0;

        _ps2MotionSpeed = (uint8_t)((127 - _Ps2PadYValue) * PS2_SPEED_MAX / 127);
    }
    if (_Ps2PadYValue > 138) //后退
    {
        _ps2MotionDirection = 1;

        _ps2MotionSpeed = (uint8_t)((_Ps2PadYValue - 127) * PS2_SPEED_MAX / 127);
    }
    MotorControl(_ps2AngularDirection, _ps2AngularSpeed, _ps2MotionDirection, _ps2MotionSpeed);
}

#elif defined(MOTOR_SUM_2)
/*
*********************************************************************************************************
*	函 数 名: MotorControl
*	功能说明: 电机运动控制
*	形    参: _ucAngularDirection转向方向,_ucAngularSpeed转向速度, _ucMotionDirection运动方向, _usMotionSpeed运动速度
*	返 回 值: 无
*********************************************************************************************************
*/
void MotorControl(uint8_t _ucAngularDirection, uint16_t _ucAngularSpeed, uint8_t _ucMotionDirection, uint16_t _usMotionSpeed)
{
    uint16_t _usMotorA_Speed = 0;
    // uint16_t _usMotorB_Speed = 0;
    uint16_t _usMotorC_Speed = 0;
    // uint16_t _usMotorD_Speed = 0;

    uint16_t _usMotorA_PWM = 0;
    // uint16_t _usMotorB_PWM = 0;
    uint16_t _usMotorC_PWM = 0;
    // uint16_t _usMotorD_PWM = 0;

    uint8_t _ucMotorA_Direction = 0;
    // uint8_t _ucMotorB_Direction = 0;
    uint8_t _ucMotorC_Direction = 0;
    // uint8_t _ucMotorD_Direction = 0;

    uint16_t _usLinearSpeed = 0; //根据角速度计算的线速度

    if ((_ucAngularSpeed == 0) && (_usMotionSpeed == 0)) //当设定的角速度或者运动速度为0时 对PID的累计误差进行清0
    {
        PID_Param_SetZero();
    }

    if (_usMotionSpeed >= AUTO_SPEED_MAX) //行驶速度限制
    {
        _usMotionSpeed = AUTO_SPEED_MAX;
    }

    if (_ucAngularSpeed == 0) //直线行驶
    {
        if (_ucMotionDirection == 0)
        {
            _ucMotorA_Direction = 0;
            // _ucMotorB_Direction = 0;
            _ucMotorC_Direction = 1;
            // _ucMotorD_Direction = 1;
        }
        else
        {
            _ucMotorA_Direction = 1;
            // _ucMotorB_Direction = 1;
            _ucMotorC_Direction = 0;
            // _ucMotorD_Direction = 0;
        }
        _usMotorA_Speed = _usMotionSpeed;
        // _usMotorB_Speed = _usMotionSpeed;
        _usMotorC_Speed = _usMotionSpeed;
        // _usMotorD_Speed = _usMotionSpeed;
    }
    else //非直线行驶
    {
        _usLinearSpeed = (uint16_t)(_ucAngularSpeed * 0.01 * (TIRE_SPACE * TIRE_SPACE + WHEELBASE * WHEELBASE) / (2 * TIRE_SPACE));

        if (_ucAngularDirection == 0) //左转弯
        {
            if (_ucMotionDirection == 0) //前进运动
            {
                _ucMotorC_Direction = 1;
                // _ucMotorD_Direction = _ucMotorC_Direction; //同一侧轮子运动方向相同

                _usMotorC_Speed = _usLinearSpeed + _usMotionSpeed;

                // _usMotorD_Speed = _usMotorC_Speed; //同一侧轮子运动速度相等

                if (_usMotionSpeed >= _usLinearSpeed)
                {
                    _ucMotorA_Direction = 0;
                    // _ucMotorB_Direction = _ucMotorA_Direction;
                    _usMotorA_Speed = _usMotionSpeed - _usLinearSpeed;
                    // _usMotorB_Speed = _usMotorA_Speed;
                }
                else
                {
                    _ucMotorA_Direction = 1;
                    // _ucMotorB_Direction = _ucMotorA_Direction;
                    _usMotorA_Speed = _usLinearSpeed - _usMotionSpeed;
                    // _usMotorB_Speed = _usMotorA_Speed;
                }
            }
            else //倒车运动
            {
                _ucMotorC_Direction = 0;
                // _ucMotorD_Direction = _ucMotorC_Direction; //同一侧轮子运动方向相同
                _usMotorC_Speed = _usLinearSpeed + _usMotionSpeed;
                // _usMotorD_Speed = _usMotorC_Speed; //同一侧轮子运动速度相等

                if (_usMotionSpeed >= _usLinearSpeed)
                {
                    _ucMotorA_Direction = 1;
                    // _ucMotorB_Direction = _ucMotorA_Direction;
                    _usMotorA_Speed = _usMotionSpeed - _usLinearSpeed;
                    // _usMotorB_Speed = _usMotorA_Speed;
                }
                else
                {
                    _ucMotorA_Direction = 0;
                    // _ucMotorB_Direction = _ucMotorA_Direction;
                    _usMotorA_Speed = _usLinearSpeed - _usMotionSpeed;
                    // _usMotorB_Speed = _usMotorA_Speed;
                }
            }
        }
        else //右转弯
        {
            if (_ucMotionDirection == 0) //前进运动
            {
                _ucMotorA_Direction = 0;
                // _ucMotorB_Direction = _ucMotorA_Direction; //同一侧轮子运动方向相同
                _usMotorA_Speed = _usLinearSpeed + _usMotionSpeed;
                // _usMotorB_Speed = _usMotorA_Speed; //同一侧轮子运动速度相等
                if (_usMotionSpeed >= _usLinearSpeed)
                {
                    _ucMotorC_Direction = 1;
                    // _ucMotorD_Direction = _ucMotorC_Direction;
                    _usMotorC_Speed = _usMotionSpeed - _usLinearSpeed;
                    // _usMotorD_Speed = _usMotorC_Speed;
                }
                else
                {
                    _ucMotorC_Direction = 0;
                    // _ucMotorD_Direction = _ucMotorC_Direction;
                    _usMotorC_Speed = _usLinearSpeed - _usMotionSpeed;
                    // _usMotorD_Speed = _usMotorC_Speed;
                }
            }
            else //倒车运动
            {
                _ucMotorA_Direction = 1;
                // _ucMotorB_Direction = _ucMotorA_Direction; //同一侧轮子运动方向相同
                _usMotorA_Speed = _usLinearSpeed + _usMotionSpeed;
                // _usMotorB_Speed = _usMotorA_Speed; //同一侧轮子运动速度相等

                if (_usMotionSpeed >= _usLinearSpeed)
                {
                    _ucMotorC_Direction = 0;
                    // _ucMotorD_Direction = _ucMotorC_Direction;
                    _usMotorC_Speed = _usMotionSpeed - _usLinearSpeed;
                    // _usMotorD_Speed = _usMotorC_Speed;
                }
                else
                {
                    _ucMotorC_Direction = 1;
                    // _ucMotorD_Direction = _ucMotorC_Direction;
                    _usMotorC_Speed = _usLinearSpeed - _usMotionSpeed;
                    // _usMotorD_Speed = _usMotorC_Speed;
                }
            }
        }
    }
    _usMotorA_PWM = wheelSpeedPidCalc(0, g_WheelSpeed[0], _usMotorA_Speed); //执行PID计算
    // _usMotorB_PWM = _usMotorA_PWM;                                          //同一侧轮子速度相等
    _usMotorC_PWM = wheelSpeedPidCalc(2, g_WheelSpeed[2], _usMotorC_Speed); //执行PID计算
    // _usMotorD_PWM = _usMotorC_PWM;                                          //同一侧轮子速度相等
    // printf("%d   %d   %d  %d   %d   %d   %d  %d   %d   %d   %d  %d\r\n", _usMotorA_Speed, _usMotorB_Speed, _usMotorC_Speed, _usMotorD_Speed, _usMotorA_PWM, _usMotorB_PWM, _usMotorC_PWM, _usMotorD_PWM, g_WheelSpeed[0], g_WheelSpeed[1], g_WheelSpeed[2], g_WheelSpeed[3]);

    //电机速度目标速度为0时，设置对应PWM=0
    if (_usMotorA_Speed == 0)
    {
        _usMotorA_PWM = 0;
    }
    // if (_usMotorB_Speed == 0)
    // {
    //     _usMotorB_PWM = 0;
    // }
    if (_usMotorC_Speed == 0)
    {
        _usMotorC_PWM = 0;
    }
    // if (_usMotorD_Speed == 0)
    // {
    //     _usMotorD_PWM = 0;
    // }

    //根据PID计算的PWM值驱动电机
    MotoASetSpeed(_ucMotorA_Direction, _usMotorA_PWM);
    // MotoBSetSpeed(_ucMotorB_Direction, _usMotorB_PWM);
    MotoCSetSpeed(_ucMotorC_Direction, _usMotorC_PWM);
    // MotoDSetSpeed(_ucMotorD_Direction, _usMotorD_PWM);
}
/*
*********************************************************************************************************
*	函 数 名: MotorControl_Ps2
*	功能说明: 手柄数据转换为实际运动速度
*	形    参: _Ps2PadXValue手柄X方向的值 _Ps2PadYValue手柄Y方向的值
*	返 回 值: 无
*********************************************************************************************************
*/
void MotorControl_Ps2(uint8_t _Ps2PadXValue, uint8_t _Ps2PadYValue)
{
    uint8_t _ps2AngularDirection = 0;
    uint16_t _ps2AngularSpeed = 0;
    uint8_t _ps2MotionDirection = 0;
    uint16_t _ps2MotionSpeed = 0;

    if (_Ps2PadXValue <= 138 & _Ps2PadXValue >= 118) //直线行驶
    {

        _ps2AngularDirection = 0;
        _ps2AngularSpeed = 0;
    }
    if (_Ps2PadXValue < 118) //左转弯行驶
    {
        _ps2AngularDirection = 0;

        _ps2AngularSpeed = (uint8_t)((127 - _Ps2PadXValue) * ANGULAR_SPEED_MAX / 127);
    }
    if (_Ps2PadXValue > 138) //右转弯行驶
    {
        _ps2AngularDirection = 1;
        _ps2AngularSpeed = (_Ps2PadXValue - 127) * ANGULAR_SPEED_MAX / 127;
    }
    if (_Ps2PadYValue <= 138 & _Ps2PadYValue >= 118) //直线行驶
    {

        _ps2MotionDirection = 0;
        _ps2MotionSpeed = 0;
    }
    if (_Ps2PadYValue < 118) //前进
    {
        _ps2MotionDirection = 0;

        _ps2MotionSpeed = (uint8_t)((127 - _Ps2PadYValue) * PS2_SPEED_MAX / 127);
    }
    if (_Ps2PadYValue > 138) //后退
    {
        _ps2MotionDirection = 1;

        _ps2MotionSpeed = (uint8_t)((_Ps2PadYValue - 127) * PS2_SPEED_MAX / 127);
    }
    MotorControl(_ps2AngularDirection, _ps2AngularSpeed, _ps2MotionDirection, _ps2MotionSpeed);
}

#endif
