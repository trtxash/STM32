#include "sdram.h"
#include "delay.h"
#include "log_rtt.h"
#include "tim.h"

// SDRAM初始化
void SDRAM_Init(void)
{
    FMC_Init();
    SDRAM_Initialization_Sequence(&hsdram1); // 发送SDRAM初始化序列
    // 刷新频率计数器(以SDCLK频率计数),计算方法:
    // COUNT=SDRAM刷新周期/行数-20=SDRAM刷新周期(us)*SDCLK频率(Mhz)/行数
    // 我们使用的SDRAM刷新周期为64ms,SDCLK=240/2=120Mhz,行数为8192(2^13).
    // 所以,COUNT=64*1000*120/8192-20=683
    // HAL_SDRAM_ProgramRefreshRate(&hsdram1, 917); // 设置刷新频率
    HAL_SDRAM_ProgramRefreshRate(&hsdram1, 683); // 设置刷新频率
}

// 发送SDRAM初始化序列
void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram)
{
    u32 temp = 0;
    // SDRAM控制器初始化完成以后还需要按照如下顺序初始化SDRAM
    SDRAM_Send_Cmd(0, FMC_SDRAM_CMD_CLK_ENABLE, 1, 0);       // 时钟配置使能
    delay_us(500);                                           // 至少延时200us
    SDRAM_Send_Cmd(0, FMC_SDRAM_CMD_PALL, 1, 0);             // 对所有存储区预充电
    SDRAM_Send_Cmd(0, FMC_SDRAM_CMD_AUTOREFRESH_MODE, 8, 0); // 设置自刷新次数
    // 配置模式寄存器,SDRAM的bit0~bit2为指定突发访问的长度，
    // bit3为指定突发访问的类型，bit4~bit6为CAS值，bit7和bit8为运行模式
    // bit9为指定的写突发模式，bit10和bit11位保留位
    temp = (u32)SDRAM_MODEREG_BURST_LENGTH_1 |           // 设置突发长度:1(可以是1/2/4/8)
           SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |         // 设置突发类型:连续(可以是连续/交错)
           SDRAM_MODEREG_CAS_LATENCY_3 |                 // 设置CAS值:3(可以是2/3)
           SDRAM_MODEREG_OPERATING_MODE_STANDARD |       // 设置操作模式:0,标准模式
           SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;         // 设置突发写模式:1,单点访问
    SDRAM_Send_Cmd(0, FMC_SDRAM_CMD_LOAD_MODE, 1, temp); // 设置SDRAM的模式寄存器
}

// 向SDRAM发送命令
// bankx:0,向BANK5上面的SDRAM发送指令
//       1,向BANK6上面的SDRAM发送指令
// cmd:指令(0,正常模式/1,时钟配置使能/2,预充电所有存储区/3,自动刷新/4,加载模式寄存器/5,自刷新/6,掉电)
// refresh:自刷新次数
// regval:模式寄存器的定义
// 返回值:0,正常;1,失败.
u8 SDRAM_Send_Cmd(u8 bankx, u8 cmd, u8 refresh, u16 regval)
{
    u32 target_bank = 0;
    FMC_SDRAM_CommandTypeDef Command;

    if (bankx == 0)
        target_bank = FMC_SDRAM_CMD_TARGET_BANK1;
    else if (bankx == 1)
        target_bank = FMC_SDRAM_CMD_TARGET_BANK2;
    Command.CommandMode = cmd;                                       // 命令
    Command.CommandTarget = target_bank;                             // 目标SDRAM存储区域
    Command.AutoRefreshNumber = refresh;                             // 自刷新次数
    Command.ModeRegisterDefinition = regval;                         // 要写入模式寄存器的值
    if (HAL_SDRAM_SendCommand(&hsdram1, &Command, 0X1000) == HAL_OK) // 向SDRAM发送命令
    {
        return 0;
    }
    else
        return 1;
}

// 在指定地址(WriteAddr+Bank5_SDRAM_ADDR)开始,连续写入n个字节.
// pBuffer:字节指针
// WriteAddr:要写入的地址
// n:要写入的字节数
void FMC_SDRAM_WriteBuffer(u8 *pBuffer, u32 WriteAddr, u32 n)
{
    for (; n != 0; n--)
    {
        *(vu8 *)(Bank5_SDRAM_ADDR + WriteAddr) = *pBuffer;
        WriteAddr++;
        pBuffer++;
    }
}

// 在指定地址((WriteAddr+Bank5_SDRAM_ADDR))开始,连续读出n个字节.
// pBuffer:字节指针
// ReadAddr:要读出的起始地址
// n:要写入的字节数
void FMC_SDRAM_ReadBuffer(u8 *pBuffer, u32 ReadAddr, u32 n)
{
    for (; n != 0; n--)
    {
        *pBuffer++ = *(vu8 *)(Bank5_SDRAM_ADDR + ReadAddr);
        ReadAddr++;
    }
}

/*
*********************************************************************************************************
*	函 数 名: WriteSpeedTest
*	功能说明: 写SDRAM速度测试
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SDRAM_WriteSpeedTest_32bits(void)
{
    volatile uint32_t i, j;
    uint32_t iTime1, iTime2, iTime3;
    volatile uint32_t *pBuf;

    /* 设置初始化值并记下开始时间 */
    pBuf = (uint32_t *)Bank5_SDRAM_ADDR;
    iTime1 = FreeRTOSRunTimeTicks;

    /* 以递增的方式写数据到SDRAM所有空间 */
    for (j = 0, i = 0; i < Bank5_SDRAM_SIZE / 512; i++)
    {
        *pBuf++ = j++; // 一行4Byte
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++; // 8行32Byte

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++; // 4*8*4 Byte = 128 Byte

        *pBuf++ = j++; // 一行4Byte
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++; // 8行32Byte

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++; // 128*2 Byte

        *pBuf++ = j++; // 一行4Byte
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++; // 8行32Byte

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++; // 128*3 Byte

        *pBuf++ = j++; // 一行4Byte
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++; // 8行32Byte

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++; // 128*4=512 Byte
    }
    iTime2 = FreeRTOSRunTimeTicks;
    if (iTime2 < iTime1)
        iTime3 = iTime1 - iTime2;
    else
        iTime3 = iTime2 - iTime1;

    /* 读取写入的是否出错 */
    pBuf = (uint32_t *)Bank5_SDRAM_ADDR;
    for (j = 0, i = 0; i < Bank5_SDRAM_SIZE / 4; i++)
    {
        if (*pBuf != j)
        {
            LOGE("Write err %u,%u", j, *pBuf);
            break;
        }
        else
        {
            pBuf++;
            j++;
        }
    }

    /* 打印速度 */
    LOGI("32bit/32MB Write time %u ms, speed %u MB/s", (uint32_t)(iTime3 * 50 / 1000),
         (uint32_t)((Bank5_SDRAM_SIZE / 1024 / 1024 * 1000000) / (iTime3 * 50)));
}

void SDRAM_WriteSpeedTest_16bits(void)
{
    volatile uint32_t i;
    volatile uint16_t j;
    uint32_t iTime1, iTime2, iTime3;
    volatile uint16_t *pBuf;

    /* 设置初始化值并记下开始时间 */
    pBuf = (uint16_t *)Bank5_SDRAM_ADDR;
    iTime1 = FreeRTOSRunTimeTicks;

    /* 以递增的方式写数据到SDRAM所有空间 */
    for (j = 0, i = 0; i < Bank5_SDRAM_SIZE / (65536 * 2);) // 2*(2^16) Byte = 2^17 Byte
    {
        *pBuf++ = j++; // 一行2Byte
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++; // 8行16Byte

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++; // 2*8*4 Byte = 64 Byte

        *pBuf++ = j++; // 一行2Byte
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++; // 8行16Byte

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j; // 64*2 Byte = 128 Byte

        if (j == 0XFFFF)
        {
            j = 0;
            i++;
        }

        else
        {
            j++;
        }
    }

    iTime2 = FreeRTOSRunTimeTicks;
    if (iTime2 < iTime1)
        iTime3 = iTime1 - iTime2;
    else
        iTime3 = iTime2 - iTime1;

    /* 读取写入的是否出错 */
    pBuf = (uint16_t *)Bank5_SDRAM_ADDR;
    for (j = 0, i = 0; i < Bank5_SDRAM_SIZE / (65536 * 2);) // 2^17 Byte
    {
        if (*pBuf != j)
        {
            LOGE("Write err %u,%u", j, *pBuf);
            break;
        }
        pBuf++;
        if (j == 0XFFFF)
        {
            i++;
            j = 0;
        }
        else
        {
            j++;
        }
    }

    /* 打印速度 */
    LOGI("16bit/32MB Write time %u ms, speed %u MB/s", (uint32_t)(iTime3 * 50 / 1000),
         (uint32_t)((Bank5_SDRAM_SIZE / 1024 / 1024 * 1000000) / (iTime3 * 50)));
}

/*
*********************************************************************************************************
*	函 数 名: ReadSpeedTest
*	功能说明: 读SDRAM速度测试
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SDRAM_ReadSpeedTest(void)
{
    volatile uint32_t i;
    volatile uint32_t ulTemp;
    uint32_t iTime1, iTime2, iTime3;
    volatile uint32_t *pBuf;

    /* 设置初始化值并记下开始时间 */
    pBuf = (uint32_t *)Bank5_SDRAM_ADDR;
    iTime1 = FreeRTOSRunTimeTicks;

    /* 读取SDRAM所有空间数据 */
    for (i = 0; i < Bank5_SDRAM_SIZE / 128; i++)
    {
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++; // 32Byte

        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;

        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;

        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++; // 128Byte
    }
    iTime2 = FreeRTOSRunTimeTicks;
    if (iTime2 < iTime1)
        iTime3 = iTime1 - iTime2;
    else
        iTime3 = iTime2 - iTime1;

    /* 打印速度 */
    LOGI("32bit/32MB Read time %u ms, speed %u MB/s", (uint32_t)(iTime3 * 50 / 1000),
         (uint32_t)((Bank5_SDRAM_SIZE / 1024 / 1024 * 1000000) / (iTime3 * 50)));
}
