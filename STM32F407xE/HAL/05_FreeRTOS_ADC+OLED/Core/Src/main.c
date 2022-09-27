/**
 * @file	  05_ADC+OLED
 * @brief 	移植FreeRTOS和OLED硬件
 * @author 	TRTX-gamer      https://github.com/TRTX-gamer；
 *          突然吐血    https://space.bilibili.com/12890038;
 * @version 1.01
 * @date 	2022年9月18号23点43分
 */

// 软件模拟，未开启GCC优化
// IIC最大214fps，延时TIM3_Init(469 - 1, 840 - 1);
// SPI最大720fps，延时TIM3_Init(139 - 1, 840 - 1);
// 硬件，未开启GCC优化
// IIC最大20fps，延时TIM3_Init(2500 - 1, 840 - 1);
// SPI最大495FPS，延时TIM3_Init(202 - 1, 840 - 1);
// DMA下FPS不好测，估算5,126FPS，实际写入可能低一点

/**
 * 软件模拟优点：波特率高，速度快，可移植性好
 *        缺点：占用管脚口，使用MCU资源多，不太稳定
 * 硬件优缺点和软件模拟相反
 *
 * 本次实验还可以继续优化
 *    优化方向1：不用HAL的SPI发送，用寄存器；
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
  u8 i;
  u32 Xvalue;
  u8 temp[10] = {0};

  if (HAL_Init()) // 初始化HAL库
  {
    Error_Handler();
  }
  Stm32_Clock_Init(168U, 4U, 2U, 4U); // 初始化时钟
  delay_init(168);                    // 初始化延时函数
  LED_Init();                         // 初始化LED
  // MX_DMA_Init();                      // 要先初始化DMA
  // MX_I2C1_Init();                    // 初始化i2c接口
  // MX_SPI1_Init();                     // 初始化MDA后再初始话SPI
  // MX_ADC1_Init();                     // 初始化ADC1
  // OLED_Init();                        // 初始化OLED
  uart_init(115200); // 初始化串口
  // TIM3_Init(202 - 1, 840 - 1);
  // TIM4_Init(10000 - 1, 8400 - 1); // 定时器3初始化，周期1s

  printf("OK!");
  // HAL_ADC_Start_DMA(&hadc1, (u32 *)value, 10);
  while (1)
  {
    LED0_Reverse();
    delay_ms(400);
    LED1_Reverse();
    delay_ms(100);
    printf("OK!");
    // for (i = 0; i < 10; i++)
    // {
    //   Xvalue += value[i];
    // }
    // Xvalue /= 10;
    // sprintf(temp, "%f", (float)Xvalue * 3.3 / 4096);
    // OLED_ShowString(88, 0, temp, 16, 1);
    // OLED_ShowNum(88, 16, Xvalue, 5, 16, 1);
  }
}

#ifdef Debug
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
