#ifndef _USART_H
#define _USART_H

#include "sys.h"
#include "stdio.h"

#define VALUEPACK 1 // 使能valuepack

#define USART_REC_LEN 256 // 定义最大接收字节数 256

#define EN_USART1_RX 1 // 使能（1）/禁止（0）串口1接收
#define EN_USART3_RX 1 // 使能（1）/禁止（0）串口1接收

extern u8 USART_RX_BUF[USART_REC_LEN]; // 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART_RX_STA;               // 接收状态标记

#define RXBUFFERSIZE 1             // 缓存大小
extern u8 aRxBuffer[RXBUFFERSIZE]; // HAL库USART接收Buffer

extern UART_HandleTypeDef UART1_Handler; // UART句柄
extern UART_HandleTypeDef UART3_Handler; // UART句柄

extern u32 Error_sum;

void uart_init(u32 bound);
void uart3_init(u32 bound);

#endif