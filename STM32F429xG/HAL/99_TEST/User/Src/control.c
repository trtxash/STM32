#include "control.h"

#define SPEED 210

Usart_DataTypeDef servo_usart; // 串口总线舵机对应的USART
positional_pid_params_t motor1_velocity;
positional_pid_params_t motor2_velocity;
CheckParameter checkPar[7] = {0};

u8 TASK = 0, Do_count = 0;

// 数据帧转换为字节数组
void IPiUS_Package2RingBuffer(PackageTypeDef *pkg, RingBufferTypeDef *ringBuf)
{
    uint8_t checksum; // 校验和
    // 写入帧头
    RingBuffer_WriteUShort(ringBuf, pkg->header);
    // 写入指令ID
    RingBuffer_WriteByte(ringBuf, pkg->cmdId);
    // 写入包的长度
    RingBuffer_WriteByte(ringBuf, pkg->size);
    // 写入内容主题
    RingBuffer_WriteByteArray(ringBuf, pkg->content, pkg->size);
    // 计算校验和
    checksum = RingBuffer_GetChecksum(ringBuf);
    // 写入校验和
    RingBuffer_WriteByte(ringBuf, checksum);
}

// 将串口发送缓冲区的内容全部发出去
void Usart_SendAll(Usart_DataTypeDef *usart)
{
    uint8_t value;
    servo_usart.pUSARTx = &UART2_Handler;
    while (RingBuffer_GetByteUsed(usart->sendBuf))
    {
        value = RingBuffer_Pop(usart->sendBuf);
        // printf("Usart_SendAll pop: %d", value);
        HAL_UART_Transmit_DMA(usart->pUSARTx, &value, 1);
    }
}

// 构造发送数据帧
void IPiUS_SendPackage(Usart_DataTypeDef *usart, uint8_t cmdId, uint8_t size, uint8_t *content)
{
    // 申请内存
    // printf("[Package] malloc for pkg\r\n");
    PackageTypeDef pkg;

    // 设置帧头
    pkg.header = IPiUS_PACK_REQUEST_HEADER;
    // 设置指令ID
    pkg.cmdId = cmdId;
    // 设置尺寸
    pkg.size = size;
    // 逐一拷贝数组里面的内容
    for (int i = 0; i < size; i++)
    {
        pkg.content[i] = content[i];
    }
    // 将pkg发送到发送缓冲区sendBuf里面
    IPiUS_Package2RingBuffer(&pkg, usart->sendBuf);
    // 通过串口将数据发送出去
    Usart_SendAll(usart);
}

// 设置舵机的角度
// @angle 单位度
// @interval 单位ms
// @power 舵机执行功率 单位mW
//        若power=0或者大于保护值
u8 IPiUS_SetServoAngle(Usart_DataTypeDef *usart, uint8_t servo_id, float angle, uint16_t interval, uint16_t power, uint8_t wait)
{
    // 创建环形缓冲队列
    const uint8_t size = 9;
    uint8_t buffer[size + 1];
    RingBufferTypeDef ringBuf;
    // Buffer内存申请成功
    ringBuf.buffer = buffer; // (uint8_t *)malloc(ringBuf->size);
    ringBuf.head = 0;
    ringBuf.tail = 0;
    // 尺寸size需要比容量capacity大一
    ringBuf.size = size + 1;
    // 数值初始化置为0
    for (uint16_t i = 0; i < ringBuf.size; i++)
    {
        ringBuf.buffer[i] = 0;
    }
    // 数值约束
    if (angle > 180.0f)
    {
        angle = 180.0f;
    }
    else if (angle < -180.0f)
    {
        angle = -180.0f;
    }
    // 构造content
    RingBuffer_WriteByte(&ringBuf, servo_id);
    RingBuffer_WriteShort(&ringBuf, (int16_t)(10 * angle));
    RingBuffer_WriteUShort(&ringBuf, interval);
    RingBuffer_WriteUShort(&ringBuf, power);
    // 发送请求包
    // 注: 因为用的是环形队列 head是空出来的,所以指针需要向后推移一个字节
    IPiUS_SendPackage(usart, IPiUS_CMD_ROTATE, size, buffer + 1);

    /*if (wait){
        return IPiUS_Wait(usart, servo_id, angle, 0);
    }else{
        return IPiUS_STATUS_SUCCESS;
    }*/
    return 0;
}

// 通过角度设置舵机的位置
int SetAngle(int armID, float angle, int seep, int wait)
{
    if (angle < -135 || angle > 135)
        return 0; // 判断角度有效性
    if (seep < 0)
        return 0; // 判断速度有效性

    IPiUS_SetServoAngle(&servo_usart, armID, angle + checkPar[armID].mCheckAngle, seep, 0, wait);
    checkPar[armID].mTraget = angle + checkPar[armID].mCheckAngle; // 记录目标位置

    checkPar[armID].mCurrentAngle = angle + checkPar[armID].mCheckAngle;
    return 1;
}

// 第一个岔口:2150，第二个岔口4850，第三个岔路口7500，近中端1400，远端2650
// 主要任务
void MIAN_TASK(void)
{
    if (TASK == 0) // 等待识别目标房间
    {
    }
    else if (TASK == 1)
    {
        switch (Do_count)
        {
        case 0: // 直走
            Do_count++;
            break;
        default:
            break;
        }
    }
}