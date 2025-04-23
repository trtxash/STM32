/*
*********************************************************************************************************
*
*	模块名称 : pid控制
*	文件名称 : pid.c
*	版    本 : V1.0
*	说    明 :
*
*   Copyright (C), 2019-2030, 武汉酷点机器人科技有限公司
*   淘宝店铺地址：https://shop559826635.taobao.com/
*********************************************************************************************************
*/
#include "pid.h"

static Pid_t tPidSpeedA;
// static Pid_t tPidSpeedB;
static Pid_t tPidSpeedC;
// static Pid_t tPidSpeedD;
/*
*********************************************************************************************************
*	函 数 名: PID_Init
*	功能说明: 设置4个电机P、I、D的值
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitPid(void)
{
    tPidSpeedA.P = 0.1;
    tPidSpeedA.I = 0.2;
    tPidSpeedA.D = 0.1;

    tPidSpeedA.PrevError = 0;
    tPidSpeedA.SumError = 0;
    
    
    // tPidSpeedB.P = 0.1;
    // tPidSpeedB.I = 0.2;
    // tPidSpeedB.D = 0.1;

    // tPidSpeedB.PrevError = 0;
    // tPidSpeedB.SumError = 0;

    tPidSpeedC.P = 0.1;
    tPidSpeedC.I = 0.2;
    tPidSpeedC.D = 0.1;
    tPidSpeedC.PrevError = 0;
    tPidSpeedC.SumError = 0;
    
    // tPidSpeedD.P = 0.1;
    // tPidSpeedD.I = 0.2;
    // tPidSpeedD.D = 0.1;
    // tPidSpeedD.PrevError = 0;
    // tPidSpeedD.SumError = 0;
}
/*
*********************************************************************************************************
*	函 数 名: wheelSpeedPidCalc
*	功能说明: 根据PID模型计算电机的pwm值
*	形    参: _ucWheelNum电机编号 _usMeasure 检测值（编码器检测的电机的速度）   _usTarget 目标值（设定的电机的速度）
*	返 回 值: int
*********************************************************************************************************
*/

int wheelSpeedPidCalc(uint8_t _ucWheelNum, uint16_t _usMeasure, uint16_t _usTarget)
{
    float _dError = 0;
    float _error  = 0;
    float _tempP  = 0;
    float _tempI  = 0; 
    float _tempD  = 0; 
    float _tempPID = 0;
    
    Pid_t *_tpid; 
    switch(_ucWheelNum)
    {
        case 0:
          _tpid = &tPidSpeedA;  
        break;
        
        case 1:
        //   _tpid = &tPidSpeedB; 
        break;
        
        case 2:
           _tpid = &tPidSpeedC; 
        break;
        
        case 3:
            // _tpid = &tPidSpeedD;
        break;
    
    }
    
    _error = _usTarget - _usMeasure;  /*计算当前误差*/

    _tempP = _tpid->P * _error;/*计算P项的值*/

    _tpid->SumError += _error;  /*累计误差*/
    
    _tempI = _tpid->I * _tpid->SumError;   /*计算I项的值*/

    if(_tempI > 2000)
    {
        _tpid->SumError = 2000 / _tpid->I;
    }
    if(_tempI < -2000)
    {
        _tpid->SumError = -2000 / _tpid->I;
    }

    _dError = _error - _tpid->PrevError;/*微分误差*/
    
    _tempD  = _tpid->D * _dError;/*计算D项的值*/

    _tpid->PrevError = _error ;
    
    _tempPID = _tempP + _tempI + _tempD;
    
    _tempPID = _tempPID;

    if(_tempPID > 2000)
    {
        _tempPID = 2000;
    }
    if(_tempPID < 0)
    {
        _tempPID  = 0;
    }
    
    return _tempPID;
}



/*
*********************************************************************************************************
*	函 数 名: PID_Param_SetZero
*	功能说明: 对电机的累计误差 PrevError清0
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void PID_Param_SetZero(void)
{

    tPidSpeedA.PrevError = 0;
    tPidSpeedA.SumError = 0;
    
    // tPidSpeedB.PrevError = 0;
    // tPidSpeedB.SumError = 0;
    
    tPidSpeedC.PrevError = 0;
    tPidSpeedC.SumError = 0;
    
    // tPidSpeedD.PrevError = 0;
    // tPidSpeedD.SumError = 0;
}