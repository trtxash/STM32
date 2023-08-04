#ifndef _CONTROL_H
#define _CONTROL_H

#include "ipi_ring_buffer.h"
#include "led.h"
#include "main.h"
#include "positional_pid.h"
#include "sys.h"
#include "usart.h"

// IPiUS控制指令数据
// 注: 一下所有的指令都是针对单个舵机的
#define IPiUS_CMD_NUM 17
#define IPiUS_CMD_PING 1                               // 舵机通讯检测
#define IPiUS_CMD_RESET_USER_DATA 2                    // 重置用户数据
#define IPiUS_CMD_READ_DATA 3                          // 单个舵机 读取数据库
#define IPiUS_CMD_WRITE_DATA 4                         // 单个舵机 写入数据块
#define IPiUS_CMD_READ_BATCH_DATA 5                    // 单个舵机 批次读取(读取一个舵机所有的数据)
#define IPiUS_CMD_WRITE_BATCH_DATA 6                   // 单个舵机 批次写入(写入一个舵机所有的数据)
#define IPiUS_CMD_SPIN 7                               // 单个舵机 设置轮式模式
#define IPiUS_CMD_ROTATE 8                             // 角度控制模式(设置舵机的角度))
#define IPiUS_CMD_DAMPING 9                            // 阻尼模式
#define IPiUS_CMD_READ_ANGLE 10                        // 舵机角度读取
#define IPiUS_CMD_SET_SERVO_ANGLE_BY_INTERVAL 11       // 角度设置(指定周期)
#define IPiUS_CMD_SET_SERVO_ANGLE_BY_VELOCITY 12       // 角度设置(指定转速)
#define IPiUS_CMD_SET_SERVO_ANGLE_MTURN 13             // 多圈角度设置
#define IPiUS_CMD_SET_SERVO_ANGLE_MTURN_BY_INTERVAL 14 // 多圈角度设置(指定周期)
#define IPiUS_CMD_SET_SERVO_ANGLE_MTURN_BY_VELOCITY 15 // 多圈角度设置(指定转速)
#define IPiUS_CMD_QUERY_SERVO_ANGLE_MTURN 16

// 返回的响应数据包最长的长度
#define IPiUS_PACK_RESPONSE_MAX_SIZE 50
#define IPiUS_PACK_REQUEST_HEADER 0x4c12

// 自定义参数
typedef struct _DeviceCustomParam_
{

    __IO int response_switch;    // 响应开关1
    __IO int servo_id;           // 设备ID1
    __IO int baudrate;           // 波特率选项1
    __IO int stall_protect_mode; // 堵转保护模式 1
    __IO int stall_power_limit;  // 堵转功率上限  2
    __IO int over_volt_high;     // 电压上限  2
    __IO int over_volt_low;      // 电压下限  2
    __IO float over_temprature;  // 温度保护上限 2
    __IO int over_power;         // 电流上限 2
    __IO int over_current;       // 功率上限 2
    __IO int accel_switch;       // 加速度处理开关 1
    __IO int po_lock_switch;     // 上电锁力开关 1
    __IO int wb_lock_switch;     // 轮式刹车锁力开关 1
    __IO int angle_limit_switch; // 角度限制开关 1
    __IO float angle_limit_high; // 角度上限 2
    __IO float angle_limit_low;  // 角度下限 2
    __IO float angle_mid_offset; // 中位角度偏移  2
    __IO int soft_start_switch;  // 上电首次缓慢启动开关  1
    __IO int soft_start_time;    // 上电首次缓慢启动时间  2

} DeviceCustomParam;

typedef struct
{
    UART_HandleTypeDef *pUSARTx;
    // 发送端缓冲区
    RingBufferTypeDef *sendBuf;
    // 接收端缓冲区
    RingBufferTypeDef *recvBuf;
} Usart_DataTypeDef;

// 请求数据帧的结构体
typedef struct
{
    uint16_t header;                               // 请求头
    uint8_t cmdId;                                 // 指令ID号
    uint8_t size;                                  // 包的长度
    uint8_t content[IPiUS_PACK_RESPONSE_MAX_SIZE]; // 包的内容
    uint8_t checksum;                              // 校验和
    // 用于帧的接收
    // 数据帧的接收状态 flag标志位
    uint8_t status;
} PackageTypeDef;

// 只读参数
typedef struct _DeviceParam_
{

    __IO float devWenDu;  // 设备温度
    float devJiaoDu;      // 设备角度
    __IO float devDianYa; // 设备电压
    __IO int devGongLv;   // 设备功率
    __IO int devCurrent;  // 设备电流
} DeviceParam;

// 设备信息
typedef struct _DeviceInfo_
{
    __IO uint8_t ServoId;          // 舵机id
    DeviceCustomParam customParam; // 自定义参数
    DeviceParam param;             // 参数
} DeviceInfo;

// uint8_t 1字节，uint16_t/uint32_t/uint64_t  2/4/8字节
typedef struct _CheckParameter_
{
    float mTraget; // 目标位置（角度），
    int mF;
    float mCurrentAngle;
    // 实际校对需要的参数值
    int mCheckAngle; // 校对角度，（就是某个角度相对正确角度的偏移量）
    __IO DeviceInfo devInfo;
} CheckParameter;

extern Usart_DataTypeDef servo_usart; // 串口总线舵机对应的USART
extern positional_pid_params_t motor1_velocity;
extern positional_pid_params_t motor2_velocity;

void IPiUS_Package2RingBuffer(PackageTypeDef *pkg, RingBufferTypeDef *ringBuf);
void Usart_SendAll(Usart_DataTypeDef *usart);
void IPiUS_SendPackage(Usart_DataTypeDef *usart, uint8_t cmdId, uint8_t size, uint8_t *content);
u8 IPiUS_SetServoAngle(Usart_DataTypeDef *usart, uint8_t servo_id, float angle, uint16_t interval, uint16_t power, uint8_t wait);
int SetAngle(int armID, float angle, int seep, int wait);
void MIAN_TASK(void);

#endif