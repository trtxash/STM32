/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include "dma2d.h"
#include "lcd.h"
#include "ltdc.h"
#include <string.h>

#include "tasks_common.h"
#include "tasks_sync.h"

/*********************
 *      DEFINES
 *********************/
#define MY_DISP_HOR_RES 800
#define MY_DISP_VER_RES 480

#ifndef MY_DISP_HOR_RES
#warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen width, default value 320 is used for now.
#define MY_DISP_HOR_RES 800
#endif

#ifndef MY_DISP_VER_RES
#warning Please define or replace the macro MY_DISP_VER_RES with the actual screen height, default value 240 is used for now.
#define MY_DISP_VER_RES 480
#endif

#define BYTE_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565)) /*will be 2 for RGB565 */

volatile uint8_t g_gpu_state = 0XFF;
volatile uint8_t g_gpu_state_temp = 0XFF;

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*------------------------------------
     * Create a display and set a flush_cb
     * -----------------------------------*/
    lv_display_t *disp = lv_display_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);
    lv_display_set_flush_cb(disp, disp_flush);

#if 0
    /* Example 1
     * One buffer for partial rendering*/
    LV_ATTRIBUTE_MEM_ALIGN
    // static uint8_t buf_1_1[MY_DISP_HOR_RES * MY_DISP_VER_RES * BYTE_PER_PIXEL] __attribute__((section(".my_sdram_section")));
    // lv_display_set_buffers(disp, buf_1_1, NULL, sizeof(buf_1_1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    /* Example 2
     * Two buffers for partial rendering
     * In flush_cb DMA or similar hardware should be used to update the display in the background.*/
    LV_ATTRIBUTE_MEM_ALIGN
    static uint8_t buf_2_1[MY_DISP_HOR_RES * 10 * BYTE_PER_PIXEL];

    LV_ATTRIBUTE_MEM_ALIGN
    static uint8_t buf_2_2[MY_DISP_HOR_RES * 10 * BYTE_PER_PIXEL];
    lv_display_set_buffers(disp, buf_2_1, buf_2_2, sizeof(buf_2_1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    /* Example 3
     * Two buffers screen sized buffer for double buffering.
     * Both LV_DISPLAY_RENDER_MODE_DIRECT and LV_DISPLAY_RENDER_MODE_FULL works, see their comments*/
    LV_ATTRIBUTE_MEM_ALIGN
    static uint8_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES * BYTE_PER_PIXEL];

    LV_ATTRIBUTE_MEM_ALIGN
    static uint8_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES * BYTE_PER_PIXEL];
    lv_display_set_buffers(disp, buf_3_1, buf_3_2, sizeof(buf_3_1), LV_DISPLAY_RENDER_MODE_DIRECT);
#endif
    lv_display_set_buffers(disp, (uint8_t *)ltdc_framebuf[1], (uint8_t *)ltdc_framebuf[0], MY_DISP_HOR_RES * MY_DISP_VER_RES * BYTE_PER_PIXEL, LV_DISPLAY_RENDER_MODE_FULL);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    /*You code here*/
    // LCD_Init();
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

/*Flush the content of the internal buffer the specific area on the display.
 *`px_map` contains the rendered image as raw pixel map and it should be copied to `area` on the display.
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_display_flush_ready()' has to be called when it's finished.*/
static void disp_flush(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *px_map)
{
    if (disp_flush_enabled)
    {
        // while (!(LTDC->CDSR & LTDC_CDSR_VSYNCS)) // 垂直同步信号通常用于指示显示器已经准备好接收新的一帧数据。
        //     ;
        // HAL_LTDC_SetAddress(&hltdc, (uint32_t)lv_display_get_buf_active(lv_display_get_default())->data, 0);

#if 0
        /* 按生产者强制即时更新 */
        // 40fps,短板在SDRAM带宽和LTDC频率
        // 这里只有可能LTDC慢，导致没读完当前buff就到了另一个buff，产生闪屏
        HAL_LTDC_SetAddress(&hltdc1, (uint32_t)lv_display_get_buf_active(lv_display_get_default())->data, 0);
        __HAL_LTDC_RELOAD_IMMEDIATE_CONFIG(&hltdc1); // 强制即时更新
        lv_display_flush_ready(disp_drv);
#endif

#if 0
        /* 按消费者强制即时更新，大部分数据在中断处理 */
        // 短板在SDRAM带宽和LTDC频率
        // 这里只有可能LTDC慢，导致LVGL自动覆写了另一个buff，产生闪屏
        if ((uint32_t)lv_display_get_buf_active(lv_display_get_default())->data == (uint32_t)ltdc_framebuf[0]) // 检测画完了哪个buf
            g_gpu_state = 0;
        else
            g_gpu_state = 1;

        if (g_gpu_state == 0 && g_gpu_state_temp == 0) // 算完buf0，且LTDC写完buf0
        {
            // lv_display_flush_ready(lv_display_get_default()); // 去算buf1
        }
        else if (g_gpu_state == 1 && g_gpu_state_temp == 0) // 算完buf1，且LTDC写完buf0
        {
            // HAL_LTDC_SetAddress(&hltdc1, (uint32_t)ltdc_framebuf[g_gpu_state], 0); // 切换buf1
            // lv_display_flush_ready(lv_display_get_default()); // 去算buf0
            // g_gpu_state_temp = g_gpu_state;                   // 保存LTDC状态
        }
        else if (g_gpu_state == 0 && g_gpu_state_temp == 1) // 算完buf0，且LTDC写完buf1
        {
            // HAL_LTDC_SetAddress(&hltdc1, (uint32_t)ltdc_framebuf[g_gpu_state], 0); // 切换buf0
            // lv_display_flush_ready(lv_display_get_default()); // 去算buf1
            // g_gpu_state_temp = g_gpu_state;                   // 保存LTDC状态
        }
        else if (g_gpu_state == 1 && g_gpu_state_temp == 1) // 算完buf1，且LTDC写完buf1
        {
            // lv_display_flush_ready(lv_display_get_default()); // 去算buf0
        }
        else if (g_gpu_state == 0)
        {
        }
        else if (g_gpu_state == 1)
        {
        }
#endif

#if 1
        // /* 按消费者垂直同步更新 */
        // // 短板在SDRAM带宽和LTDC频率
        if (xSemaphoreTake(xSemaphore_VSync, portMAX_DELAY) == pdTRUE) // 等待垂直同步信号
        {
            HAL_LTDC_SetAddress(&hltdc1, (uint32_t)lv_display_get_buf_active(lv_display_get_default())->data, 0); // 切换buf
            while (!(LTDC->CDSR & LTDC_CDSR_VSYNCS))                                                              // 垂直同步信号通常用于指示显示器已经准备好接收新的一帧数据。
                ;
            lv_display_flush_ready(lv_display_get_default()); // 去算buf
        }

#endif
    }

    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    // lv_display_flush_ready(disp_drv);
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
