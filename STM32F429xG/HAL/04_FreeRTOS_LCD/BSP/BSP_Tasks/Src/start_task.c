#include "start_task.h"
#include "adc_task.h"
#include "gui_task.h"
#include "key_task.h"
#include "led_task.h"
#include "tasks_sync.h"

TaskHandle_t StartTask_Handler;      // 任务句柄
void vStartTask(void *pvParameters); // 任务函数

void freertos_enter(void)
{
    // 同步资源初始化
    vSyncResources_Init();

    // 创建开始任务
    xTaskCreate((TaskFunction_t)vStartTask,          // 任务函数
                (const char *)"vStartTask",          // 任务名称
                (uint16_t)START_STK_SIZE,            // 任务堆栈大小
                (void *)NULL,                        // 传递给任务函数的参数
                (UBaseType_t)TASK_PRIO_START,        // 任务优先级
                (TaskHandle_t *)&StartTask_Handler); // 任务句柄
    vTaskStartScheduler();
}

// 开始任务任务函数
void vStartTask(void *pvParameters)
{
    (void)pvParameters; // 明确标记未使用参数

    taskENTER_CRITICAL(); // 进入临界区
    // 创建LED0任务
    xTaskCreate((TaskFunction_t)vLedTask,
                (const char *)"vLedTask",
                (uint16_t)LED_TSTK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK_PRIO_LED,
                (TaskHandle_t *)&LEDTask_Handler);

    // 创建KEY任务
    xTaskCreate((TaskFunction_t)vKeyTask,
                (const char *)"vKeyTask",
                (uint16_t)KEY_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK_PRIO_KEY,
                (TaskHandle_t *)&KEYTask_Handler);

    // 创建GUI任务
    xTaskCreate((TaskFunction_t)vGUITask,
                (const char *)"vGUITask",
                (uint16_t)GUI_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK_PRIO_GUI,
                (TaskHandle_t *)&GUITask_Handler);

    // // 创建CPU任务
    // xTaskCreate((TaskFunction_t)cpu_task,
    //             (const char *)"cpu_task",
    //             (uint16_t)CPU_TSTK_SIZE,
    //             (void *)NULL,
    //             (UBaseType_t)CPU_TASK_PRIO,
    //             (TaskHandle_t *)&CPU_Task_Handler);

    // 创建adc任务
    xTaskCreate((TaskFunction_t)vADCTask,
                (const char *)"vADCTask",
                (uint16_t)ADC_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK_PRIO_ADC,
                (TaskHandle_t *)&ADCTask_Handler);
    taskEXIT_CRITICAL(); // 退出临界区

    vTaskDelete(StartTask_Handler); // 删除开始任务
}

// void cpu_task(void *pvParameters)
// {
//     (void)pvParameters; // 明确标记未使用参数

//     // uint8_t CPU_RunInfo[400]; // 保存任务运行时间信息
//     TickType_t xLastWakeTime;
//     xLastWakeTime = xTaskGetTickCount();
//     while (1)
//     {
//         // printf("---------------------------------------------\r\n");
//         // printf("任务名      任务状态 优先级   剩余栈 任务序号\r\n");
//         // printf("%s", CPU_RunInfo);
//         // printf("---------------------------------------------\r\n");

//         // memset(CPU_RunInfo, 0, 400); // 信息缓冲区清零

//         // vTaskGetRunTimeStats((char *)&CPU_RunInfo);

//         // printf("任务名       运行计数         利用率\r\n");
//         // printf("%s", CPU_RunInfo);
//         // printf("---------------------------------------------\r\n\n");
//         vTaskDelayUntil(&xLastWakeTime, 500);
//     }
// }
