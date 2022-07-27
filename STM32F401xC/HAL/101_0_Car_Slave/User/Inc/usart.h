#ifndef _USART_H
#define _USART_H
#include "sys.h"

#define USART_REC_LEN 12 //定义最大接收字节数 12
#define EN_USART1_RX 1   //使能（1）/禁止（0）串口1接收
#define EN_USART6_RX 1   //使能（1）/禁止（0）串口1接收

extern UART_HandleTypeDef UART1_Handler; // UART句柄
extern UART_HandleTypeDef UART6_Handler; // UART句柄
extern u8 USART_RX_BUF[USART_REC_LEN];   //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART_RX_STA;                 //接收状态标记
extern u8 beep;
extern u8 move;
extern u8 bluetooth;
extern double TargetSpeed_1; // 目标速度
extern double TargetSpeed_2; // 目标速度
extern double TargetSpeed;   // 目标和速度
extern double Angle_Target;  // 目标速度

//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
void uart6_init(u32 bound);

#endif
