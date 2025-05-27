// #ifndef _VOFA_H
// #define _VOFA_H

// #include "sys.h"
// #include "usart.h"

// // 本程序通过DMA和USART 进行数据包的接收和发送
// // 接收的数据自动写入到buffer中，通过定时调用readVOFA()函数来解析，定时间隔建议在10ms以内。
// // 数据发送也采用DMA

// /// 1.指定接收缓冲区的大小 ----------------------------------------------------------------------------------
// //    一般需要512字节以上，需要根据实际接收数据的速度和proc函数的频率考虑。
// #define VOFA_BUFFER_SIZE 1024

// /// 2.指定发送到上位机的数据包的结构--------------------------在发送时会自动额外在前后加上包头，包尾和校验和数据，因此会多出3个字节
// //    根据实际需要的通道数，都为float型
// #define TX_CH_COUNT 1

// /// 3.指定接收数据包的结构-----------------------------------------------------------------------------------
// //    根据实际需要的变量，float
// #define RX_CH_COUNT 1

// typedef struct
// {
//     float floats[TX_CH_COUNT];
// } VOFA_TxPack;

// typedef struct
// {
//     float floats[RX_CH_COUNT];
// } VOFA_RxPack;
// // 初始化 valuepack 包括一些必要的硬件外设配置

// extern VOFA_TxPack txvofapack;
// extern VOFA_RxPack rxvofapack;

// void initVOFAPack(u32 baudrate);

// // 需要保证至少每秒执行10次该函数
// // 该函数的主要过程是先解析接收的缓冲区，如果接收到完整的RX数据包，则解析RX数据包中的数据，然后开始串口发送TX数据包 。
// // 接收到数据包时 返回 1 ，否则返回 0
// unsigned char readVOFAPack(VOFA_RxPack *rx_pack_ptr);

// // 发送数据包
// void sendVOFAPack(VOFA_TxPack *tx_pack_ptr);

// #define PACK_TAIL 0x5a

// #endif