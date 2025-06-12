#include "start_task.h"
#include "adc_task.h"
#include "cpu_task.h"
#include "gui_task.h"
#include "key_task.h"
#include "led_task.h"
#include "tasks_sync.h"
#include "touch_task.h"

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

    // 创建Touch任务
    xTaskCreate((TaskFunction_t)vTouchTask,
                (const char *)"vTouchTask",
                (uint16_t)TOUCH_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK_PRIO_TOUCH,
                (TaskHandle_t *)&TouchTask_Handler);

    // 创建CPU任务
    xTaskCreate((TaskFunction_t)vCPUTask,
                (const char *)"vCPUTask",
                (uint16_t)CPU_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK_PRIO_CPU,
                (TaskHandle_t *)&CPUTask_Handler);

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
