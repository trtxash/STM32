#include "stm32f4xx_it_callback.h"
#include "adc.h"
#include "adc_task.h"
#include "i2c.h"
#include "lcd.h"
#include "tasks_common.h"
#include "tasks_sync.h"
#include "tim.h"
#include "touch_task.h"

#if SYSTEM_SUPPORT_OS

#endif

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM7 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
/* USER CODE BEGIN Callback 0 */

/* USER CODE END Callback 0 */
#if SYSTEM_SUPPORT_OS
    if (htim == (&htim7))
    {
        HAL_IncTick();
    }
#endif

    if (htim == (&htim14))
    {
        FreeRTOSRunTimeTicks++;
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1)
    {
        // 触摸数据接收完成后触发
        xSemaphoreGiveFromISR(xSemaphore_Touch_i2c, NULL);
    }
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    uint32_t error = HAL_I2C_GetError(hi2c);
    if (error & HAL_I2C_ERROR_AF)
    {
        // 处理ACK失败：复位I2C总线
        // HAL_I2C_Init(hi2c);
        // // 重新启动触摸数据轮询
        // I2C_StartTouchPolling();
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == (&hadc1))
    {
        xSemaphoreGiveFromISR(xSemaphore_ADC, NULL); // 释放信号量
    }
}

// void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
// {
//     // printf("DMA Half transfer completern");
// }

// void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
// {
//     // printf("DMA transfer errorrn");
// }

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    /* 栈溢出处理函数 */
    LTDC_Show_Char(400, 480 - 12, '!', 12, 0, GUI_Red);
    while (1)
    {
    }
}
