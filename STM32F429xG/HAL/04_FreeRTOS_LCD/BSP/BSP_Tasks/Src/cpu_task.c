#include "cpu_task.h"
#include "tasks_sync.h"

TaskHandle_t CPUTask_Handler = NULL; // 任务句柄
char CPU_RunInfo[CPU_RUNINFO_SIZE];  // 保存任务运行时间信息
uint32_t CPUTaskCycleTime_ms = 1000; // 任务运行时间间隔

static char *prvWriteNameToBuffer(char *pcBuffer, const char *pcTaskName)
{
    size_t x;

    /* Start by copying the entire string. */
    strcpy(pcBuffer, pcTaskName);

    /* Pad the end of the string with spaces to ensure columns line up when
     * printed out. */
    for (x = strlen(pcBuffer); x < (size_t)(configMAX_TASK_NAME_LEN - 1); x++)
    {
        pcBuffer[x] = ' ';
    }

    /* Terminate. */
    pcBuffer[x] = (char)0x00;

    /* Return the new end of string. */
    return &(pcBuffer[x]);
}

static void vTaskGetRunTimeStats_Cycle(char *pcWriteBuffer, uint32_t CycleTime_ms, uint32_t TickFrequency)
{
    TaskStatus_t *pxTaskStatusArray = NULL;            // 任务状态结构体
    static TaskStatus_t *pxTaskStatusArrayLast = NULL; // 任务状态结构体
    float ulStatsAsPercentage;                         // 运行时间百分比
    UBaseType_t uxArraySize;                           // 任务数量
    static UBaseType_t uxArraySizeLast;                // 任务数量

    /* Make sure the write buffer does not contain a string. */
    *pcWriteBuffer = (char)0x00;

    /* Take a snapshot of the number of tasks in case it changes while this
     * function is executing. */
    uxArraySize = uxTaskGetNumberOfTasks();

    pxTaskStatusArray = pvPortMalloc(uxArraySize * sizeof(TaskStatus_t)); // 分配当前任务状态结构体空间
    if (pxTaskStatusArray != NULL)
    {
        if (pxTaskStatusArrayLast == NULL) // 第一次运行
        {
            pxTaskStatusArrayLast = pvPortMalloc(uxArraySize * sizeof(TaskStatus_t)); // 第一次运行时,分配上一次任务状态结构体空间
            if (pxTaskStatusArrayLast != NULL)
            {
            }
            else
            {
                mtCOVERAGE_TEST_MARKER(); // 错误处理
            }
        }
        else // 不是第一次运行
        {
            /* Generate the (binary) data. */
            uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, NULL);

            /* Create a human readable table from the binary data. */
            for (UBaseType_t x = 0; x < uxArraySize; x++)
            {
                for (UBaseType_t y = 0; y < uxArraySizeLast; y++)
                {
                    if (pxTaskStatusArray[x].pcTaskName == pxTaskStatusArrayLast[y].pcTaskName)
                    {
                        ulStatsAsPercentage = (float)(pxTaskStatusArray[x].ulRunTimeCounter - pxTaskStatusArrayLast[y].ulRunTimeCounter) / TickFrequency / ((float)CycleTime_ms / 1000) * 100.0f; // 运行时间百分比

                        pcWriteBuffer = prvWriteNameToBuffer(pcWriteBuffer, pxTaskStatusArray[x].pcTaskName);
                        sprintf(pcWriteBuffer, " %2u.%1u%%\r\n", (uint32_t)ulStatsAsPercentage, (uint32_t)(ulStatsAsPercentage * 10UL) % 10); // 定长

                        pcWriteBuffer += strlen(pcWriteBuffer); /*lint !e9016 Pointer arithmetic ok on char pointers especially as in this case where it best denotes the intent of the code. */
                    }
                }
            }
        }

        /* 保存当前任务信息到上一次任务信息 */
        TaskStatus_t *pxTaskStatusArrayLast_temp = NULL;
        pxTaskStatusArrayLast_temp = pvPortMalloc(uxArraySize * sizeof(TaskStatus_t)); // 这里的uxArraySize是当前运行的任务数量
        if (pxTaskStatusArrayLast_temp != NULL)
        {
            for (uint32_t i = 0; i < uxArraySize; i++) // 复制上一次到临时数组
            {
                pxTaskStatusArrayLast_temp[i] = pxTaskStatusArray[i];
            }
            // 交换指针
            vPortFree(pxTaskStatusArrayLast);                   // 上次数据要更新,不要了
            pxTaskStatusArrayLast = pxTaskStatusArrayLast_temp; // 换成复制的数据
        }
        else
        {
            mtCOVERAGE_TEST_MARKER(); // 错误处理
        }
        uxArraySizeLast = uxArraySize; // 保存当前任务数量

        /* 释放当前任务状态结构体空间 */
        vPortFree(pxTaskStatusArray);
    }
    else
    {
        mtCOVERAGE_TEST_MARKER();
    }
}

int sqrt_compare_(const void *a, const void *b)
{
    TaskStatus_t *s1 = (TaskStatus_t *)a;
    TaskStatus_t *s2 = (TaskStatus_t *)b;

    // 优先按分数降序，分数相同按ID升序
    // if (s1->ulRunTimeCounter > s2->ulRunTimeCounter)
    // {
    //     return (s2->ulRunTimeCounter > s1->ulRunTimeCounter) ? 1 : -1;
    // }
    // else
    // {
    //     return s1->id - s2->id;
    // }

    // if (fabs(s1->score - s2->score) > 1e-6)
    // {
    //     return (s2->score > s1->score) ? 1 : -1;
    // }
    // else
    // {
    //     return s1->id - s2->id;
    // }
}

void vCPUTask(void *pvParameters)
{
    (void)pvParameters;

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        taskENTER_CRITICAL(); // 进入临界区
        // vTaskGetRunTimeStats(CPU_RunInfo);
        vTaskGetRunTimeStats_Cycle(CPU_RunInfo, CPUTaskCycleTime_ms, 20000);
        taskEXIT_CRITICAL(); // 退出临界区
        xQueueSend(xQueue_CPU, CPU_RunInfo, 0);
        vTaskDelayUntil(&xLastWakeTime, CPUTaskCycleTime_ms);
    }
}
