/*
*********************************************************************************************************
*
*	模块名称 : 转向舵机控制
*	文件名称 : steercontrol.c
*	版    本 : V1.0
*	说    明 :
*
*   Copyright (C), 2019-2030, 武汉酷点机器人科技有限公司
*   淘宝店铺地址：https://shop559826635.taobao.com/
*********************************************************************************************************
*/
#include "includes.h"

/*
*********************************************************************************************************
*	函 数 名: EpsInit
*	功能说明: 转向系统初始化，使舵机处于90度位置，前轮摆正
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void EpsInit(void)
{
    servoSetPluse(1, EPS_ZEROVALUE);//使舵机处于90度位置，前轮摆正
}

/*
*********************************************************************************************************
*	函 数 名: EpsSteerControl_Ps2
*	功能说明: 手柄遥控转向控制
*	形    参: _Ps2PadXValue 手柄左摇杆X轴方向的值
*	返 回 值: 无
*********************************************************************************************************
*/
void ps2_epsSteerControl(uint8_t _Ps2PadXValue)
{

    uint16_t _pulse = EPS_ZEROVALUE;

    _pulse = EPS_LVALUE + (EPS_RVALUE - EPS_LVALUE) * (_Ps2PadXValue - 0) / 255; //根据手柄行程，转向范围，换算占空比


    servoSetPluse(1, _pulse);
}
/*
*********************************************************************************************************
*	函 数 名: EpsSteerControl_Auto
*	功能说明: 自动驾驶转向控制
*	形    参: _SteerDirection转向方向， _SteerAngle转向角度
*	返 回 值: 无
*********************************************************************************************************
*/
void auto_epsSteerControl(uint8_t _ucSteerDirection, uint8_t _ucSteerAngle)
{
    if(_ucSteerDirection == 0) //左转
    {
        uint16_t _pulse = EPS_ZEROVALUE;

        _pulse = EPS_ZEROVALUE - (EPS_ZEROVALUE - EPS_LVALUE) * _ucSteerAngle / EPS_RANGE;

        servoSetPluse(1, _pulse);

    }
    if(_ucSteerDirection == 1) //右转
    {

        uint16_t _pulse = EPS_ZEROVALUE;

        _pulse = EPS_ZEROVALUE + (EPS_RVALUE - EPS_ZEROVALUE) * _ucSteerAngle / EPS_RANGE;

        servoSetPluse(1, _pulse);

    }

}




