#include "key_task.h"
#include "key.h"

QueueHandle_t xQueue_KEY = NULL;
TaskHandle_t KEYTask_Handler; // 任务句柄

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

void vKeyTask(void *pvParameters)
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
