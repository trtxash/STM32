#include "bsp_freertos.h"
#include "SEGGER_RTT.h"
#include "adc.h"
#include "filter.h"
#include "lcd.h"
#include "stm32f4xx_hal_adc.h"
#include "task.h"

#define KeyQueueLen 16
QueueHandle_t xQueue_KEY = NULL;
QueueHandle_t xSemaphore_ADC = NULL;

volatile uint32_t JS_RTT_UpBuffer[1024] = {0};

/*
中断管理组4,中断优先级0~15;os优先级0~15
configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY为8,管理优先级数值8~15的中断
*/

#define START_TASK_PRIO 1       // 任务优先级,越大越高优先级
#define START_STK_SIZE  128 + 1 // 任务堆栈大小
TaskHandle_t StartTask_Handler; // 任务句柄
// void start_task(void *pvParameters); // 任务函数
void start_task(void *pvParameters); // 明确标记未使用参数; // 任务函数

#define LED_TASK_PRIO 1            // 任务优先级,越大越高优先级
#define LED_TSTK_SIZE 64 + 1       // 任务堆栈大小,实际为32word=32*4byte=128byte=128*8bit
TaskHandle_t LEDTask_Handler;      // 任务句柄
void led_task(void *pvParameters); // 任务函数

#define KEY_TASK_PRIO 3            // 任务优先级,越大越高优先级
#define KEY_TSTK_SIZE 256 + 1      // 任务堆栈大小,实际为32word=32*4byte=128byte=128*8bit
TaskHandle_t KEYTask_Handler;      // 任务句柄
void key_task(void *pvParameters); // 任务函数

#define GUI_TASK_PRIO 2            // 任务优先级,越大越高优先级
#define GUI_TSTK_SIZE 256 + 1      // 任务堆栈大小
TaskHandle_t GUI_Task_Handler;     // 任务句柄
void gui_task(void *pvParameters); // 任务函数

#define CPU_TASK_PRIO 2            // 任务优先级,越大越高优先级
#define CPU_TSTK_SIZE 256 + 1      // 任务堆栈大小
TaskHandle_t CPU_Task_Handler;     // 任务句柄
void cpu_task(void *pvParameters); // 任务函数

#define ADC_TASK_PRIO 2            // 任务优先级,越大越高优先级
#define ADC_TSTK_SIZE 512 + 1      // 任务堆栈大小
TaskHandle_t ADCTask_Handler;      // 任务句柄
void adc_task(void *pvParameters); // 任务函数

void freertos_main(void)
{
    // 创建开始任务
    xTaskCreate((TaskFunction_t)start_task,          // 任务函数
                (const char *)"start_task",          // 任务名称
                (uint16_t)START_STK_SIZE,            // 任务堆栈大小
                (void *)NULL,                        // 传递给任务函数的参数
                (UBaseType_t)START_TASK_PRIO,        // 任务优先级
                (TaskHandle_t *)&StartTask_Handler); // 任务句柄
    vTaskStartScheduler();
}

// 开始任务任务函数
void start_task(void *pvParameters)
{
    (void)pvParameters; // 明确标记未使用参数

    taskENTER_CRITICAL(); // 进入临界区
    // 创建LED0任务
    xTaskCreate((TaskFunction_t)led_task,
                (const char *)"led_task",
                (uint16_t)LED_TSTK_SIZE,
                (void *)NULL,
                (UBaseType_t)LED_TASK_PRIO,
                (TaskHandle_t *)&LEDTask_Handler);

    // 创建KEY任务
    xTaskCreate((TaskFunction_t)key_task,
                (const char *)"key_task",
                (uint16_t)KEY_TSTK_SIZE,
                (void *)NULL,
                (UBaseType_t)KEY_TASK_PRIO,
                (TaskHandle_t *)&KEYTask_Handler);

    // 创建GUI任务
    xTaskCreate((TaskFunction_t)gui_task,
                (const char *)"gui_task",
                (uint16_t)GUI_TSTK_SIZE,
                (void *)NULL,
                (UBaseType_t)GUI_TASK_PRIO,
                (TaskHandle_t *)&GUI_Task_Handler);

    // 创建CPU任务
    xTaskCreate((TaskFunction_t)cpu_task,
                (const char *)"cpu_task",
                (uint16_t)CPU_TSTK_SIZE,
                (void *)NULL,
                (UBaseType_t)CPU_TASK_PRIO,
                (TaskHandle_t *)&CPU_Task_Handler);

    // 创建test任务
    xTaskCreate((TaskFunction_t)adc_task,
                (const char *)"adc_task",
                (uint16_t)ADC_TSTK_SIZE,
                (void *)NULL,
                (UBaseType_t)ADC_TASK_PRIO,
                (TaskHandle_t *)&ADCTask_Handler);
    taskEXIT_CRITICAL(); // 退出临界区

    vTaskDelete(StartTask_Handler); // 删除开始任务
}

// led任务函数
void led_task(void *pvParameters)
{
    (void)pvParameters; // 明确标记未使用参数

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        LED0_Reverse();
        vTaskDelayUntil(&xLastWakeTime, 250);
    }
}

static void Callback_Single_Click_Handler(void *btn)
{
    if (btn == &button_up)
    {
        PressEvent event = SINGLE_CLICK;
        xQueueSend(xQueue_KEY, &event, 10);
    }
}

static void Callback_Double_Click_Handler(void *btn)
{
    if (btn == &button_up)
    {
        PressEvent event = DOUBLE_CLICK;
        xQueueSend(xQueue_KEY, &event, 10);
    }
}

// key任务函数
void key_task(void *pvParameters)
{
    (void)pvParameters; // 明确标记未使用参数

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    xQueue_KEY = xQueueCreate(KeyQueueLen, sizeof(PressEvent));
    button_attach(&button_up, SINGLE_CLICK, Callback_Single_Click_Handler);
    button_attach(&button_up, DOUBLE_CLICK, Callback_Double_Click_Handler);
    button_start(&button_up);
    while (1)
    {
        button_ticks();
        vTaskDelayUntil(&xLastWakeTime, 50);
    }
}

// gui任务函数
void gui_task(void *pvParameters)
{
    (void)pvParameters; // 明确标记未使用参数

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        static uint32_t lineflag = 0;

        PressEvent key_event;
        if (xQueueReceive(xQueue_KEY, &key_event, 10) == pdPASS)
        {
            if (lineflag <= 468)
            {
                if (key_event == SINGLE_CLICK)
                    LTDC_Show_String(0, lineflag, 240, 12, 12, (u8 *)"Single Click", 1, GUI_Black);
                else if (key_event == DOUBLE_CLICK)
                    LTDC_Show_String(0, lineflag, 240, 240, 12, (u8 *)"Double Click", 1, GUI_Black);
                else
                    LTDC_Show_String(0, lineflag, 240, 240, 12, (u8 *)"No Event", 1, GUI_Black);
                LTDC_Show_String(14 * 6, lineflag, 240, 240, 12, (u8 *)",Line", 1, GUI_Black);
                LTDC_Show_Num(20 * 6, lineflag, lineflag, 3, 12, 1, GUI_Black);
                lineflag += 12;
            }
        }
        vTaskDelayUntil(&xLastWakeTime, 5);
    }
}

void cpu_task(void *pvParameters)
{
    (void)pvParameters; // 明确标记未使用参数

    uint8_t CPU_RunInfo[400]; // 保存任务运行时间信息
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
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

// 测试任务函数
void adc_task(void *pvParameters)
{
    (void)pvParameters; // 明确标记未使用参数

    xSemaphore_ADC = xSemaphoreCreateBinary(); // 创建二值信号量

    Kalman adcx_kalman;
    Kalman_Init(0.001, 0.1, 100, &adcx_kalman); // 初始化卡尔曼滤波器

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    short temperate_result;
    float temperate;

    uint32_t JS_RTT_Channel = 1;

    SEGGER_RTT_ConfigUpBuffer(JS_RTT_Channel, "JScope_U4U4", (void *)JS_RTT_UpBuffer, sizeof(JS_RTT_UpBuffer), SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL); // 配置RTT输出

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcx, ADC_Sec);
    xSemaphoreTake(xSemaphore_ADC, 10);        // 等待信号量
    adcx_kalman.out = adcx[1][0] = adcx[1][0]; // 初始值
    while (1)
    {
        // 处理数据
        adcx[1][0] = KalmanFilter(&adcx_kalman, adcx[0][0]);
        LTDC_Show_Num(400, 0, adcx[0][0], 4, 12, 0, GUI_Black);
        LTDC_Show_Num(400, 12, adcx[1][0], 4, 12, 0, GUI_Black);

        temperate = (float)adcx[1][0] * (2.5 / 4096); // 电压值
        temperate = (temperate - 0.76) / 0.0025 + 25; // 转换为温度值
        temperate_result = temperate *= 100;          // 扩大100倍.
        LTDC_Show_Num(400, 24, temperate_result, 4, 12, 0, GUI_Black);

        SEGGER_RTT_Write(JS_RTT_Channel, (void *)adcx, sizeof(adcx));

        HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcx, ADC_Sec);
        vTaskDelayUntil(&xLastWakeTime, 100);
        xSemaphoreTake(xSemaphore_ADC, 10); // 等待信号量,超时时间为500ms
    }
}
