#include "usart.h"

UART_HandleTypeDef UART1_Handler; // UART1句柄
UART_HandleTypeDef UART6_Handler; // UART6句柄

u32 Error_sum = 0;

#ifdef __GNUC__
__attribute__((used)) int _write(int fd, char *ptr, int len) // 如果用GNUC库，重定向串口，支持printf函数
{
	int i;
	for (i = 0; i < len; i++)
	{
		USART1->DR = ptr[i];
		while ((USART1->SR & 0x40) == 0)
			;
	}
	return len;
}
#endif

// 注意,读取USARTx->SR能避免莫名其妙的错误
u8 USART_RX_BUF[USART_REC_LEN]; // 接收缓冲,最大USART_REC_LEN个字节.
// 接收状态
// bit15，	接收完成标志
// bit14，	接收到0x0d
// bit13~0，	接收到的有效字节数目
u16 USART_RX_STA = 0; // 接收状态标记

u8 aRxBuffer[RXBUFFERSIZE]; // HAL库使用的串口接收缓冲

// 初始化IO 串口1
//  bound:波特率
void uart_init(u32 bound)
{
	// UART 初始化设置
	UART1_Handler.Instance = USART1;					// USART1
	UART1_Handler.Init.BaudRate = bound;				// 波特率
	UART1_Handler.Init.WordLength = UART_WORDLENGTH_8B; // 字长为8位数据格式
	UART1_Handler.Init.StopBits = UART_STOPBITS_1;		// 一个停止位
	UART1_Handler.Init.Parity = UART_PARITY_NONE;		// 无奇偶校验位
	UART1_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; // 无硬件流控
	UART1_Handler.Init.Mode = UART_MODE_TX_RX;			// 收发模式
	HAL_UART_Init(&UART1_Handler);						// HAL_UART_Init()会使能UART1

	HAL_UART_Receive_IT(&UART1_Handler, aRxBuffer, RXBUFFERSIZE); // 如果要调用处理回调函数，用这个函数使能接收中断
}

// 初始化IO 串口6
//  bound:波特率
void uart6_init(u32 bound)
{
	// UART 初始化设置
	UART6_Handler.Instance = USART6;					// USART6
	UART6_Handler.Init.BaudRate = bound;				// 波特率
	UART6_Handler.Init.WordLength = UART_WORDLENGTH_8B; // 字长为8位数据格式
	UART6_Handler.Init.StopBits = UART_STOPBITS_1;		// 一个停止位
	UART6_Handler.Init.Parity = UART_PARITY_NONE;		// 无奇偶校验位
	UART6_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; // 无硬件流控
	UART6_Handler.Init.Mode = UART_MODE_TX_RX;			// 收发模式
	HAL_UART_Init(&UART6_Handler);						// HAL_UART_Init()会使能UART6

	HAL_UART_Receive_IT(&UART6_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE); // 该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
}

// UART底层初始化，时钟使能，引脚配置，中断配置
// 此函数会被HAL_UART_Init()调用
// huart:串口句柄
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	// GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;

	if (huart->Instance == USART1) // 如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();  // 使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE(); // 使能USART1时钟

		GPIO_Initure.Pin = GPIO_PIN_9;			  // PA9
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;	  // 复用推挽输出
		GPIO_Initure.Pull = GPIO_PULLUP;		  // 上拉
		GPIO_Initure.Speed = GPIO_SPEED_FAST;	  // 高速
		GPIO_Initure.Alternate = GPIO_AF7_USART1; // 复用为USART1
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);	  // 初始化PA9

		GPIO_Initure.Pin = GPIO_PIN_10;		 // PA10
		HAL_GPIO_Init(GPIOA, &GPIO_Initure); // 初始化PA10

#if EN_USART1_RX
		HAL_NVIC_EnableIRQ(USART1_IRQn);		 // 使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn, 7, 0); // 抢占优先级0，子优先级0
#endif
	}
	if (huart->Instance == USART6) // 如果是串口6，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();  // 使能GPIOA时钟
		__HAL_RCC_USART6_CLK_ENABLE(); // 使能USART6时钟

		GPIO_Initure.Pin = GPIO_PIN_6;			  // PA11
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;	  // 复用推挽输出
		GPIO_Initure.Pull = GPIO_PULLUP;		  // 上拉
		GPIO_Initure.Speed = GPIO_SPEED_FAST;	  // 高速
		GPIO_Initure.Alternate = GPIO_AF8_USART6; // 复用为USART6
		HAL_GPIO_Init(GPIOC, &GPIO_Initure);	  // 初始化PA11

		GPIO_Initure.Pin = GPIO_PIN_7;		 // PA12
		HAL_GPIO_Init(GPIOC, &GPIO_Initure); // 初始化PA12

#if EN_USART6_RX
		HAL_NVIC_EnableIRQ(USART6_IRQn);		 // 使能USART1中断通道
		HAL_NVIC_SetPriority(USART6_IRQn, 8, 0); // 抢占优先级1，子优先级1
#endif
	}
}

/*
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);//发送完成回调函数
void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart);//发送完成过半
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);//接收完成回调函数
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart);//接收完成过半
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);//错误处理回调函数
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1) // 如果是串口1
	{
		// aRxBuffer[0] = USART1->DR;		  // 读取可以自动清楚RXNE
		if ((USART_RX_STA & 0x8000) == 0) // 接收未完成
		{
			if (USART_RX_STA & 0x4000) // 接收到了0x0d
			{
				if (aRxBuffer[0] != 0x0a)
					USART_RX_STA = 0; // 接收错误,重新开始
				else
					USART_RX_STA |= 0x8000; // 接收完成了
			}
			else // 还没收到0X0D
			{
				if (aRxBuffer[0] == 0x0d)
					USART_RX_STA |= 0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA & 0X3FFF] = aRxBuffer[0];
					USART_RX_STA++;
					if (USART_RX_STA > (USART_REC_LEN - 1))
					{
						USART_RX_STA = 0; // 接收数据错误,重新开始接收
						Error_sum++;
					}
				}
			}
		}

		HAL_UART_Receive_IT(&UART1_Handler, aRxBuffer, RXBUFFERSIZE); // Receive_IT中会关闭中断，需要重开
	}
	else if (huart->Instance == USART6) // 如果是串口6
	{
	}
}

/*下面代码我们直接把中断控制逻辑写在中断服务函数内部。*/
// 串口1中断服务程序
void USART1_IRQHandler(void)
{
	// if (__HAL_UART_GET_FLAG(&UART1_Handler, UART_FLAG_RXNE))
	// {
	// 	aRxBuffer[0] = USART1->DR;		  // 读取可以自动清楚RXNE
	// 	if ((USART_RX_STA & 0x8000) == 0) // 接收未完成
	// 	{
	// 		if (USART_RX_STA & 0x4000) // 接收到了0x0d
	// 		{
	// 			if (aRxBuffer[0] != 0x0a)
	// 				USART_RX_STA = 0; // 接收错误,重新开始
	// 			else
	// 				USART_RX_STA |= 0x8000; // 接收完成了
	// 		}
	// 		else // 还没收到0X0D
	// 		{
	// 			if (aRxBuffer[0] == 0x0d)
	// 				USART_RX_STA |= 0x4000;
	// 			else
	// 			{
	// 				USART_RX_BUF[USART_RX_STA & 0X3FFF] = aRxBuffer[0];
	// 				USART_RX_STA++;
	// 				if (USART_RX_STA > (USART_REC_LEN - 1))
	// 				{
	// 					USART_RX_STA = 0; // 接收数据错误,重新开始接收
	// 					Error_sum++;
	// 				}
	// 			}
	// 		}
	// 	}
	// }

	HAL_UART_IRQHandler(&UART1_Handler);
}

// 串口6中断服务程序
void USART6_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UART6_Handler);
}