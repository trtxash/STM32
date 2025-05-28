#include "gui_task.h"
#include "key.h"
#include "key_task.h"
#include "lcd.h"

TaskHandle_t GUITask_Handler; // 任务句柄

void vGUITask(void *pvParameters)
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
