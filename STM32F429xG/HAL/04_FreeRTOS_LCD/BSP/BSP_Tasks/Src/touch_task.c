#include "touch_task.h"
#include "tasks_sync.h"

TaskHandle_t TouchTask_Handler = NULL; // 任务句柄

uint8_t Touch_Data[Touch_Data_Len]; // 触摸屏数据

void vTouchTask(void *pvParameters)
{
    (void)pvParameters;

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        if (HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_READY)
            HAL_I2C_Mem_Read_DMA(&hi2c1, FT_ADDRESS << 1U, FT_REG_NUM_FINGER, I2C_MEMADD_SIZE_8BIT, Touch_Data, Touch_Data_Len);
        vTaskDelayUntil(&xLastWakeTime, TOUCH_TaskCycleTime_ms);
        xSemaphoreTake(xSemaphore_Touch_i2c, 0); // 等待信号量
        xQueueSend(xQueue_Touch, &Touch_Data, 0);
    }
}
