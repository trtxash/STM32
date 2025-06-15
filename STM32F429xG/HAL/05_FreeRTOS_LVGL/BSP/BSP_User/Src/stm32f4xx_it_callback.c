#include "stm32f4xx_it_callback.h"
#include "adc.h"
#include "adc_task.h"
#include "dma2d.h"
#include "i2c.h"
#include "lcd.h"
#include "ltdc.h"
#include "lv_tick.h"
#include "tasks_common.h"
#include "tasks_sync.h"
#include "tim.h"
#include "touch_task.h"

#include "lvgl.h"

#if SYSTEM_SUPPORT_OS

#endif

extern volatile uint8_t g_gpu_state;
extern volatile uint8_t g_gpu_state_temp;

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

// void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
// {
//     uint32_t error = HAL_I2C_GetError(hi2c);
//     if (error & HAL_I2C_ERROR_AF)
//     {
//         // 处理ACK失败：复位I2C总线
//         // HAL_I2C_Init(hi2c);
//         // // 重新启动触摸数据轮询
//         // I2C_StartTouchPolling();
//     }
// }

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

void HAL_DMA2D_TransferCpltCallback(DMA2D_HandleTypeDef *hdma2d)
{
    // HAL_LTDC_SetAddress(&hltdc, (uint32_t)lv_display_get_buf_active(lv_display_get_default())->data, 0);
    // lv_display_flush_ready(lv_display_get_default());
}

void HAL_LTDC_LineEventCallback(LTDC_HandleTypeDef *hltdc)
{
    if (hltdc == (&hltdc1))
    {
#if 0
        /* 按消费者强制即时更新，大部分数据在中断处理 */
        // 短板在SDRAM带宽和LTDC频率
        // 这里只有可能LTDC慢，导致LVGL自动覆写了另一个buff，产生闪屏
        if (hltdc1.LayerCfg[0].FBStartAdress == (uint32_t)ltdc_framebuf[0]) // 检测写完了哪个buf
            g_gpu_state_temp = 0;
        else
            g_gpu_state_temp = 1;

        if (g_gpu_state == 0 && g_gpu_state_temp == 0) // 算完buf0，且LTDC写完buf0
        {
            lv_display_flush_ready(lv_display_get_default()); // 去算buf1
        }
        else if (g_gpu_state == 1 && g_gpu_state_temp == 0) // 算完buf1，且LTDC写完buf0
        {
            HAL_LTDC_SetAddress(&hltdc1, (uint32_t)ltdc_framebuf[g_gpu_state], 0); // 切换buf1
            lv_display_flush_ready(lv_display_get_default());                      // 去算buf0
        }
        else if (g_gpu_state == 0 && g_gpu_state_temp == 1) // 算完buf0，且LTDC写完buf1
        {
            HAL_LTDC_SetAddress(&hltdc1, (uint32_t)ltdc_framebuf[g_gpu_state], 0); // 切换buf0
            lv_display_flush_ready(lv_display_get_default());                      // 去算buf1
        }
        else if (g_gpu_state == 1 && g_gpu_state_temp == 1) // 算完buf1，且LTDC写完buf1
        {
            lv_display_flush_ready(lv_display_get_default()); // 去算buf0
        }
#endif
        xSemaphoreGiveFromISR(xSemaphore_VSync, NULL); // 释放信号量

        __HAL_LTDC_ENABLE_IT(hltdc, LTDC_IT_LI);
    }
}

void HAL_LTDC_ReloadEventCallback(LTDC_HandleTypeDef *hltdc)
{
    if (hltdc == (&hltdc1))
    {
        xSemaphoreGiveFromISR(xSemaphore_VSync, NULL);           // 释放信号量
        HAL_LTDC_Reload(&hltdc1, LTDC_RELOAD_VERTICAL_BLANKING); // 请求重载
    }
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    /* 栈溢出处理函数 */
    LTDC_Show_Char(400, 480 - 12, '!', 12, 0, GUI_Red);
    while (1)
    {
    }
}
