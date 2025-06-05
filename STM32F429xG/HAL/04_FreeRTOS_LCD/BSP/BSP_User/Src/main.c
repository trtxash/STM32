/**
 * @file	04_FreeRTOS_LCD
 * @brief
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2025年5月10号19点46分
 */

// 使用RTT需要开启 JLinkRTTClient，插好jlink，按下Ctrl+P，输入task[空格]rtt，即可启动RTT Client
// DMA初始化要在别的之前,不然出问题

#include "main.h"
#include "adc.h"
#include "dma.h"
#include "ft5446.h"
#include "key.h"
#include "lcd.h"
#include "led.h"
#include "sdram.h"
#include "start_task.h"

/**
 * @brief	板级初始化函数
 * @param 	无
 * @arg
 * @note  	争对板子的初始化，包括时钟配置，外设初始化，中断配置等。
 * @retval	无
 */
static void bsp_init(void)
{
    Stm32_Clock_Init(SYS_CLOCK, 12, RCC_PLLP_DIV2, 8); // 设置时钟
    delay_init(SYS_CLOCK);                             // 延时初始化
    LED0_Init();
    LED1_Init();
    LED2_Init();
    LED3_Init();
    KEY_Init();

    DMA_Init(); // DMA初始化要在别的之前,不然出问题

    // 16bit * 4 BANK * 4M = 32MB SDRAM; FMC 120Mhz,
    // 测试SDRAM读写速度,顺序写入32bit/32MB Write time 252 ms, speed 126 MB/s; 顺序读取32bit/32MB Read time 330 ms, speed 96 MB/s
    // 126 * 8 = 1008 Mbit/s, 相当于一个时钟周期写入 1008/120 = 8.4bit
    SDRAM_Init();

    ADC1_Init();

    // 一般放在最后,等待LCD硬件上电完成
    // LTDC初始化,LTDC 25MHz时钟,分辨率为800*480,16bit;计算得到刷新率61.7686fps,带宽47.4383MB/s
    // 同步宽度,前廊,后廊要设置好,不然出现部分显示问题
    LCD_Init();
    FT5xxx_Init();
}

/**
 * @brief	对函数简要描述
 * @param 	参数说明，以’:’作为参数结束标志；
 * @arg
 * 参数里面可选择参量列举，对于可数情况可进行参量列举，同样以’:’作为参数结束标志；
 * @note  	注释，配合brief一起使用可以达到很好的注释效果；
 * @retval	返回值说明。
 */
int main(void)
{
    if (HAL_Init()) // 初始化HAL库
    {
        Error_Handler();
    }
    bsp_init(); // 板级初始化

    while (1)
    {
        /*
        中断管理组4,中断优先级0~15;os优先级0~15
        configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY为8,管理优先级数值8~15的中断
        */
        freertos_enter(); // 进入os
    }
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    while (1)
    {
    }
}
