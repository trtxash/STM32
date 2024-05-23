/**
 * @file	06_SEGGER_RTT
 * @brief
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2024年5月17号14点41分
 */

// 未完，有时间再补

#include "main.h"

/**
 * @brief	对函数简要描述
 * @param 	参数说明，以’:’作为参数结束标志；
 * @arg		参数里面可选择参量列举，对于可数情况可进行参量列举，同样以’:’作为参数结束标志；
 * @note  	注释，配合brief一起使用可以达到很好的注释效果；
 * @retval	返回值说明。
 */
int main(void)
{
    if (HAL_Init()) // 初始化HAL库
    {
        Error_Handler();
    }
    Stm32_Clock_Init(180, 12, RCC_PLLP_DIV2, 8); // 设置时钟
    delay_init(180);
    bsp_init();

    while (1)
    {
        static uint32_t i = 0;
        LED0_Reverse();
        // LOGE("ERR");
        LOGI("Inf");
        // LOGW("WAR");
        SEGGER_RTT_printf(0, "%shelow:%u\r\n%s", RTT_CTRL_TEXT_BRIGHT_BLUE, i, RTT_CTRL_RESET);
        SEGGER_RTT_printf(0, "%shelow:%u\r\n%s", RTT_CTRL_TEXT_BRIGHT_RED, i, RTT_CTRL_RESET);
        delay_ms(500);
        i++;
    }
}

#if Debug == 1
/**
 * @brief   栈溢出钩子函数
 * @param   xTask
 * @arg		the task that just exceeded its stack boundaries.
 * @param   pcTaskName
 * @arg		A character string containing the name of the offending task.
 * @note    FreeRTOS打印栈溢出的任务，关联#define configCHECK_FOR_STACK_OVERFLOW 2，在FreeRTOSConfig.h下
 * @retval  void
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
    called if a stack overflow is detected. */
    printf("任务：%s 溢出\r\n", pcTaskName);
}
#endif