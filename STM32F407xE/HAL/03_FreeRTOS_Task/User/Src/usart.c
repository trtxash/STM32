#include "usart.h"

//加入以下代码,支持printf函数,而不需要选择use MicroLIB
// #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#if 0
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

// 发送数据包的字节长度
#define TXPACK_BYTE_SIZE ((TX_BOOL_NUM + 7) >> 3) + TX_BYTE_NUM + (TX_SHORT_NUM << 1) + (TX_INT_NUM << 2) + (TX_FLOAT_NUM << 2)
// 接收数据包的字节长度
#define RXPACK_BYTE_SIZE ((RX_BOOL_NUM + 7) >> 3) + RX_BYTE_NUM + (RX_SHORT_NUM << 1) + (RX_INT_NUM << 2) + (RX_FLOAT_NUM << 2)
// 接收数据包的原数据加上包头、校验和包尾 之后的字节长度
unsigned short rx_pack_length = RXPACK_BYTE_SIZE + 3;
// 接收计数-记录当前的数据接收进度
// 接收计数每次随串口的接收中断后 +1
long rxIndex = 0;
// 读取计数-记录当前的数据包读取进度，读取计数会一直落后于接收计数，当读取计数与接收计数之间距离超过一个接收数据包的长度时，会启动一次数据包的读取。
// 读取计数每次在读取数据包后增加 +(数据包长度)
long rdIndex = 0;
// 用于环形缓冲区的数组，环形缓冲区的大小可以在.h文件中定义VALUEPACK_BUFFER_SIZE
unsigned char vp_rxbuff[VALUEPACK_BUFFER_SIZE];
// 用于暂存发送数据的数组
unsigned char vp_txbuff[TXPACK_BYTE_SIZE + 3];
// 数据包环形缓冲区计数
unsigned int vp_circle_rx_index;

// 数据读取涉及到的变量
unsigned short rdi, rdii, idl, idi, bool_index, bool_bit;
// 变量地址
uint32_t idc;
// 记录读取的错误字节的次数
unsigned int err = 0;
// 用于和校验
unsigned char sum = 0;
// 存放数据包读取的结果
unsigned char isok;

UART_HandleTypeDef UART1_Handler; // UART1句柄
UART_HandleTypeDef UART6_Handler; // UART6句柄

#ifdef __GNUC__
__attribute__((used)) int _write(int fd, char *ptr, int len)
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
// int _write(int fd, char *ptr, int len) // 重定向 _write() 函数
// {
// 	HAL_UART_Transmit(&UART1_Handler, (uint8_t *)ptr, len, 0xFFFF);
// 	return len;
// }
#endif

//------------------------------------------------------------------------------------------------------------------------
// unsigned char readValuePack(RxPack *rx_pack_ptr)
// 尝试从缓冲区中读取数据包
// 参数   - RxPack *rx_pack_ptr： 传入接收数据结构体的指针，从环形缓冲区中读取出数据包，并将各类数据存储到rx_pack_ptr指向的结构体中
// 返回值 - 如果成功读取到数据包，则返回1，否则返回0
unsigned char readValuePack(RxPack *rx_pack_ptr)
{
	isok = 0;

	// 确保读取计数和接收计数之间的距离小于2个数据包的长度
	while (rdIndex < (rxIndex - ((rx_pack_length)*2)))
	{
		rdIndex += rx_pack_length;
	}

	// 如果读取计数落后于接收计数超过 1个 数据包的长度，则尝试读取
	while (rdIndex <= (rxIndex - rx_pack_length))
	{
		rdi = rdIndex % VALUEPACK_BUFFER_SIZE;
		rdii = rdi + 1;
		if (vp_rxbuff[rdi] == PACK_HEAD) // 比较包头
		{
			if (vp_rxbuff[(rdi + RXPACK_BYTE_SIZE + 2) % VALUEPACK_BUFFER_SIZE] == PACK_TAIL) // 比较包尾 确定包尾后，再计算校验和
			{
				//  计算校验和
				sum = 0;
				for (short s = 0; s < RXPACK_BYTE_SIZE; s++)
				{
					rdi++;
					if (rdi >= VALUEPACK_BUFFER_SIZE)
						rdi -= VALUEPACK_BUFFER_SIZE;
					sum += vp_rxbuff[rdi];
				}
				rdi++;
				if (rdi >= VALUEPACK_BUFFER_SIZE)
					rdi -= VALUEPACK_BUFFER_SIZE;

				if (sum == vp_rxbuff[rdi]) // 校验和正确，则开始将缓冲区中的数据读取出来
				{
//  提取数据包数据 一共有五步， bool byte short int float
// 1. bool
#if RX_BOOL_NUM > 0

					idc = (uint32_t)rx_pack_ptr->bools;
					idl = (RX_BOOL_NUM + 7) >> 3;

					bool_bit = 0;
					for (bool_index = 0; bool_index < RX_BOOL_NUM; bool_index++)
					{
						*((unsigned char *)(idc + bool_index)) = (vp_rxbuff[rdii] & (0x01 << bool_bit)) ? 1 : 0;
						bool_bit++;
						if (bool_bit >= 8)
						{
							bool_bit = 0;
							rdii++;
						}
					}
					if (bool_bit)
						rdii++;

#endif
// 2.byte
#if RX_BYTE_NUM > 0
					idc = (uint32_t)(rx_pack_ptr->bytes);
					idl = RX_BYTE_NUM;
					for (idi = 0; idi < idl; idi++)
					{
						if (rdii >= VALUEPACK_BUFFER_SIZE)
							rdii -= VALUEPACK_BUFFER_SIZE;
						(*((unsigned char *)idc)) = vp_rxbuff[rdii];
						rdii++;
						idc++;
					}
#endif
// 3.short
#if RX_SHORT_NUM > 0
					idc = (uint32_t)(rx_pack_ptr->shorts);
					idl = RX_SHORT_NUM << 1;
					for (idi = 0; idi < idl; idi++)
					{
						if (rdii >= VALUEPACK_BUFFER_SIZE)
							rdii -= VALUEPACK_BUFFER_SIZE;
						(*((unsigned char *)idc)) = vp_rxbuff[rdii];
						rdii++;
						idc++;
					}
#endif
// 4.int
#if RX_INT_NUM > 0
					idc = (uint32_t)(&(rx_pack_ptr->integers[0]));
					idl = RX_INT_NUM << 2;
					for (idi = 0; idi < idl; idi++)
					{
						if (rdii >= VALUEPACK_BUFFER_SIZE)
							rdii -= VALUEPACK_BUFFER_SIZE;
						(*((unsigned char *)idc)) = vp_rxbuff[rdii];
						rdii++;
						idc++;
					}
#endif
// 5.float
#if RX_FLOAT_NUM > 0
					idc = (uint32_t)(&(rx_pack_ptr->floats[0]));
					idl = RX_FLOAT_NUM << 2;
					for (idi = 0; idi < idl; idi++)
					{
						if (rdii >= VALUEPACK_BUFFER_SIZE)
							rdii -= VALUEPACK_BUFFER_SIZE;
						(*((unsigned char *)idc)) = vp_rxbuff[rdii];
						rdii++;
						idc++;
					}
#endif

					// 更新读取计数
					rdIndex += rx_pack_length;
					isok = 1;
				}
				else
				{
					// 校验值错误 则 err+1 且 更新读取计数
					rdIndex++;
					err++;
				}
			}
			else
			{
				// 包尾错误 则 err+1 且 更新读取计数
				rdIndex++;
				err++;
			}
		}
		else
		{
			// 包头错误 则 err+1 且 更新读取计数
			rdIndex++;
			err++;
		}
	}
	return isok;
}

//-------------------------------------------------------------------------------------------------------------------------
// void sendBuffer(unsigned char *p,unsigned short length)
// 发送数据包
// 传入指针 和 字节长度

void sendBuffer(unsigned char *p, unsigned short length)
{
	// for (int i = 0; i < length; i++)
	// {
	// 	USART_SendData(USART1, *p++);
	// 	while (__HAL_UART_GET_IT_SOURCE(&UART1_Handler, UART_FLAG_TXE) == RESET)
	// 	{
	// 	}
	// }
	for (u8 i = 0; i < length; i++)
	{
		USART1->DR = *p++;
		while ((USART1->SR & 0X40) == 0) // 等待接收完成
		{
		}
	}
}

// 数据包发送涉及的变量
unsigned short loop;
unsigned char valuepack_tx_bit_index;
unsigned char valuepack_tx_index;

//---------------------------------------------------------------------------------------------------------
//  void sendValuePack(TxPack *tx_pack_ptr)
//  将发送数据结构体中的变量打包，并发送出去
//  传入参数- TxPack *tx_pack_ptr 待发送数据包的指针
//
//  先将待发送数据包结构体的变量转移到“发送数据缓冲区”中，然后将发送数据缓冲区中的数据发送
void sendValuePack(TxPack *tx_pack_ptr)
{
	int i;
	vp_txbuff[0] = 0xa5;
	sum = 0;
	//  由于结构体中不同类型的变量在内存空间的排布不是严格对齐的，中间嵌有无效字节，因此需要特殊处理
	valuepack_tx_bit_index = 0;
	valuepack_tx_index = 1;

#if TX_BOOL_NUM > 0
	for (loop = 0; loop < TX_BOOL_NUM; loop++)
	{
		if (tx_pack_ptr->bools[loop])
			vp_txbuff[valuepack_tx_index] |= 0x01 << valuepack_tx_bit_index;
		else
			vp_txbuff[valuepack_tx_index] &= ~(0x01 << valuepack_tx_bit_index);
		valuepack_tx_bit_index++;

		if (valuepack_tx_bit_index >= 8)
		{
			valuepack_tx_bit_index = 0;
			valuepack_tx_index++;
		}
	}
	if (valuepack_tx_bit_index != 0)
		valuepack_tx_index++;
#endif
#if TX_BYTE_NUM > 0

	for (loop = 0; loop < TX_BYTE_NUM; loop++)
	{
		vp_txbuff[valuepack_tx_index] = tx_pack_ptr->bytes[loop];
		valuepack_tx_index++;
	}
#endif

#if TX_SHORT_NUM > 0
	for (loop = 0; loop < TX_SHORT_NUM; loop++)
	{
		vp_txbuff[valuepack_tx_index] = tx_pack_ptr->shorts[loop] & 0xff;
		vp_txbuff[valuepack_tx_index + 1] = tx_pack_ptr->shorts[loop] >> 8;
		valuepack_tx_index += 2;
	}
#endif

#if TX_INT_NUM > 0
	for (loop = 0; loop < TX_INT_NUM; loop++)
	{
		i = tx_pack_ptr->integers[loop];
		vp_txbuff[valuepack_tx_index] = i & 0xff;
		vp_txbuff[valuepack_tx_index + 1] = (i >> 8) & 0xff;
		vp_txbuff[valuepack_tx_index + 2] = (i >> 16) & 0xff;
		vp_txbuff[valuepack_tx_index + 3] = (i >> 24) & 0xff;
		valuepack_tx_index += 4;
	}
#endif

#if TX_FLOAT_NUM > 0
	for (loop = 0; loop < TX_FLOAT_NUM; loop++)
	{
		i = *(int *)(&(tx_pack_ptr->floats[loop]));
		vp_txbuff[valuepack_tx_index] = i & 0xff;
		vp_txbuff[valuepack_tx_index + 1] = (i >> 8) & 0xff;
		vp_txbuff[valuepack_tx_index + 2] = (i >> 16) & 0xff;
		vp_txbuff[valuepack_tx_index + 3] = (i >> 24) & 0xff;
		valuepack_tx_index += 4;
	}
#endif

	for (unsigned short d = 1; d <= TXPACK_BYTE_SIZE; d++)
		sum += vp_txbuff[d];

	vp_txbuff[TXPACK_BYTE_SIZE + 1] = sum;
	vp_txbuff[TXPACK_BYTE_SIZE + 2] = 0x5a;
	sendBuffer(vp_txbuff, TXPACK_BYTE_SIZE + 3);
}

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

	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)vp_rxbuff, VALUEPACK_BUFFER_SIZE); //使能接收中断
																				 // HAL_UART_Transmit_IT(&UART1_Handler, (u8 *)vp_rxbuff, VALUEPACK_BUFFER_SIZE); //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
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

	HAL_UART_Receive_IT(&UART6_Handler, (u8 *)vp_rxbuff, VALUEPACK_BUFFER_SIZE); //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
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
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0); //抢占优先级0，子优先级0
#endif
	}
	if (huart->Instance == USART6) //如果是串口6，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();  //使能GPIOA时钟
		__HAL_RCC_USART6_CLK_ENABLE(); //使能USART6时钟

		GPIO_Initure.Pin = GPIO_PIN_6;			  // PA11
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;	  //复用推挽输出
		GPIO_Initure.Pull = GPIO_PULLUP;		  //上拉
		GPIO_Initure.Speed = GPIO_SPEED_FAST;	  //高速
		GPIO_Initure.Alternate = GPIO_AF8_USART6; //复用为USART6
		HAL_GPIO_Init(GPIOC, &GPIO_Initure);	  //初始化PA11

		GPIO_Initure.Pin = GPIO_PIN_7;		 // PA12
		HAL_GPIO_Init(GPIOC, &GPIO_Initure); //初始化PA12

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

	if (__HAL_UART_GET_IT_SOURCE(&UART1_Handler, UART_IT_RXNE)) // 判断是否为接收中断
	{
		vp_rxbuff[rxIndex] = USART1->DR;	  //读取串口数据
		rxIndex++;							  //接收累加器加1
		if (rxIndex >= VALUEPACK_BUFFER_SIZE) //接收到的数据长度超过最大长度，环形缓冲区溢出，将接收累加器重置为0
		{
			rxIndex = 0;
			rdIndex = 0;
		}

		__HAL_UART_CLEAR_FLAG(&UART1_Handler, UART_FLAG_RXNE); //清除接收中断标志
	}

	// vp_rxbuff[rxIndex] = USART1->DR; //读取串口数据
	// if (USART1->DR == 0XA5)					 //接受到了开始数据，数据包头正确
	// 	rxIndex = 0;					 //接收累加器清零
	// rxIndex++;							 //接收累加器加1
	// if (rxIndex == VALUEPACK_BUFFER_SIZE)		 //接收完成了
	// {
	// 	if (vp_rxbuff[rxIndex - 1] = 0X5A) // 数据包尾接受正确
	// 	{
	// 		u8 temp = 0;
	// 		rxIndex = 0;
	// 		for (int i = 0; i < VALUEPACK_BUFFER_SIZE - 3; i++)
	// 		{
	// 			temp += vp_rxbuff[i + 1];
	// 		}
	// 		if (temp == vp_rxbuff[VALUEPACK_BUFFER_SIZE - 2]) // 校验数据包和
	// 		{
	// 			u8 temp2[16] = {0};
	// 			Encoder_1 = vp_rxbuff[1];
	// 			Encoder_2 = vp_rxbuff[2];
	// 			Encoder_3 = vp_rxbuff[3];
	// 			Encoder_4 = vp_rxbuff[4];

	// 			/* 温度和电量 */
	// 			for (u8 i = 0; i < 8; i++)
	// 			{
	// 				OLED_ShowNum(i * 6, 58, vp_rxbuff[5] & 0B0001, 1, 8, 1);
	// 				vp_rxbuff[5] >> 1;
	// 			}
	// 			sprintf(temp2, "TEM:%3.1f;POW:%d", tem, vp_rxbuff[6]);
	// 			OLED_ShowString(0, 0, temp2, 16, 1);

	// 			/* 编码器速度 */
	// 			if (Encoder_1 >= 0)
	// 			{
	// 				OLED_ShowChar(0, 16, '+', 8, 1);
	// 				OLED_ShowNum(6, 16, Encoder_1, 3, 8, 1); // 显示红外传感器的数值
	// 			}
	// 			else
	// 			{
	// 				OLED_ShowChar(0, 16, '-', 8, 1);
	// 				OLED_ShowNum(6, 16, -Encoder_1, 3, 8, 1); // 显示红外传感器的数值
	// 			}
	// 			if (Encoder_2 >= 0)
	// 			{
	// 				OLED_ShowChar(0, 24, '+', 8, 1);
	// 				OLED_ShowNum(6, 24, Encoder_2, 3, 8, 1); // 显示红外传感器的数值
	// 			}
	// 			else
	// 			{
	// 				OLED_ShowChar(0, 24, '-', 8, 1);
	// 				OLED_ShowNum(6, 24, -Encoder_2, 3, 8, 1); // 显示红外传感器的数值
	// 			}

	// 			OLED_Refresh();
	// 		}
	// 		else
	// 		{
	// 			rxIndex = 0; // 数据包错误,重新开始接收
	// 		}
	// 	}
	// 	else
	// 		rxIndex = 0;
	// }
}

//串口6中断服务程序
void USART6_IRQHandler(void)
{
	// vp_rxbuff[rxIndex] = USART6->DR;	  //读取串口数据
	// if (USART6->DR == 0XA5)				  //接受到了开始数据，数据包头正确
	// 	rxIndex = 0;					  //接收累加器清零
	// rxIndex++;							  //接收累加器加1
	// if (rxIndex == VALUEPACK_BUFFER_SIZE) //接收完成了
	// {
	// 	if (vp_rxbuff[rxIndex - 1] = 0X5A) // 数据包尾接受正确
	// 	{
	// 		u8 temp;
	// 		rxIndex = 0;
	// 		for (int i = 0; i < VALUEPACK_BUFFER_SIZE - 3; i++)
	// 		{
	// 			temp += vp_rxbuff[i + 1];
	// 		}
	// 		if (temp == vp_rxbuff[VALUEPACK_BUFFER_SIZE - 2]) // 校验数据包和
	// 		{
	// 			move = vp_rxbuff[1] & 0X01;				//移动控制
	// 			bluetooth = (vp_rxbuff[1] >> 1) & 0X01; //蓝牙控制
	// 			beep = (vp_rxbuff[1] >> 2) & 0X01;		//蜂鸣器控制
	// 			Angle_Target = (float)(vp_rxbuff[2] + 0x100 * vp_rxbuff[3] + 0x10000 * vp_rxbuff[4] + 0x1000000 * vp_rxbuff[5]) / 100;
	// 			TargetSpeed = (float)(vp_rxbuff[6] + 0x100 * vp_rxbuff[7] + 0x10000 * vp_rxbuff[8] + 0x1000000 * vp_rxbuff[9]) / 100;
	// 		}
	// 		else
	// 		{
	// 			rxIndex = 0; // 数据包错误,重新开始接收
	// 		}
	// 	}
	// 	else
	// 		rxIndex = 0;
	// }
	__HAL_UART_CLEAR_FLAG(&UART6_Handler, UART_FLAG_RXNE); //清除接收中断标志
}