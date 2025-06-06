#include "gui_task.h"
#include "cpu_task.h"
#include "key_task.h"
#include "tasks_sync.h"
#include "touch_task.h"

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
        }

        float adc_temp;
        if (xQueueReceive(xQueue_ADC_Temp, &adc_temp, 10) == pdPASS)
        {
            LTDC_Show_float(799 - 6 * 6, 0, adc_temp, 3, 2, 12, 0, GUI_Black);
        }
        float adc_bat;
        if (xQueueReceive(xQueue_ADC_Bat, &adc_bat, 10) == pdPASS)
        {
            LTDC_Show_float(799 - 6 * 6, 12, adc_bat, 3, 2, 12, 0, GUI_Black);
        }
        float adc_bat_value;
        if (xQueueReceive(xQueue_ADC_BatVolt, &adc_bat_value, 10) == pdPASS)
        {
            LTDC_Show_float(799 - 6 * 6, 24, adc_bat_value, 3, 2, 12, 0, GUI_Black);
        }

        char cpu_temp[CPU_RUNINFO_SIZE];
        if (xQueueReceive(xQueue_CPU, &cpu_temp, 10) == pdPASS)
        {
            LTDC_Show_String_sprintf(400, 0, 400, 240, 12, (u8 *)CPU_RunInfo, 0, GUI_Black);
        }

        char touch_temp[Touch_Data_Len];
        if (xQueueReceive(xQueue_Touch, &touch_temp, 10) == pdPASS)
        {
            uint16_t xp = 0;
            uint16_t yp = 0;
            uint16_t tp = 0;
            for (int i = 0; i < 3; i++)
            {
                LTDC_Show_Num(400 + xp, 240, touch_temp[i], 3, 12, 0, GUI_Red);
                xp += 6 * 3;
                LTDC_Show_Char(400 + xp, 240, ',', 12, 0, GUI_Black);
                xp += 6;
            }
            xp = yp = tp = 0;
            for (int j = 0; j < 10; j++)
            {
                for (int i = 0; i < 6; i++)
                {
                    LTDC_Show_Num(400 + xp, 240 + 12 + yp, touch_temp[3 + tp], 3, 12, 0, GUI_Black);
                    tp++;
                    xp += 6 * 3;
                    LTDC_Show_Char(400 + xp, 240 + 12 + yp, ',', 12, 0, GUI_Black);
                    xp += 6;
                }
                xp = 0;
                yp += 12;
            }
        }

        vTaskDelayUntil(&xLastWakeTime, 15); // 大致66.6Hz
    }
}
