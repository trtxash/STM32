#include "bsp_freertos.h"
#include "key.h"
#include "lcd.h"
#include "led.h"
#include "log_rtt.h"
#include "multi_button.h"
#include "queue.h"
#include "sdram.h"

#define KeyQueueLen 16
QueueHandle_t xQueue_KEY = NULL;

#define START_TASK_PRIO 1       // 任务优先级,越大越高优先级
#define START_STK_SIZE  128 + 1 // 任务堆栈大小
TaskHandle_t StartTask_Handler; // 任务句柄
// void start_task(void *pvParameters); // 任务函数
void start_task(void *pvParameters); // 明确标记未使用参数; // 任务函数

#define LED_TASK_PRIO 14           // 任务优先级,越大越高优先级
#define LED_TSTK_SIZE 32 + 1       // 任务堆栈大小,实际为32word=32*4byte=128byte=128*8bit
TaskHandle_t LEDTask_Handler;      // 任务句柄
void led_task(void *pvParameters); // 任务函数

#define KEY_TASK_PRIO 15           // 任务优先级,越大越高优先级
#define KEY_TSTK_SIZE 256 + 1      // 任务堆栈大小,实际为32word=32*4byte=128byte=128*8bit
TaskHandle_t KEYTask_Handler;      // 任务句柄
void key_task(void *pvParameters); // 任务函数

#define GUI_TASK_PRIO 13           // 任务优先级,越大越高优先级
#define GUI_TSTK_SIZE 256 + 1      // 任务堆栈大小
TaskHandle_t GUI_Task_Handler;     // 任务句柄
void gui_task(void *pvParameters); // 任务函数

#define TEST_TASK_PRIO 11           // 任务优先级,越大越高优先级
#define TEST_TSTK_SIZE 256 + 1      // 任务堆栈大小
TaskHandle_t TESTTask_Handler;      // 任务句柄
void test_task(void *pvParameters); // 任务函数

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

    // 创建test任务
    xTaskCreate((TaskFunction_t)test_task,
                (const char *)"test_task",
                (uint16_t)TEST_TSTK_SIZE,
                (void *)NULL,
                (UBaseType_t)TEST_TASK_PRIO,
                (TaskHandle_t *)&TESTTask_Handler);

    vTaskDelete(StartTask_Handler); // 删除开始任务

    taskEXIT_CRITICAL(); // 退出临界区
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
        PressEvent event = DOUBLE_CLICK;
        xQueueSend(xQueue_KEY, &event, 10);
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
    // LTDC_Draw_Line(800 - 1, 0, 0, 480 - 1, GUI_Black);
    // LTDC_Draw_Circle((800 - 1) / 2, (480 - 1) / 2, 100, GUI_Green);
    // LTDC_Show_Char(0, (480 - 1) / 2, 'O', 12, 1, GUI_Black);
    LTDC_Show_Char(0, 480 - 1 - 12, 'O', 12, 1, GUI_Red);
    // LTDC_Fill(6, 480 - 1 - 12, 240, 480 - 1 - 1, GUI_Yellow); // y差11,x差5
    // for (int i = 0; i < 20; i++)
    // {
    //     if (i < 10)
    //         LTDC_Fill(6 + i * 6, 480 - 1 - 12, 240 + i * 12, 480 - 1, GUI_Black); // y差11,x差5
    //     else
    //         LTDC_Fill(6 + i * 6, 480 - 1 - 1, 240 + i * 12, 480 - 1 - 1, GUI_Yellow); // y差11,x差5
    // }
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
                // LTDC_Show_Num(20 * 6, lineflag, lineflag, 3, 12, 1, GUI_Black);
                LTDC_Show_Num(20 * 6, lineflag, lineflag, 3, 12, 1, GUI_Black);
                lineflag += 12;
            }
        }
        vTaskDelayUntil(&xLastWakeTime, 10);
    }
}

// 测试任务函数
void test_task(void *pvParameters)
{
    (void)pvParameters; // 明确标记未使用参数

    static uint8_t flag = 0;
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        flag = !flag;
        if (flag)
        {
            LED1_Set();
        }
        else
        {
            LED1_Clr();
        }
        vTaskDelayUntil(&xLastWakeTime, 1000);
    }
}
