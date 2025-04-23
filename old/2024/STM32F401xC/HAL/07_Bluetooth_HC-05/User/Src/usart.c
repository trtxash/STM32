#include "usart.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//如果使用os,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h" //os 使用
#endif

//加入以下代码,支持printf函数,而不需要选择use MicroLIB
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
	int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
	while ((USART1->SR & 0X40) == 0)
		; //循环发送,直到发送完毕
	USART1->DR = (u8)ch;
	return ch;
}
#endif

#if EN_USART1_RX //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
u8 USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
// bit15，	接收完成标志
// bit14，	接收到0x0d
// bit13~0，	接收到的有效字节数目
u16 USART_RX_STA = 0; //接收状态标记

UART_HandleTypeDef UART1_Handler; // UART1句柄
UART_HandleTypeDef UART6_Handler; // UART6句柄

//初始化IO 串口1
// bound:波特率
void uart_init(u32 bound)
{
	// UART 初始化设置
	UART1_Handler.Instance = USART1;					// USART1
	UART1_Handler.Init.BaudRate = bound;				//波特率
	UART1_Handler.Init.WordLength = UART_WORDLENGTH_8B; //字长为8位数据格式
	UART1_Handler.Init.StopBits = UART_STOPBITS_1;		//一个停止位
	UART1_Handler.Init.Parity = UART_PARITY_NONE;		//无奇偶校验位
	UART1_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; //无硬件流控
	UART1_Handler.Init.Mode = UART_MODE_TX_RX;			//收发模式
	HAL_UART_Init(&UART1_Handler);						// HAL_UART_Init()会使能UART1

	__HAL_UART_ENABLE_IT(&UART1_Handler, UART_IT_RXNE); //使能接收中断
}

//初始化IO 串口6
// bound:波特率
void uart6_init(u32 bound)
{
	// UART 初始化设置
	UART6_Handler.Instance = USART6;					// USART6
	UART6_Handler.Init.BaudRate = bound;				//波特率
	UART6_Handler.Init.WordLength = UART_WORDLENGTH_8B; //字长为8位数据格式
	UART6_Handler.Init.StopBits = UART_STOPBITS_1;		//一个停止位
	UART6_Handler.Init.Parity = UART_PARITY_NONE;		//无奇偶校验位
	UART6_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; //无硬件流控
	UART6_Handler.Init.Mode = UART_MODE_TX_RX;			//收发模式
	HAL_UART_Init(&UART6_Handler);						// HAL_UART_Init()会使能UART6

	HAL_UART_Receive_IT(&UART6_Handler, (u8 *)USART_RX_BUF, USART_REC_LEN); //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
}

// UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
// huart:串口句柄
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	// GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;

	if (huart->Instance == USART1) //如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();  //使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE(); //使能USART1时钟

		GPIO_Initure.Pin = GPIO_PIN_9;			  // PA9
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;	  //复用推挽输出
		GPIO_Initure.Pull = GPIO_PULLUP;		  //上拉
		GPIO_Initure.Speed = GPIO_SPEED_FAST;	  //高速
		GPIO_Initure.Alternate = GPIO_AF7_USART1; //复用为USART1
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);	  //初始化PA9

		GPIO_Initure.Pin = GPIO_PIN_10;		 // PA10
		HAL_GPIO_Init(GPIOA, &GPIO_Initure); //初始化PA10

#if EN_USART1_RX
		HAL_NVIC_EnableIRQ(USART1_IRQn);		 //使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn, 2, 1); //抢占优先级2，子优先级1
#endif
	}
	if (huart->Instance == USART6) //如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();  //使能GPIOA时钟
		__HAL_RCC_USART6_CLK_ENABLE(); //使能USART6时钟

		GPIO_Initure.Pin = GPIO_PIN_11;			  // PA11
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;	  //复用推挽输出
		GPIO_Initure.Pull = GPIO_PULLUP;		  //上拉
		GPIO_Initure.Speed = GPIO_SPEED_FAST;	  //高速
		GPIO_Initure.Alternate = GPIO_AF8_USART6; //复用为USART6
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);	  //初始化PA11

		GPIO_Initure.Pin = GPIO_PIN_12;		 // PA12
		HAL_GPIO_Init(GPIOA, &GPIO_Initure); //初始化PA12

#if EN_USART6_RX
		HAL_NVIC_EnableIRQ(USART6_IRQn);		 //使能USART1中断通道
		HAL_NVIC_SetPriority(USART6_IRQn, 1, 1); //抢占优先级1，子优先级1
#endif
	}
}

/*下面代码我们直接把中断控制逻辑写在中断服务函数内部。*/
//串口1中断服务程序
void USART1_IRQHandler(void)
{
	u8 Res;
#if SYSTEM_SUPPORT_OS //使用OS
	OSIntEnter();
#endif
	if ((__HAL_UART_GET_FLAG(&UART1_Handler, UART_FLAG_RXNE) != RESET)) //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		HAL_UART_Receive(&UART1_Handler, &Res, 1, 1000);
		if ((USART_RX_STA & 0x8000) == 0) //接收未完成
		{
			if (USART_RX_STA & 0x4000) //接收到了0x0d
			{
				if (Res != 0x0a)
					USART_RX_STA = 0; //接收错误,重新开始
				else
					USART_RX_STA |= 0x8000; //接收完成了
			}
			else //还没收到0X0D
			{
				if (Res == 0x0d)
					USART_RX_STA |= 0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res;
					USART_RX_STA++;
					if (USART_RX_STA > (USART_REC_LEN - 1))
						USART_RX_STA = 0; //接收数据错误,重新开始接收
				}
			}
		}
	}

	HAL_UART_IRQHandler(&UART1_Handler);

#if SYSTEM_SUPPORT_OS //使用OS
	OSIntExit();
#endif
}

//串口6中断服务程序
void USART6_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS //使用OS
	OSIntEnter();
#endif
	USART_RX_BUF[USART_RX_STA] = USART6->DR; //读取串口数据
	if (USART6->DR == 0XA5)					 //接受到了开始数据，数据包头正确
		USART_RX_STA = 0;					 //接收累加器清零
	USART_RX_STA++;							 //接收累加器加1
	if (USART_RX_STA == USART_REC_LEN)		 //接收完成了
	{
		if (USART_RX_BUF[USART_RX_STA - 1] = 0X5A) // 数据包尾接受正确
		{
			u8 temp;
			USART_RX_STA = 0;
			for (int i = 0; i < USART_REC_LEN - 3; i++)
			{
				temp += USART_RX_BUF[i + 1];
			}
			if (temp == USART_RX_BUF[USART_REC_LEN - 2]) // 校验数据包和
			{
				TargetSpeed_1 = (float)USART_RX_BUF[1] / 100;
				TargetSpeed_2 = (float)USART_RX_BUF[5] / 100;
			}
			else
			{
				USART_RX_STA = 0; // 数据包错误,重新开始接收
			}
		}
		else
			USART_RX_STA = 0;
	}
	__HAL_UART_CLEAR_FLAG(&UART6_Handler, UART_FLAG_RXNE); //清除接收中断标志
#if SYSTEM_SUPPORT_OS									   //使用OS
	OSIntExit();
#endif
}

#endif