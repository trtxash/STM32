#include "touch_task.h"
#include "lcd.h"
#include "tasks_sync.h"

TaskHandle_t TouchTask_Handler = NULL; // 任务句柄
uint8_t Touch_Data[Touch_Data_Len];    // 触摸屏数据

void vTouchTask(void *pvParameters)
{
    (void)pvParameters;
    uint16_t xp = 0;
    uint16_t yp = 0;
    uint16_t tp = 0;

    if (HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_READY)
        HAL_I2C_Mem_Read_DMA(&hi2c1, FT_ADDRESS << 1U, FT_DEVIDE_MODE, I2C_MEMADD_SIZE_8BIT, Touch_Data, Touch_Data_Len);

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        // // 读取触摸屏数据
        // for (int i = 0; i < 3; i++)
        // {
        //     LTDC_Show_Num(400 + xp, 240, Touch_Data[i], 3, 12, 0, GUI_Red);
        //     xp += 6 * 3;
        //     LTDC_Show_Char(400 + xp, 240, ',', 12, 0, GUI_Black);
        //     xp += 6;
        // }
        // xp = yp = tp = 0;
        // for (int j = 0; j < 10; j++)
        // {
        //     for (int i = 0; i < 6; i++)
        //     {
        //         LTDC_Show_Num(400 + xp, 240 + 12 + yp, Touch_Data[3 + tp], 3, 12, 0, GUI_Black);
        //         tp++;
        //         xp += 6 * 3;
        //         LTDC_Show_Char(400 + xp, 240 + 12 + yp, ',', 12, 0, GUI_Black);
        //         xp += 6;
        //     }
        //     xp = 0;
        //     yp += 12;
        // }

        vTaskDelayUntil(&xLastWakeTime, 5);
        if (HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_READY)
            HAL_I2C_Mem_Read_DMA(&hi2c1, FT_ADDRESS << 1U, FT_DEVIDE_MODE, I2C_MEMADD_SIZE_8BIT, Touch_Data, Touch_Data_Len);
    }
}
