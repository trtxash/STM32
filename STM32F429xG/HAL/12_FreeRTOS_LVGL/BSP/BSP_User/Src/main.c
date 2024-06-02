/**
 * @file	11_FreeRTOS_FMC+SDRAM
 * @brief
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2024年5月25号17点45分
 */
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
  Stm32_Clock_Init(SYS_CLOCK, 12, RCC_PLLP_DIV2, 8); // 设置时钟
  delay_init(SYS_CLOCK);                             // 延时初始化
  bsp_init();                                        // 板级初始化
  freertos_main();                                   // 进入os
}