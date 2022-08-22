/**
 * @file	  01_FreeRTOS+LED
 * @brief 	移植FreeRTOS和LED驱动
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	  2022年8月22号15点35分
 */

#include "main.h"

/**
 * @brief   主函数,程序入口
 * @param   none
 * @arg		  none
 * @note    循环点亮和熄灭LED
 * @retval  int
 */
int main(void)
{
  if (HAL_Init()) // 初始化HAL库
  {
    Error_Handler();
  }
  Stm32_Clock_Init(168, 4, 2, 4); // 初始化时钟
  delay_init(168);                // 初始化延时函数
  LED_Init();                     // 初始化LED

  while (1)
  {
    delay_ms(500);
    LED0_Reverse();
    delay_ms(500);
    LED1_Reverse();
  }
}

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
