#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"
#include "oled.h"

#define EN_USART1_RX 1 //使能（1）/禁止（0）串口1接收
#define EN_USART6_RX 1 //使能（1）/禁止（0）串口1接收

// 本程序通过USART 配合接收中断 进行数据包的接收和发送
// 接收的数据在接收中断中写入到buffer中，通过定时调用readValuePack()函数来解析，定时间隔建议在10ms以内。
// 1.指定接收缓冲区的大小 ----------------------------------------------------------------------------------
//    一般需要512字节以上，需要根据实际接收数据的速度和proc函数的频率考虑。
#define VALUEPACK_BUFFER_SIZE 10240

// 2.指定发送数据包的结构--------------------------在发送时会自动额外在前后加上包头，包尾和校验和数据，因此会多出3个字节
//    根据实际需要的变量，定义数据包中 bool byte short int float 五种类型的数目
#define TX_BOOL_NUM 0
#define TX_BYTE_NUM 0
#define TX_SHORT_NUM 0
#define TX_INT_NUM 0
#define TX_FLOAT_NUM 0

// 3.指定接收数据包的结构-----------------------------------------------------------------------------------
//    根据实际需要的变量，定义数据包中 bool byte short int float 五种类型的数目
#define RX_BOOL_NUM 8
#define RX_BYTE_NUM 0
#define RX_SHORT_NUM 5
#define RX_INT_NUM 0
#define RX_FLOAT_NUM 0

typedef struct
{
#if TX_BOOL_NUM > 0
    unsigned char bools[TX_BOOL_NUM];
#endif

#if TX_BYTE_NUM > 0
    char bytes[TX_BYTE_NUM];
#endif

#if TX_SHORT_NUM > 0
    short shorts[TX_SHORT_NUM];
#endif

#if TX_INT_NUM > 0
    int integers[TX_INT_NUM];
#endif

#if TX_FLOAT_NUM > 0
    float floats[TX_FLOAT_NUM];
#endif
    char space; //只是为了占一个空，当所有变量数目都为0时确保编译成功
} TxPack;
typedef struct
{
#if RX_BOOL_NUM > 0
    unsigned char bools[RX_BOOL_NUM];
#endif

#if RX_BYTE_NUM > 0
    char bytes[RX_BYTE_NUM];
#endif

#if RX_SHORT_NUM > 0
    short shorts[RX_SHORT_NUM];
#endif

#if RX_INT_NUM > 0
    int integers[RX_INT_NUM];
#endif

#if RX_FLOAT_NUM > 0
    float floats[RX_FLOAT_NUM];
#endif
    char space; //只是为了占一个空，当所有变量数目都为0时确保编译成功
} RxPack;

#define PACK_HEAD 0xa5
#define PACK_TAIL 0x5a

#define TX_MODE_SEND_BACK 0
#define TX_MODE_ALWAYS 1
#define TX_MODE_DISABLE 2

extern UART_HandleTypeDef UART1_Handler; // UART句柄
extern UART_HandleTypeDef UART6_Handler; // UART句柄
extern RxPack rxpack;
extern u8 beep;
extern u8 move;
extern u8 bluetooth;
extern short Encoder_1;      // 外部变量，当前1速度
extern short Encoder_2;      // 外部变量，当前2速度
extern short Encoder_3;      // 外部变量，当前3速度
extern short Encoder_4;      // 外部变量，当前4速度
extern double TargetSpeed_1; // 目标速度
extern double TargetSpeed_2; // 目标速度
extern double TargetSpeed;   // 目标和速度
extern double Angle_Target;  // 目标速度
extern int Now_pos;          // 当前位置
extern float tem;            // 温度数组

//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
void uart6_init(u32 bound);
// 需要保证至少每秒执行10次该函数
// 该函数的主要过程是先解析接收的缓冲区，如果接收到完整的RX数据包，则解析RX数据包中的数据，然后开始串口发送TX数据包 。
// 接收到数据包时 返回 1 ，否则返回 0
unsigned char readValuePack(RxPack *rx_pack_ptr);
// 发送数据包
void sendValuePack(TxPack *tx_pack_ptr);

#endif