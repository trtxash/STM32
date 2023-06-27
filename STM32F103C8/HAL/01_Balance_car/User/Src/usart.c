#include "usart.h"

UART_HandleTypeDef UART1_Handler; // UART1句柄
UART_HandleTypeDef UART3_Handler; // UART3句柄
DMA_HandleTypeDef hdma_usart3_rx;

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

// 初始化IO 串口3
//  bound:波特率
void uart3_init(u32 bound)
{
#if VALUEPACK
    UART3_Handler.Instance = USART3;
    UART3_Handler.Init.BaudRate = bound;
    UART3_Handler.Init.WordLength = UART_WORDLENGTH_8B;
    UART3_Handler.Init.StopBits = UART_STOPBITS_1;
    UART3_Handler.Init.Parity = UART_PARITY_NONE;
    UART3_Handler.Init.Mode = UART_MODE_TX_RX;
    UART3_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UART3_Handler.Init.OverSampling = UART_OVERSAMPLING_16; // 过采样16倍
    if (HAL_UART_Init(&UART3_Handler) != HAL_OK)
    {
        Error_Handler();
    }
#else
    // UART 初始化设置
    UART3_Handler.Instance = USART3;                    // USART3
    UART3_Handler.Init.BaudRate = bound;                // 波特率
    UART3_Handler.Init.WordLength = UART_WORDLENGTH_8B; // 字长为8位数据格式
    UART3_Handler.Init.StopBits = UART_STOPBITS_1;      // 一个停止位
    UART3_Handler.Init.Parity = UART_PARITY_NONE;       // 无奇偶校验位
    UART3_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; // 无硬件流控
    UART3_Handler.Init.Mode = UART_MODE_TX_RX;          // 收发模式
    if (HAL_UART_Init(&UART3_Handler) != HAL_OK)        // HAL_UART_Init()会使能UART3
    {
        Error_Handler();
    }

    HAL_UART_Receive_IT(&UART3_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE); // 该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
#endif
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

        GPIO_Initure.Pin = GPIO_PIN_9;             // PA9
        GPIO_Initure.Mode = GPIO_MODE_AF_PP;       // 复用推挽输出
        GPIO_Initure.Pull = GPIO_PULLUP;           // 上拉
        GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; // 高速
        HAL_GPIO_Init(GPIOA, &GPIO_Initure);       // 初始化PA9

        GPIO_Initure.Pin = GPIO_PIN_10;      // PA10
        HAL_GPIO_Init(GPIOA, &GPIO_Initure); // 初始化PA10

#if EN_USART1_RX
        HAL_NVIC_EnableIRQ(USART1_IRQn);         // 使能USART1中断通道
        HAL_NVIC_SetPriority(USART1_IRQn, 7, 0); // 抢占优先级7，子优先级0
#endif
    }
    if (huart->Instance == USART3) // 如果是串口3，进行串口3 MSP初始化
    {
#if VALUEPACK
        /* USER CODE BEGIN USART3_MspInit 0 */

        /* USER CODE END USART3_MspInit 0 */
        /* USART3 clock enable */
        __HAL_RCC_USART3_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**USART3 GPIO Configuration
        PB10     ------> USART3_TX
        PB11     ------> USART3_RX
        */
        GPIO_Initure.Pin = GPIO_PIN_10;
        GPIO_Initure.Mode = GPIO_MODE_AF_PP;
        GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &GPIO_Initure);

        GPIO_Initure.Pin = GPIO_PIN_11;
        GPIO_Initure.Mode = GPIO_MODE_INPUT;
        GPIO_Initure.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOB, &GPIO_Initure);

        /* USART3 DMA Init */
        /* USART3_RX Init */
        hdma_usart3_rx.Instance = DMA1_Channel3;
        hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart3_rx.Init.Mode = DMA_CIRCULAR;
        hdma_usart3_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
        if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(huart, hdmarx, hdma_usart3_rx);

        /* USART3 interrupt Init */
        HAL_NVIC_SetPriority(USART3_IRQn, 7, 0); // 这里开中断是因为stm32f4xx_hal_uart.c里面的介绍，用于校验完成最后一个字节的发送完成
        HAL_NVIC_EnableIRQ(USART3_IRQn);
        /* USER CODE BEGIN USART3_MspInit 1 */

        /* USER CODE END USART3_MspInit 1 */
#else
        __HAL_RCC_GPIOC_CLK_ENABLE();                                   // 使能GPIOA时钟
        __HAL_RCC_USART3_CLK_ENABLE();                                  // 使能USART3时钟

        GPIO_Initure.Pin = GPIO_PIN_6;             // PA11
        GPIO_Initure.Mode = GPIO_MODE_AF_PP;       // 复用推挽输出
        GPIO_Initure.Pull = GPIO_PULLUP;           // 上拉
        GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; // 高速
        GPIO_Initure.Alternate = GPIO_AF8_USART3;  // 复用为USART3
        HAL_GPIO_Init(GPIOC, &GPIO_Initure);       // 初始化PA11

        GPIO_Initure.Pin = GPIO_PIN_7;           // PA12
        HAL_GPIO_Init(GPIOC, &GPIO_Initure);     // 初始化PA12

#if EN_USART3_RX
        HAL_NVIC_EnableIRQ(USART3_IRQn);         // 使能USART3中断通道
        HAL_NVIC_SetPriority(USART3_IRQn, 8, 0); // 抢占优先级6，子优先级1
#endif

#endif
    }
}