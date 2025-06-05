#include "adc_task.h"
#include "tasks_sync.h"

// volatile float JS_RTT_UpBuffer[2048] = {0};

TaskHandle_t ADCTask_Handler; // 任务句柄;

static float battery_voltage_to_soc(float voltage)
{
    // 电压安全校验
    if (voltage > 4.2f)
        return 100.0f; // 过压保护
    if (voltage < 3.0f)
        return 0.0f; // 欠压保护

    // 分段线性插值计算
    if (voltage >= 4.15f)
    {
        return 100.0f;
    }
    else if (voltage >= 4.0f)
    {
        return 90.0f + 10.0f * (voltage - 4.0f) / 0.15f;
    }
    else if (voltage >= 3.9f)
    {
        return 80.0f + 10.0f * (voltage - 3.9f) / 0.1f;
    }
    else if (voltage >= 3.8f)
    {
        return 60.0f + 20.0f * (voltage - 3.8f) / 0.1f;
    }
    else if (voltage >= 3.7f)
    {
        return 40.0f + 20.0f * (voltage - 3.7f) / 0.1f;
    }
    else if (voltage >= 3.6f)
    {
        return 20.0f + 20.0f * (voltage - 3.6f) / 0.1f;
    }
    else if (voltage >= 3.3f)
    {
        return 5.0f + 15.0f * (voltage - 3.3f) / 0.3f;
    }
    else
    {
        return 0.0f; // 低于3.3V按0%处理
    }
}

void vADCTask(void *pvParameters)
{
    (void)pvParameters; // 明确标记未使用参数

    Kalman adc1_kalman[ADC_Ch + 1];
    float adc_temp[ADC_Ch] = {0};
    float temperate = 0, V_Bat = 0, V_Bat_Volt = 0;

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    // uint32_t JS_RTT_Channel = 1;
    // SEGGER_RTT_ConfigUpBuffer(JS_RTT_Channel, "JScope_f4f4", (void *)JS_RTT_UpBuffer, sizeof(JS_RTT_UpBuffer), SEGGER_RTT_MODE_NO_BLOCK_SKIP); // 配置RTT输出

    Kalman_Init(0.001, 0.1, 100, &adc1_kalman[0]);     // 初始化卡尔曼滤波器
    Kalman_Init(0.001, 0.1, 10, &adc1_kalman[1]);     // 初始化卡尔曼滤波器
    Kalman_Init(0.001, 0.001, 1000, &adc1_kalman[2]); // 初始化卡尔曼滤波器
    // 第一次传输,处理数据
    for (u8 j = 0; j < ADC_Ch; j++)
    {
        switch (j)
        {
        case 0:
            ADC_Change_Channel_SingleScan_DMA_NOMAL(&hadc1, 18, 480);
            break;
        case 1:
            ADC_Change_Channel_SingleScan_DMA_NOMAL(&hadc1, 15, 480);
            break;
        default:
            break;
        }
        HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcx + j * ADC_Sec, ADC_Sec);
        vTaskDelayUntil(&xLastWakeTime, 100);
        xSemaphoreTake(xSemaphore_ADC, 10); // 等待信号量

        for (u32 i = 0; i < ADC_Sec; i++)
        {
            adc_temp[j] += adcx[j][i];
        }
        adc1_kalman[j].out = adc_temp[j] /= ADC_Sec; // 初始值
    }
    // 电压转换
    temperate = adc_temp[0] * (2.5 / 4096.0);           // 电压值
    temperate = (temperate - 0.76) / 0.0025 + 25;       // 转换为温度值
    V_Bat = adc_temp[1] * (2.5 / 4096.0) * 2 * 1.0064;  // 电压值
    adc1_kalman[2].out = battery_voltage_to_soc(V_Bat); // 初始值

    while (1)
    {
        // // 传输数据到RTT
        // SEGGER_RTT_Write(JS_RTT_Channel, &adc_temp[0], sizeof(adc_temp));

        // 循环传输数据
        for (u8 j = 0; j < ADC_Ch; j++)
        {
            switch (j)
            {
            case 0:
                ADC_Change_Channel_SingleScan_DMA_NOMAL(&hadc1, 18, 480);
                break;
            case 1:
                ADC_Change_Channel_SingleScan_DMA_NOMAL(&hadc1, 15, 480);
                break;
            default:
                break;
            }
            HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcx + j * ADC_Sec, ADC_Sec);
            vTaskDelayUntil(&xLastWakeTime, 100);
            xSemaphoreTake(xSemaphore_ADC, 10); // 等待信号量
            adc_temp[j] = 0;
            for (u32 i = 0; i < ADC_Sec; i++) // 均值处理
            {
                adc_temp[j] += adcx[j][i];
            }
            adc_temp[j] = KalmanFilter(&adc1_kalman[j], adc_temp[j] / ADC_Sec); // 卡尔曼滤波
        }

        // 电压转换
        temperate = adc_temp[0] * (2.5 / 4096.0);          // 电压值
        temperate = (temperate - 0.76) / 0.0025 + 25;      // 转换为温度值
        V_Bat = adc_temp[1] * (2.5 / 4096.0) * 2 * 1.0064; // 电压值

        V_Bat_Volt = KalmanFilter(&adc1_kalman[2], battery_voltage_to_soc(V_Bat)); // 电量卡尔曼滤波
        // 传输数据到GUI任务
        xQueueSend(xQueue_ADC_Temp, &temperate, 10);
        xQueueSend(xQueue_ADC_Bat, &V_Bat, 10);
        xQueueSend(xQueue_ADC_BatVolt, &V_Bat_Volt, 10);
    }
}
