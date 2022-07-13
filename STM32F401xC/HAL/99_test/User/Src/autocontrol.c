/*
*********************************************************************************************************
*
*	模块名称 : 自动驾驶控制
*	文件名称 : autocontol.c
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
*	函 数 名: AutoControl_UartMsg
*	功能说明: 处理从上位机接收到的指令，并进行转向速度控制
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/

void auto_ControlCarCmdHandle()
{
    uint8_t  _ucAngularDirection = 0;
    uint16_t _ucAngularSpeed     = 0;

    uint8_t   _ucMotionDirection = 0;
    uint16_t _ucMotionSpeed      = 0;

    if(g_uartFrame.code == 01)
    {
        _ucMotionDirection  = g_uartFrame.buf[1];
        _ucMotionSpeed      = g_uartFrame.buf[2] * 256 + g_uartFrame.buf[3];
        _ucAngularDirection = g_uartFrame.buf[7];
        _ucAngularSpeed     = g_uartFrame.buf[8] * 256 + g_uartFrame.buf[9];

        MotorControl(_ucAngularDirection, _ucAngularSpeed, _ucMotionDirection, _ucMotionSpeed);
    }

}
/*
*********************************************************************************************************
*	函 数 名: AutoControl_CarStop
*	功能说明: 停车控制
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void auto_ControlCarStop()
{
    auto_epsSteerControl(0, 0);
    MotoASetSpeed(0, 0);
    MotoBSetSpeed(0, 0);
    MotoCSetSpeed(0, 0);
    MotoDSetSpeed(0, 0);
    PID_Param_SetZero();
}




