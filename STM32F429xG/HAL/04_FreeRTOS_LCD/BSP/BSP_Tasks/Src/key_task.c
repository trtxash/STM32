#include "key_task.h"
#include "tasks_sync.h"

TaskHandle_t KEYTask_Handler; // 任务句柄

Button button_up;

static uint8_t read_button_GPIO(uint8_t button_id)
{
    // you can share the GPIO read function with multiple Buttons
    switch (button_id)
    {
    case KEY_ID_UP:
        return HAL_GPIO_ReadPin(KEY_UP_Port, KEY_UP_PIN);
        break;
    default:
        return 0;
        break;
    }
}

static void Callback_Down_Click_Handler(void *btn)
{
    if (btn == &button_up)
    {
        PressEvent event = PRESS_DOWN;
        xQueueSend(xQueue_KEY, &event, 10);
    }
}

static void Callback_Up_Click_Handler(void *btn)
{
    if (btn == &button_up)
    {
        PressEvent event = PRESS_UP;
        xQueueSend(xQueue_KEY, &event, 10);
    }
}

static void Callback_Repeat_Click_Handler(void *btn)
{
    if (btn == &button_up)
    {
        PressEvent event = PRESS_REPEAT;
        xQueueSend(xQueue_KEY, &event, 10);
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

static void Callback_Long_Press_Start_Handler(void *btn)
{
    if (btn == &button_up)
    {
        PressEvent event = LONG_PRESS_START;
        xQueueSend(xQueue_KEY, &event, 10);
    }
}

static void Callback_Long_Press_Hold_Handler(void *btn)
{
    if (btn == &button_up)
    {
        PressEvent event = LONG_PRESS_HOLD;
        xQueueSend(xQueue_KEY, &event, 10);
    }
}

void vKeyTask(void *pvParameters)
{
    (void)pvParameters; // 明确标记未使用参数

    button_init(&button_up, read_button_GPIO, 1, KEY_ID_UP);

    button_attach(&button_up, PRESS_DOWN, Callback_Down_Click_Handler);
    button_attach(&button_up, PRESS_UP, Callback_Up_Click_Handler);
    button_attach(&button_up, PRESS_REPEAT, Callback_Repeat_Click_Handler);
    button_attach(&button_up, SINGLE_CLICK, Callback_Single_Click_Handler);
    button_attach(&button_up, DOUBLE_CLICK, Callback_Double_Click_Handler);
    button_attach(&button_up, LONG_PRESS_START, Callback_Long_Press_Start_Handler);
    button_attach(&button_up, LONG_PRESS_HOLD, Callback_Long_Press_Hold_Handler);

    button_start(&button_up); // 将button_up加入链表

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        button_ticks();
        vTaskDelayUntil(&xLastWakeTime, 50);
    }
}
