#include "gui_task.h"
#include "cpu_task.h"
#include "key_task.h"
#include "tasks_sync.h"
#include "touch_task.h"

#include "lv_demos.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lvgl.h"

TaskHandle_t GUITask_Handler; // 任务句柄
_m_tp_dev GUITask_TouchData = {0};

void vGUITask(void *pvParameters)
{
    (void)pvParameters; // 明确标记未使用参数

    tp_dev.touchtype = 1; // 触摸屏 设为横屏

    // lv_demo_widgets();
    // lv_demo_scroll();
    // lv_demo_benchmark();
    // lv_demo_music();

    // lv_obj_t *switch_obj1 = lv_switch_create(lv_screen_active());
    // lv_obj_set_size(switch_obj1, 120, 60);
    // lv_obj_align(switch_obj1, LV_ALIGN_CENTER, 0, 0);

    // lv_obj_t *switch_obj2 = lv_switch_create(lv_screen_active());
    // lv_obj_set_size(switch_obj2, 120, 60);
    // lv_obj_align(switch_obj2, LV_ALIGN_TOP_MID, 0, 0);

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
#if 0
        static int lineflag = 0;

        Button key_temp;
        if (xQueueReceive(xQueue_KEY, &key_temp, 0) == pdPASS)
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
        if (xQueueReceive(xQueue_ADC_Temp, &adc_temp, 0) == pdPASS)
        {
            LTDC_Show_float(799 - 6 * 6, 0, adc_temp, 3, 2, 12, 0, GUI_Black);
        }
        float adc_bat;
        if (xQueueReceive(xQueue_ADC_Bat, &adc_bat, 0) == pdPASS)
        {
            LTDC_Show_float(799 - 6 * 6, 12, adc_bat, 3, 2, 12, 0, GUI_Black);
        }
        float adc_bat_value;
        if (xQueueReceive(xQueue_ADC_BatVolt, &adc_bat_value, 0) == pdPASS)
        {
            LTDC_Show_float(799 - 6 * 6, 24, adc_bat_value, 3, 2, 12, 0, GUI_Black);
        }

        char cpu_temp[CPU_RUNINFO_SIZE];
        if (xQueueReceive(xQueue_CPU, &cpu_temp, 0) == pdPASS)
        {
            LTDC_Show_String_sprintf(400, 0, 400, 240, 12, (u8 *)CPU_RunInfo, 0, GUI_Black);
        }

        _m_tp_dev touch_temp;
        if (xQueueReceive(xQueue_Touch, &touch_temp, 0) == pdPASS)
        {
            uint16_t xp = 0;
            uint16_t yp = 0;
            LTDC_Show_Num(400, 240, touch_temp.sta, 3, 12, 0, GUI_Red);
            LTDC_Show_Num(400 + 6 + 6 * 3, 240, touch_temp.touchtype, 3, 12, 0, GUI_Red);
            for (int j = 0; j < (CT_MAX_TOUCH / 5); j++)
            {
                for (int i = 0; i < 5; i++)
                {
                    LTDC_Show_Num(400 + xp, 240 + 12 + yp, touch_temp.x[i + j * 5], 3, 12, 0, GUI_Black);
                    xp += 6 * 3;
                    LTDC_Show_Char(400 + xp, 240 + 12 + yp, ',', 12, 0, GUI_Black);
                    xp += 6 * 1;
                    LTDC_Show_Num(400 + xp, 240 + 12 + yp, touch_temp.y[i + j * 5], 3, 12, 0, GUI_Black);
                    xp += 6 * 3;
                    LTDC_Show_Char(400 + xp, 240 + 12 + yp, ' ', 12, 0, GUI_Black);
                    xp += 6 * 1;
                }
                xp = 0;
                yp += 12;
            }
        }
        vTaskDelayUntil(&xLastWakeTime, GUI_TaskCycleTime_ms); // 大致125Hz
#else
        if (xQueueReceive(xQueue_Touch, &GUITask_TouchData, 0) == pdPASS)
        {
        }
        lv_timer_handler();
        vTaskDelayUntil(&xLastWakeTime, 5);
#endif
    }
    vTaskDelete(NULL);
}
