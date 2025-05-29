#ifndef _LED_TASK_H
#define _LED_TASK_H

#include "tasks_common.h"

#define LEDQueueLen 16

#define TASK_PRIO_LED TASK_PRIO_NORMAL // 任务优先级,越大越高优先级
#define LED_TSTK_SIZE STACK_SIZE_BASE  // 任务堆栈大小,实际为32word=32*4byte=128byte=128*8bit

// 定义控制数据结构
typedef enum
{
    SOLID,  // 常亮
    BLINK,  // 闪烁
    BREATHE // 呼吸
} LedMode;

typedef struct
{
    LedMode mode;
    uint16_t on_time; // 单位ms
    uint16_t off_time;
    uint8_t intensity; // 亮度0-255
} LedCmd_t;

extern TaskHandle_t LEDTask_Handler; // 任务句柄

void vLedTask(void *pvParameters); // 任务函数

#endif /* _LED_TASK_H */
