#ifndef _TASKS_COMMON_H
#define _TASKS_COMMON_H

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#include "SEGGER_RTT.h"

// 任务优先级宏
#define TASK_PRIO_LOW    (tskIDLE_PRIORITY + 1)
#define TASK_PRIO_NORMAL (tskIDLE_PRIORITY + 2)
#define TASK_PRIO_HIGH   (tskIDLE_PRIORITY + 3) // #define TASK_PRIO_EMERGENCY (configMAX_PRIORITIES - 1)

// 任务堆栈大小宏
#define STACK_SIZE_BASE   (configMINIMAL_STACK_SIZE)
#define STACK_SIZE_LEVEL1 ((STACK_SIZE_BASE - 2) * 2 + 2)
#define STACK_SIZE_LEVEL2 ((STACK_SIZE_BASE - 2) * 4 + 2)
#define STACK_SIZE_LEVEL3 ((STACK_SIZE_BASE - 2) * 8 + 2)
#define STACK_SIZE_LEVEL4 ((STACK_SIZE_BASE - 2) * 16 + 2)
#define STACK_SIZE_LEVEL5 ((STACK_SIZE_BASE - 2) * 32 + 2)
#define STACK_SIZE_LEVEL6 ((STACK_SIZE_BASE - 2) * 64 + 2)

#endif /* _TASKS_COMMON_H */
