#include "cpu_task.h"
#include "tasks_sync.h"

TaskHandle_t CPUTask_Handler = NULL; // 任务句柄
char CPU_RunInfo[CPU_RUNINFO_SIZE];  // 保存任务运行时间信息

void vCPUTask(void *pvParameters)
{
    (void)pvParameters;

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        vTaskGetRunTimeStats(CPU_RunInfo);
        xQueueSend(xQueue_CPU, CPU_RunInfo, 10);
        vTaskDelayUntil(&xLastWakeTime, 500);
    }
}
