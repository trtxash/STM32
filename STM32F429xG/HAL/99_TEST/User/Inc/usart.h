#ifndef _USART_H
#define _USART_H

#include "stdio.h"
#include "sys.h"

#define USART_REC_LEN 256 // 定义最大接收字节数 256

extern u8 USART_RX_BUF[USART_REC_LEN];   // 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u8 USART_RX_BUF_C[USART_REC_LEN]; // 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART_RX_STA;                 // 接收状态标记
extern u16 USART_RX_STA_C;               // 接收状态标记

#define RXBUFFERSIZE 1 // 缓存大小

extern u8 aRxBuffer[RXBUFFERSIZE]; // HAL库USART接收Buffer
extern u8 bRxBuffer[RXBUFFERSIZE]; // HAL库USART接收Buffer
extern u8 cRxBuffer[RXBUFFERSIZE]; // HAL库USART接收Buffer

extern UART_HandleTypeDef UART1_Handler; // UART句柄
extern UART_HandleTypeDef UART2_Handler; // UART2句柄
extern UART_HandleTypeDef UART6_Handler; // UART句柄
extern DMA_HandleTypeDef hdma_usart6_tx;
extern DMA_HandleTypeDef hdma_usart6_rx;

extern u32 Error_sum;

void uart_init(u32 bound);
void uart2_init(u32 bound);
void uart6_init(u32 bound);

#endif