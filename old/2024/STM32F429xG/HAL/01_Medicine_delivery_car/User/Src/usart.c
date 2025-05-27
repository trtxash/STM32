#include "usart.h"

UART_HandleTypeDef UART1_Handler; // UART1句柄
UART_HandleTypeDef UART6_Handler; // UART6句柄
DMA_HandleTypeDef hdma_usart6_tx;
DMA_HandleTypeDef hdma_usart6_rx;

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
    UART1_Handler.Instance = USART1;                    // USART1
    UART1_Handler.Init.BaudRate = bound;                // 波特率
    UART1_Handler.Init.WordLength = UART_WORDLENGTH_8B; // 字长为8位数据格式
    UART1_Handler.Init.StopBits = UART_STOPBITS_1;      // 一个停止位
    UART1_Handler.Init.Parity = UART_PARITY_NONE;       // 无奇偶校验位
    UART1_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; // 无硬件流控
    UART1_Handler.Init.Mode = UART_MODE_TX_RX;          // 收发模式
    if (HAL_UART_Init(&UART1_Handler) != HAL_OK)        // HAL_UART_Init()会使能UART1
    {
        Error_Handler();
    }

    HAL_UART_Receive_IT(&UART1_Handler, aRxBuffer, RXBUFFERSIZE); // 如果要调用处理回调函数，用这个函数使能接收中断
}

// 初始化IO 串口6
//  bound:波特率
void uart6_init(u32 bound)
{
#if VALUEPACK
    UART6_Handler.Instance = USART6;
    UART6_Handler.Init.BaudRate = bound;
    UART6_Handler.Init.WordLength = UART_WORDLENGTH_8B;
    UART6_Handler.Init.StopBits = UART_STOPBITS_1;
    UART6_Handler.Init.Parity = UART_PARITY_NONE;
    UART6_Handler.Init.Mode = UART_MODE_TX_RX;
    UART6_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UART6_Handler.Init.OverSampling = UART_OVERSAMPLING_16; // 过采样16倍
    if (HAL_UART_Init(&UART6_Handler) != HAL_OK)
    {
        Error_Handler();
    }
#else
    // UART 初始化设置
    UART6_Handler.Instance = USART6;                    // USART6
    UART6_Handler.Init.BaudRate = bound;                // 波特率
    UART6_Handler.Init.WordLength = UART_WORDLENGTH_8B; // 字长为8位数据格式
    UART6_Handler.Init.StopBits = UART_STOPBITS_1;      // 一个停止位
    UART6_Handler.Init.Parity = UART_PARITY_NONE;       // 无奇偶校验位
    UART6_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; // 无硬件流控
    UART6_Handler.Init.Mode = UART_MODE_TX_RX;          // 收发模式
    if (HAL_UART_Init(&UART6_Handler) != HAL_OK)        // HAL_UART_Init()会使能UART6
    {
        Error_Handler();
    }

    HAL_UART_Receive_IT(&UART6_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE); // 该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
#endif
}

// UART底层初始化，时钟使能，引脚配置，中断配置
// 此函数会被HAL_UART_Init()调用
// uartHandle:串口句柄
void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{
    // GPIO端口设置
    GPIO_InitTypeDef GPIO_Initure = {0};

    if (uartHandle->Instance == USART1) // 如果是串口1，进行串口1 MSP初始化
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();  // 使能GPIOA时钟
        __HAL_RCC_USART1_CLK_ENABLE(); // 使能USART1时钟

        GPIO_Initure.Pin = GPIO_PIN_9;            // PA9
        GPIO_Initure.Mode = GPIO_MODE_AF_PP;      // 复用推挽输出
        GPIO_Initure.Pull = GPIO_PULLUP;          // 上拉
        GPIO_Initure.Speed = GPIO_SPEED_FAST;     // 高速
        GPIO_Initure.Alternate = GPIO_AF7_USART1; // 复用为USART1
        HAL_GPIO_Init(GPIOA, &GPIO_Initure);      // 初始化PA9

        GPIO_Initure.Pin = GPIO_PIN_10;      // PA10
        HAL_GPIO_Init(GPIOA, &GPIO_Initure); // 初始化PA10

#if EN_USART1_RX
        HAL_NVIC_EnableIRQ(USART1_IRQn);         // 使能USART1中断通道
        HAL_NVIC_SetPriority(USART1_IRQn, 0, 0); // 抢占优先级0，子优先级0
#endif
    }
    if (uartHandle->Instance == USART6) // 如果是串口6，进行串口6 MSP初始化
    {
#if VALUEPACK
        /* USER CODE BEGIN USART6_MspInit 0 */

        /* USER CODE END USART6_MspInit 0 */
        /* USART6 clock enable */
        __HAL_RCC_USART6_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**USART6 GPIO Configuration
        PC6     ------> USART6_TX
        PC7     ------> USART6_RX
        */
        GPIO_Initure.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_Initure.Mode = GPIO_MODE_AF_PP;
        GPIO_Initure.Pull = GPIO_NOPULL;
        GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_Initure.Alternate = GPIO_AF8_USART6;
        HAL_GPIO_Init(GPIOC, &GPIO_Initure);

        /* USART6 DMA Init */
        /* USART6_RX Init */
        hdma_usart6_rx.Instance = DMA2_Stream1;
        hdma_usart6_rx.Init.Channel = DMA_CHANNEL_5;
        hdma_usart6_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_usart6_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart6_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart6_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart6_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart6_rx.Init.Mode = DMA_CIRCULAR;
        hdma_usart6_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
        hdma_usart6_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_usart6_rx) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(uartHandle, hdmarx, hdma_usart6_rx);

        /* USART6_TX Init */
        hdma_usart6_tx.Instance = DMA2_Stream6;
        hdma_usart6_tx.Init.Channel = DMA_CHANNEL_5;
        hdma_usart6_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_usart6_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart6_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart6_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart6_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart6_tx.Init.Mode = DMA_NORMAL;
        hdma_usart6_tx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_usart6_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_usart6_tx) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(uartHandle, hdmatx, hdma_usart6_tx);

        /* USART6 interrupt Init */
        HAL_NVIC_SetPriority(USART6_IRQn, 1, 0); // 这里开中断是因为stm32f4xx_hal_uart.c里面的介绍，用于校验完成最后一个字节的发送完成
        HAL_NVIC_EnableIRQ(USART6_IRQn);
        /* USER CODE BEGIN USART6_MspInit 1 */
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{

    if (uartHandle->Instance == USART6)
    {
        /* USER CODE BEGIN USART6_MspDeInit 0 */

        /* USER CODE END USART6_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART6_CLK_DISABLE();

        /**USART6 GPIO Configuration
        PC6     ------> USART6_TX
        PC7     ------> USART6_RX
        */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6 | GPIO_PIN_7);

        /* USART6 DMA DeInit */
        HAL_DMA_DeInit(uartHandle->hdmarx);
        HAL_DMA_DeInit(uartHandle->hdmatx);
        /* USER CODE BEGIN USART6_MspDeInit 1 */

        /* USER CODE END USART6_MspDeInit 1 */
#else
        __HAL_RCC_GPIOC_CLK_ENABLE();                                   // 使能GPIOA时钟
        __HAL_RCC_USART6_CLK_ENABLE();                                  // 使能USART6时钟

        GPIO_Initure.Pin = GPIO_PIN_6;            // PA11
        GPIO_Initure.Mode = GPIO_MODE_AF_PP;      // 复用推挽输出
        GPIO_Initure.Pull = GPIO_PULLUP;          // 上拉
        GPIO_Initure.Speed = GPIO_SPEED_FAST;     // 高速
        GPIO_Initure.Alternate = GPIO_AF8_USART6; // 复用为USART6
        HAL_GPIO_Init(GPIOC, &GPIO_Initure);      // 初始化PA11

        GPIO_Initure.Pin = GPIO_PIN_7;           // PA12
        HAL_GPIO_Init(GPIOC, &GPIO_Initure);     // 初始化PA12

#if EN_USART6_RX
        HAL_NVIC_EnableIRQ(USART6_IRQn);         // 使能USART6中断通道
        HAL_NVIC_SetPriority(USART6_IRQn, 8, 0); // 抢占优先级6，子优先级1
#endif

#endif
    }
}