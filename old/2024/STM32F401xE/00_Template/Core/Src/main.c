/**
 * @file	  05_FreeRTOS_ADC+OLED
 * @brief 	移植FreeRTOS和ADC,OLED
 * @author 	TRTX-gamer      https://github.com/TRTX-gamer；
 *          突然吐血    https://space.bilibili.com/12890038;
 * @version 1.02
 * @date 	2022年9月28号20点54分
 */

/**
 * ADC采样注意参考电压VERF+接口，如果悬空，则一直为4095
 */
#include "main.h"

#define Debug 0 // 控制Debug的一些相关函数

/**
 * @brief   主函数,程序入口
 * @param   none
 * @arg		  none
 * @note    ADC,OLED
 * @retval  int
 */
int main(void)
{
  if (HAL_Init()) // 初始化HAL库
  {
    Error_Handler();
  }
  Stm32_Clock_Init(168U, 25U, 2U, 4U); // 初始化时钟
  delay_init(84);                      // 初始化延时函数
  LED_Init();                          // 初始化LED

  while (1)
  {
    delay_ms(250);
    LED0_Reverse();
    delay_ms(250);
    LED1_Reverse();
  }
}

#if Debug == 1
/**
 * @brief   栈溢出钩子函数
 * @param   xTask
 * @arg		  the task that just exceeded its stack boundaries.
 * @param   pcTaskName
 * @arg		  A character string containing the name of the offending task.
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

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
