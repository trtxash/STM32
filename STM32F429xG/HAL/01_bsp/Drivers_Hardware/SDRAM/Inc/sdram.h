#ifndef __SDRAM_H
#define __SDRAM_H

#include "delay.h"
#include "fmc.h"
#include "log_rtt.h"

#define Bank5_SDRAM_ADDR ((uint32_t)0xC0000000) // SDRAM开始地址
#define Bank5_SDRAM_SIZE (32 * 1024 * 1024)     // 4*1024*1024*4*16 Bits=32M Byte

// SDRAM配置参数
#define SDRAM_MODEREG_BURST_LENGTH_1 ((u16)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2 ((u16)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4 ((u16)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8 ((u16)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL ((u16)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED ((u16)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2 ((u16)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3 ((u16)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD ((u16)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((u16)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE ((u16)0x0200)

void SDRAM_Init(void);
void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram);
u8 SDRAM_Send_Cmd(u8 bankx, u8 cmd, u8 refresh, u16 regval);
void FMC_SDRAM_WriteBuffer(u8 *pBuffer, u32 WriteAddr, u32 n);
void FMC_SDRAM_ReadBuffer(u8 *pBuffer, u32 ReadAddr, u32 n);
void __attribute__((optimize("O3"))) SDRAM_WriteSpeedTest_32bits(void);
void __attribute__((optimize("O3"))) SDRAM_WriteSpeedTest_16bits(void);
void __attribute__((optimize("O3"))) SDRAM_ReadSpeedTest(void);

#endif