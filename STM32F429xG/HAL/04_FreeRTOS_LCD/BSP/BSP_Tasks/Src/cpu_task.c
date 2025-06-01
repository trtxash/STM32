#include "cpu_task.h"

TaskHandle_t CPUTask_Handler = NULL; // 任务句柄
char CPU_RunInfo[512];               // 保存任务运行时间信息

void vCPUTask(void *pvParameters)
{
    (void)pvParameters;

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        vTaskGetRunTimeStats(CPU_RunInfo);

        vTaskDelayUntil(&xLastWakeTime, 500);
    }
}
