#include "gui_task.h"
#include "key_task.h"
#include "lcd.h"
#include "multi_button.h"
#include "tasks_sync.h"

TaskHandle_t GUITask_Handler; // 任务句柄

void vGUITask(void *pvParameters)
{
    (void)pvParameters; // 明确标记未使用参数

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        static int lineflag = 0;

        Button key_temp;
        if (xQueueReceive(xQueue_KEY, &key_temp, 10) == pdPASS)
        {
            // if (lineflag <= 468)
            // {
            switch (key_temp.event)
            {
            case PRESS_DOWN:
                LTDC_Show_String(0, lineflag % 480, 240, 240, 12, (u8 *)"Press Down", 0, GUI_Black);
                break;
            case PRESS_UP:
                LTDC_Show_String(0, lineflag % 480, 240, 240, 12, (u8 *)"Press Up", 0, GUI_Black);
                break;
            case PRESS_REPEAT:
                LTDC_Show_String(0, lineflag % 480, 240, 240, 12, (u8 *)"Press Repeat", 0, GUI_Black);
                break;
            case SINGLE_CLICK:
                LTDC_Show_String(0, lineflag % 480, 240, 240, 12, (u8 *)"Single Click", 0, GUI_Black);
                break;
            case DOUBLE_CLICK:
                LTDC_Show_String(0, lineflag % 480, 240, 240, 12, (u8 *)"Double Click", 0, GUI_Black);
                break;
            case LONG_PRESS_START:
                LTDC_Show_String(0, lineflag % 480, 240, 240, 12, (u8 *)"Long Press Start", 0, GUI_Black);
                break;
            case LONG_PRESS_HOLD:
                LTDC_Show_String(0, lineflag % 480, 240, 240, 12, (u8 *)"Long Press Hold", 0, GUI_Black);
                break;
            default:
                LTDC_Show_String(0, lineflag % 480, 240, 240, 12, (u8 *)"None Press", 0, GUI_Black);
                break;
            }
            LTDC_Show_String(16 * 6, (lineflag - 12) % 480, 240, 240, 12, (u8 *)",Line", 0, GUI_Black);
            LTDC_Show_Num(22 * 6, (lineflag - 12) % 480, lineflag, 9, 12, 0, GUI_Black);

            LTDC_Show_String(16 * 6, lineflag % 480, 240, 240, 12, (u8 *)",Line", 0, GUI_Red);
            LTDC_Show_Num(22 * 6, lineflag % 480, lineflag, 9, 12, 0, GUI_Red);
            lineflag += 12;
            // }
        }
        vTaskDelayUntil(&xLastWakeTime, 5);
    }
}
