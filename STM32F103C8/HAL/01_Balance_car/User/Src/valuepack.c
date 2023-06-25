#include "valuepack.h"

TxPack txvaluepack;
RxPack rxvaluepack;

// 发送数据包的字节长度
// const unsigned short TXPACK_BYTE_SIZE = ((TX_BOOL_NUM + 7) >> 3) + TX_BYTE_NUM + (TX_SHORT_NUM << 1) + (TX_INT_NUM << 2) + (TX_FLOAT_NUM << 2);
#define TXPACK_BYTE_SIZE ((TX_BOOL_NUM + 7) >> 3) + TX_BYTE_NUM + (TX_SHORT_NUM << 1) + (TX_INT_NUM << 2) + (TX_FLOAT_NUM << 2)

// 接收数据包的字节长度
const unsigned short RXPACK_BYTE_SIZE = ((RX_BOOL_NUM + 7) >> 3) + RX_BYTE_NUM + (RX_SHORT_NUM << 1) + (RX_INT_NUM << 2) + (RX_FLOAT_NUM << 2);

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

//---------------------------------------------------------------------------------------------------------------------
// 初始化DMA和串口USART0 可设置串口波特率
// DMA将串口接收寄存器和接收缓冲数组连接，将DMA设置为循环模式，实现环形缓冲区
void initValuePack(u32 baudrate)
{
    uart6_init(baudrate);
    HAL_UART_Receive_DMA(&UART6_Handler, (uint32_t)vp_rxbuff, VALUEPACK_BUFFER_SIZE);
}

// 数据读取涉及到的变量
unsigned short this_index = 0;
unsigned short last_index = 0;
unsigned short rdi, rdii, idl, idi, bool_index, bool_bit;
uint32_t idc;
// 记录读取的错误字节的次数
unsigned int err = 0;
// 用于和校验
unsigned char sum = 0;
// 存放数据包读取的结果
unsigned char isok;

//------------------------------------------------------------------------------------------------------------------------
// unsigned char readValuePack(RxPack *rx_pack_ptr)
// 尝试从缓冲区中读取数据包
// 参数   - RxPack *rx_pack_ptr： 传入接收数据结构体的指针，从环形缓冲区中读取出数据包，并将各类数据存储到rx_pack_ptr指向的结构体中
// 返回值 - 如果成功读取到数据包，则返回1，否则返回0
//
unsigned char readValuePack(RxPack *rx_pack_ptr)
{
    isok = 0;

    // 这次通过DMA接收的计数值
    this_index = VALUEPACK_BUFFER_SIZE - UART6_Handler.hdmarx->Instance->NDTR;

    // 根据上次DMA计数值和本次DMA计数值的差值，确定两次函数执行中间接收的数据字节数，并使rxIndex增加
    if (this_index < last_index)
        rxIndex += VALUEPACK_BUFFER_SIZE + this_index - last_index;
    else
        rxIndex += this_index - last_index;

    // 确保读取计数和接收计数之间的距离小于2个数据包的长度
    while (rdIndex < (rxIndex - ((rx_pack_length)*2)))
        rdIndex += rx_pack_length;

    // 如果读取计数落后于接收计数超过 1个 数据包的长度，则尝试读取
    while (rdIndex <= (rxIndex - rx_pack_length))
    {

        rdi = rdIndex % VALUEPACK_BUFFER_SIZE;
        rdii = rdi + 1;
        if (vp_rxbuff[rdi] == PACK_HEAD)
        {
            if (vp_rxbuff[(rdi + RXPACK_BYTE_SIZE + 2) % VALUEPACK_BUFFER_SIZE] == PACK_TAIL)
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

                if (sum == vp_rxbuff[rdi])
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
    last_index = this_index;
    return isok;
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
//
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

    HAL_UART_Transmit_DMA(&UART6_Handler, (uint32_t)vp_txbuff, TXPACK_BYTE_SIZE + 3);
}
