#include "adc_task.h"
#include "tasks_sync.h"

// volatile float JS_RTT_UpBuffer[2048] = {0};

TaskHandle_t ADCTask_Handler; // 任务句柄

void vADCTask(void *pvParameters)
{
    (void)pvParameters; // 明确标记未使用参数

    Kalman adcx_kalman;
    Kalman_Init(0.001, 0.1, 1000, &adcx_kalman); // 初始化卡尔曼滤波器

    // int temperate_result;
    float temperate = 0;
    float adc_temp[2] = {0};

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    // uint32_t JS_RTT_Channel = 1;
    // SEGGER_RTT_ConfigUpBuffer(JS_RTT_Channel, "JScope_f4f4", (void *)JS_RTT_UpBuffer, sizeof(JS_RTT_UpBuffer), SEGGER_RTT_MODE_NO_BLOCK_SKIP); // 配置RTT输出

    // 第一次传输,处理数据
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcx, ADC_Sec);
    xSemaphoreTake(xSemaphore_ADC, 10); // 等待信号量
    adc_temp[0] = 0;
    for (int i = 0; i < ADC_Sec; i++)
    {
        adc_temp[0] += adcx[i][0];
    }
    adcx_kalman.out = adc_temp[1] = adc_temp[0] /= ADC_Sec; // 初始值

    while (1)
    {
        // 电压转换
        temperate = adc_temp[1] * (2.5 / 4096.0);     // 电压值
        temperate = (temperate - 0.76) / 0.0025 + 25; // 转换为温度值
        // temperate_result = temperate *= 100;          // 扩大100倍.

        // // 传输数据到RTT
        // SEGGER_RTT_Write(JS_RTT_Channel, &adc_temp[0], sizeof(adc_temp));
        // 传输数据到GUI任务
        xQueueSend(xQueue_ADC, &temperate, 10);

        vTaskDelayUntil(&xLastWakeTime, 100);

        // 循环传输数据
        HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcx, ADC_Sec);
        xSemaphoreTake(xSemaphore_ADC, 10); // 等待信号量,超时时间为500ms
        adc_temp[0] = 0;
        for (int i = 0; i < ADC_Sec; i++) // 处理数据
        {
            adc_temp[0] += adcx[i][0];
        }
        adc_temp[0] /= ADC_Sec;
        adc_temp[1] = KalmanFilter(&adcx_kalman, adc_temp[0]);
    }
}
