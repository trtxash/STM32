#include "cpu_task.h"
#include "tasks_sync.h"

TaskHandle_t CPUTask_Handler = NULL; // 任务句柄
char CPU_RunInfo[CPU_RUNINFO_SIZE];  // 保存任务运行时间信息

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

static void vTaskGetRunTimeStats_Cycle(char *pcWriteBuffer, uint16_t CycleTime_ms, uint32_t TickFrequency)
{
    TaskStatus_t *pxTaskStatusArray = NULL;
    float ulStatsAsPercentage;
    UBaseType_t uxArraySize;

    static UBaseType_t ulLastArraySize = 0;
    static configRUN_TIME_COUNTER_TYPE *ulLastulRunTimeCounterArray = NULL;

    /* Make sure the write buffer does not contain a string. */
    *pcWriteBuffer = (char)0x00;

    /* Take a snapshot of the number of tasks in case it changes while this
     * function is executing. */
    uxArraySize = uxTaskGetNumberOfTasks();

    /* Allocate an array index for each task.  NOTE!  If
     * configSUPPORT_DYNAMIC_ALLOCATION is set to 0 then pvPortMalloc() will
     * equate to NULL. */
    pxTaskStatusArray = pvPortMalloc(uxTaskGetNumberOfTasks() * sizeof(TaskStatus_t));

    // if (ulLastulRunTimeCounterArray == NULL) // 第一次运行
    // {
    //     ulLastulRunTimeCounterArray = (configRUN_TIME_COUNTER_TYPE *)pvPortMalloc(uxArraySize * sizeof(configRUN_TIME_COUNTER_TYPE));
    //     if (ulLastulRunTimeCounterArray == NULL)
    //     {
    //         mtCOVERAGE_TEST_MARKER(); // 错误处理
    //     }
    // }
    // else
    // {
    //     configRUN_TIME_COUNTER_TYPE *ulLastulRunTimeCounterArray_temp = NULL;
    //     ulLastulRunTimeCounterArray_temp = (configRUN_TIME_COUNTER_TYPE *)pvPortMalloc(uxArraySize * sizeof(configRUN_TIME_COUNTER_TYPE));
    //     if (ulLastulRunTimeCounterArray_temp == NULL)
    //     {
    //         mtCOVERAGE_TEST_MARKER(); // 错误处理
    //     }
    //     for (uint32_t i = 0; i < ulLastArraySize; i++) // 复制上一次运行时间到临时数组
    //     {
    //         ulLastulRunTimeCounterArray_temp[i] = ulLastulRunTimeCounterArray[i];
    //     }
    //     // 交换指针
    //     configRUN_TIME_COUNTER_TYPE *ulLastulRunTimeCounterArray_temp_temp = ulLastulRunTimeCounterArray_temp;
    //     ulLastulRunTimeCounterArray_temp = ulLastulRunTimeCounterArray;
    //     ulLastulRunTimeCounterArray = ulLastulRunTimeCounterArray_temp_temp;
    //     vPortFree(ulLastulRunTimeCounterArray_temp);
    // }
    // vPortFree(ulLastulRunTimeCounterArray);

    if (pxTaskStatusArray != NULL)
    {
        /* Generate the (binary) data. */
        uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, NULL);

        /* Create a human readable table from the binary data. */
        // for (UBaseType_t x = 0; x < uxArraySize; x++)
        // {
        //     ulStatsAsPercentage = (float)(pxTaskStatusArray[x].ulRunTimeCounter) / ulTotalTime;

        //     pcWriteBuffer = prvWriteNameToBuffer(pcWriteBuffer, pxTaskStatusArray[x].pcTaskName);
        //     sprintf(pcWriteBuffer, "%10u\t%2u.%1u%%\t\r\n", (unsigned int)pxTaskStatusArray[x].ulRunTimeCounter, (uint32_t)ulStatsAsPercentage, (uint32_t)(ulStatsAsPercentage * 10UL) % 10); // 定长

        //     pcWriteBuffer += strlen(pcWriteBuffer); /*lint !e9016 Pointer arithmetic ok on char pointers especially as in this case where it best denotes the intent of the code. */

        //     ulLastulRunTimeCounter = pxTaskStatusArray[x].ulRunTimeCounter;
        // }

        /* Create a human readable table from the binary data. */
        // for (UBaseType_t x = 0; x < uxArraySize; x++)
        // {
        //     ulStatsAsPercentage = (float)(pxTaskStatusArray[x].ulRunTimeCounter) / ulTotalTime;

        //     pcWriteBuffer = prvWriteNameToBuffer(pcWriteBuffer, pxTaskStatusArray[x].pcTaskName);
        //     sprintf(pcWriteBuffer, "%10u\t%2u.%1u%%\t\r\n", (unsigned int)pxTaskStatusArray[x].ulRunTimeCounter, (uint32_t)ulStatsAsPercentage, (uint32_t)(ulStatsAsPercentage * 10UL) % 10); // 定长

        //     pcWriteBuffer += strlen(pcWriteBuffer); /*lint !e9016 Pointer arithmetic ok on char pointers especially as in this case where it best denotes the intent of the code. */

        //     ulLastulRunTimeCounter = pxTaskStatusArray[x].ulRunTimeCounter;
        // }

        vPortFree(pxTaskStatusArray);
    }
    else
    {
        mtCOVERAGE_TEST_MARKER();
    }

    // 保存运行时间信息
    ulLastArraySize = uxArraySize;
}

int sqrt_compare(const void *a, const void *b)
{
    TaskStatus_t *s1 = (TaskStatus_t *)a;
    TaskStatus_t *s2 = (TaskStatus_t *)b;

    // // 优先按分数降序，分数相同按ID升序
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
        vTaskGetRunTimeStats(CPU_RunInfo);
        // vTaskGetRunTimeStats_Cycle(CPU_RunInfo, 0, 0);
        taskEXIT_CRITICAL(); // 退出临界区
        xQueueSend(xQueue_CPU, CPU_RunInfo, 0);
        vTaskDelayUntil(&xLastWakeTime, 1000);
    }
}
