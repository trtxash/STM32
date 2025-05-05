/**
 * @file	00_FreeRTOS_Tempalte
 * @brief
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2025年5月5号20点46分
 */

// 使用RTT需要开启 JLinkRTTClient，插好jlink，按下Ctrl+P，输入task[空格]rtt，即可启动RTT Client

#include "main.h"
#include "bsp_app.h"
#include "bsp_freertos.h"

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
        freertos_main(); // 进入os
    }
}
