#ifndef __TASKS_SYNC_H__
#define __TASKS_SYNC_H__

#include "key_task.h"
#include "led_task.h"
#include "tasks_common.h"

#define KeyQueueLen 16
#define LEDQueueLen 16

extern QueueHandle_t xQueue_KEY;
extern QueueHandle_t xQueue_Led;

void vSyncResources_Init(void);

#endif /* __TASKS_SYNC_H__ */
