#include "cpu_task.h"

TaskHandle_t CPUTask_Handler = NULL; // 任务句柄

void vCPUTask(void *pvParameters)
{
    (void)pvParameters;

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        // uint8_t CPU_RunInfo[400]; // 保存任务运行时间信息

        // printf("---------------------------------------------\r\n");
        // printf("任务名      任务状态 优先级   剩余栈 任务序号\r\n");
        // printf("%s", CPU_RunInfo);
        // printf("---------------------------------------------\r\n");

        // memset(CPU_RunInfo, 0, 400); // 信息缓冲区清零

        // vTaskGetRunTimeStats((char *)&CPU_RunInfo);

        // printf("任务名       运行计数         利用率\r\n");
        // printf("%s", CPU_RunInfo);
        // printf("---------------------------------------------\r\n\n");
        vTaskDelayUntil(&xLastWakeTime, 500);
    }
}

