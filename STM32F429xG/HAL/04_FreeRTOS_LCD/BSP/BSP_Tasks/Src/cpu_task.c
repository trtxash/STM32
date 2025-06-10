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

// 按ID升序
static int sqrt_compare_id_up(const void *a, const void *b)
{
    TaskStatus_t_CPU_task *s1 = (TaskStatus_t_CPU_task *)a;
    TaskStatus_t_CPU_task *s2 = (TaskStatus_t_CPU_task *)b;

    return s1->xTaskStatus.xTaskNumber - s2->xTaskStatus.xTaskNumber;
}

// 按ID降序
static int sqrt_compare_id_down(const void *a, const void *b)
{
    TaskStatus_t_CPU_task *s1 = (TaskStatus_t_CPU_task *)a;
    TaskStatus_t_CPU_task *s2 = (TaskStatus_t_CPU_task *)b;

    return s2->xTaskStatus.xTaskNumber - s1->xTaskStatus.xTaskNumber;
}

// 按任务名称升序
static int sqrt_compare_name_up(const void *a, const void *b)
{
    TaskStatus_t_CPU_task *s1 = (TaskStatus_t_CPU_task *)a;
    TaskStatus_t_CPU_task *s2 = (TaskStatus_t_CPU_task *)b;

    return strcmp(s1->xTaskStatus.pcTaskName, s2->xTaskStatus.pcTaskName);
}

// 按任务名称降序
static int sqrt_compare_name_down(const void *a, const void *b)
{
    TaskStatus_t_CPU_task *s1 = (TaskStatus_t_CPU_task *)a;
    TaskStatus_t_CPU_task *s2 = (TaskStatus_t_CPU_task *)b;

    return strcmp(s2->xTaskStatus.pcTaskName, s1->xTaskStatus.pcTaskName);
}

// 按CPU占用降序
static int sqrt_compare_cpu_down(const void *a, const void *b)
{
    TaskStatus_t_CPU_task *s1 = (TaskStatus_t_CPU_task *)a;
    TaskStatus_t_CPU_task *s2 = (TaskStatus_t_CPU_task *)b;

    if (s2->ulStatsAsPercentage != s1->ulStatsAsPercentage)
        return (s2->ulStatsAsPercentage > s1->ulStatsAsPercentage) ? 1 : ((s2->ulStatsAsPercentage < s1->ulStatsAsPercentage) ? -1 : 0);
    else
        return s2->xTaskStatus.xTaskNumber - s1->xTaskStatus.xTaskNumber;
}

// 按CPU占用升序
static int sqrt_compare_cpu_up(const void *a, const void *b)
{
    TaskStatus_t_CPU_task *s1 = (TaskStatus_t_CPU_task *)a;
    TaskStatus_t_CPU_task *s2 = (TaskStatus_t_CPU_task *)b;

    if (s2->ulStatsAsPercentage != s1->ulStatsAsPercentage)
        return (s1->ulStatsAsPercentage > s2->ulStatsAsPercentage) ? 1 : ((s1->ulStatsAsPercentage < s2->ulStatsAsPercentage) ? -1 : 0);
    else
        return s1->xTaskStatus.xTaskNumber - s2->xTaskStatus.xTaskNumber;
}

// 按运行时间降序
static int sqrt_compare_time_down(const void *a, const void *b)
{
    TaskStatus_t_CPU_task *s1 = (TaskStatus_t_CPU_task *)a;
    TaskStatus_t_CPU_task *s2 = (TaskStatus_t_CPU_task *)b;

    return s2->xTaskStatus.ulRunTimeCounter - s1->xTaskStatus.ulRunTimeCounter;
}

// 按运行时间升序
static int sqrt_compare_time_up(const void *a, const void *b)
{
    TaskStatus_t_CPU_task *s1 = (TaskStatus_t_CPU_task *)a;
    TaskStatus_t_CPU_task *s2 = (TaskStatus_t_CPU_task *)b;

    return s1->xTaskStatus.ulRunTimeCounter - s2->xTaskStatus.ulRunTimeCounter;
}

typedef int (*CompareFunc)(const void *, const void *);
static const CompareFunc sortFuncs[] = {
    sqrt_compare_id_down, sqrt_compare_id_up,
    sqrt_compare_name_down, sqrt_compare_name_up,
    sqrt_compare_cpu_down, sqrt_compare_cpu_up,
    sqrt_compare_time_down, sqrt_compare_time_up};

// mode:
// 0:按ID降序
// 1:按ID升序
// 2:按任务名称降序
// 3:按任务名称升序
// 4:按CPU占用降序
// 5:按CPU占用升序
// 6:按运行时间降序
// 7:按运行时间升序
static void vTaskGetRunTimeStats_Sort(char *pcWriteBuffer, uint8_t sortmode, uint32_t CycleTime_ms, uint32_t TickFrequency)
{
    TaskStatus_t *pxTaskStatusArray = NULL; // 任务状态结构体数组,用于uxTaskGetSystemState()函数
    static TaskStatus_t *pxTaskStatusArrayLast = NULL;
    UBaseType_t uxArraySize; // 任务数量
    static UBaseType_t uxArraySizeLast;

    /* Make sure the write buffer does not contain a string. */
    *pcWriteBuffer = (char)0x00;

    /* Take a snapshot of the number of tasks in case it changes while this
     * function is executing. */
    uxArraySize = uxTaskGetNumberOfTasks();

    pxTaskStatusArray = pvPortMalloc(uxArraySize * sizeof(TaskStatus_t)); // 分配当前任务状态结构体数组空间
    if (pxTaskStatusArray != NULL)
    {
        if (pxTaskStatusArrayLast == NULL) // 第一次运行
        {
            pxTaskStatusArrayLast = pvPortMalloc(uxArraySize * sizeof(TaskStatus_t)); // 第一次运行时,分配上一次任务状态结构体数组空间
            if (pxTaskStatusArrayLast != NULL)
            {
                uxTaskGetSystemState(pxTaskStatusArrayLast, uxArraySize, NULL); // 初始化状态
                uxArraySizeLast = uxArraySize;                                  // 保存当前任务数量

                // 进行下一次运行...
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

            /* 生成顶层任务状态结构体数组 */
            TaskStatus_t_CPU_task *pxTaskStatusArray_CPU_task = pvPortMalloc(uxArraySize * sizeof(TaskStatus_t_CPU_task)); // 分配最上层任务状态结构体数组空间
            if (pxTaskStatusArray_CPU_task != NULL)
            {
                /* Calculate the percentage */
                for (UBaseType_t x = 0; x < uxArraySize; x++)
                {
                    for (UBaseType_t y = 0; y < uxArraySizeLast; y++)
                    {
                        if (strcmp(pxTaskStatusArray[x].pcTaskName, pxTaskStatusArrayLast[y].pcTaskName) == 0)
                        {
                            pxTaskStatusArray_CPU_task[x].ulStatsAsPercentage = (float)(pxTaskStatusArray[x].ulRunTimeCounter - pxTaskStatusArrayLast[y].ulRunTimeCounter) / TickFrequency / ((float)CycleTime_ms / 1000) * 100.0f; // 运行时间百分比
                        }
                    }
                }
                /* Copy the TaskStatus_t to TaskStatus_t_CPU_task */
                for (UBaseType_t x = 0; x < uxArraySize; x++)
                {
                    pxTaskStatusArray_CPU_task[x].xTaskStatus = pxTaskStatusArray[x];
                }

                /* Sort the array. */
                if (sortmode < sizeof(sortFuncs) / sizeof(sortFuncs[0]))
                {
                    qsort(pxTaskStatusArray_CPU_task, uxArraySize, sizeof(TaskStatus_t_CPU_task), sortFuncs[sortmode]);
                }

                /* Create a human readable table from the binary data. */
                for (UBaseType_t x = 0; x < uxArraySize; x++)
                {
                    pcWriteBuffer = prvWriteNameToBuffer(pcWriteBuffer, pxTaskStatusArray_CPU_task[x].xTaskStatus.pcTaskName);
                    sprintf(pcWriteBuffer, " %2u.%1u%%\r\n", (uint32_t)pxTaskStatusArray_CPU_task[x].ulStatsAsPercentage, (uint32_t)(pxTaskStatusArray_CPU_task[x].ulStatsAsPercentage * 10UL) % 10); // 定长
                    pcWriteBuffer += strlen(pcWriteBuffer);                                                                                                                                           /*lint !e9016 Pointer arithmetic ok on char pointers especially as in this case where it best denotes the intent of the code. */
                }

                vPortFree(pxTaskStatusArray_CPU_task); // 释放最上层任务状态结构体数组空间
            }
            else
            {
                mtCOVERAGE_TEST_MARKER();
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
        }

        /* 释放当前任务状态结构体空间 */
        vPortFree(pxTaskStatusArray);
    }
    else
    {
        mtCOVERAGE_TEST_MARKER();
    }
}

void vCPUTask(void *pvParameters)
{
    (void)pvParameters;

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        vTaskGetRunTimeStats_Sort(CPU_RunInfo, 4, CPU_TaskCycleTime_ms, 20000);

        xQueueOverwrite(xQueue_CPU, CPU_RunInfo);
        vTaskDelayUntil(&xLastWakeTime, CPU_TaskCycleTime_ms);
    }
}
