/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* Ensure stdint is only used by the compiler, and not the assembler. */
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
#include "stdio.h"
#include <stdint.h>

#include "sys.h"
#include "tim.h"

extern uint32_t SystemCoreClock;
extern volatile unsigned long long FreeRTOSRunTimeTicks;
#endif

#define configUSE_PREEMPTION 1                        // 为1时使用抢占式调度器，为0时使用协程，抢断式调度器的话内核会在每个时钟节拍中断中进行任务切换，
#define configUSE_IDLE_HOOK 0                         // 为1时使用空闲任务钩子函数，vApplicationIdleHook(void)
#define configUSE_TICK_HOOK 0                         // 为1使用时间片钩子函数，需要vApplicationTickHook(void)
#define configCPU_CLOCK_HZ (SystemCoreClock)          // cpu频率
#define configTICK_RATE_HZ ((TickType_t)1000)         // 设置FreeRTOS的系统时钟节拍频率Hz，1000为1ms
#define configMAX_PRIORITIES (32)                     // 设置任务优先级数量，0~num-1
#define configMINIMAL_STACK_SIZE ((unsigned short)66) // 设置空闲任务的最小任务堆栈大小，字为单位，(66-2)*4字节
#define configTOTAL_HEAP_SIZE ((size_t)(75 * 1024))   // 设置堆大小，然后使用了动态内存管理，FreeRTOS会使用heapX.c中的内存申请函数来申请内存，这些内存时从堆ucHeap[configTOTAL_HEAP_SIZE]中申请的，堆的大小由这项来定义
#define configMAX_TASK_NAME_LEN (10)                  // 设置任务名最大长度
#define configUSE_TRACE_FACILITY 1                    // 为1启用可视化跟踪调试，会增加一些结构体成员和API函数
#define configUSE_16_BIT_TICKS 0                      // 来设置事件标志的位数，0为32位，1为16位
#define configIDLE_SHOULD_YIELD 1                     // 定义了与空闲任务(idle Task)处于同等优先级的其他用户任务行为，为1可以让出cpu使用权，减少空闲任务时间，但有副作用
#define configUSE_MUTEXES 1                           // 为1使用互斥信号量，相关API函数会被编译
#define configQUEUE_REGISTRY_SIZE 8                   // 设置可以注册的队列和信号量的最大数量，使用内核调试器查看信号量和队列要设置，而且要先将消息队列和信号量进行注册
#define configCHECK_FOR_STACK_OVERFLOW 2              // 设置堆栈溢出检测，1或2两种模式，会调用vApplicationStackOverflowHook钩子函数
#define configUSE_RECURSIVE_MUTEXES 1                 // 为1时使用递归互斥信号量，相关API会被编译
#define configUSE_MALLOC_FAILED_HOOK 0                // 为1时使用内存分配失败钩子函数，vApplicationMallocFailedHook(void)
#define configUSE_APPLICATION_TASK_TAG 0              // 为1则configUSE_APPLICATION_TASK_TAGF()和xTaskCallApplicationTaskHook()会被编译
#define configUSE_COUNTING_SEMAPHORES 1               // 为1启动计数型信号量，相关的API函数会被编译

#define configGENERATE_RUN_TIME_STATS 1                                       // 为1开启时间统计功能，相应API函数会被编译，为1还要定义额外的宏，见开发手册
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() ConfigureTimerForTimeStats() // 定义定时器初始化函数以提供统计的“时基”
#define portGET_RUN_TIME_COUNTER_VALUE() FreeRTOSRunTimeTicks                 // 定义变量以读取统计定时器的值
#define configRECORD_STACK_HIGH_ADDRESS 1                                     // 检查堆栈？

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 0             // 为1启用协程，节省开销，但功能有限
#define configMAX_CO_ROUTINE_PRIORITIES (2) // 设置分配给协程的最大优先级，0~num-1

/* Software timer definitions. */
#define configUSE_TIMERS 1                                          // 为1使用软件定时器，相关定API会被编译
#define configTIMER_TASK_PRIORITY (2)                               // 软件定时器任务优先级
#define configTIMER_QUEUE_LENGTH 10                                 // 软件定时器API函数会通过命令队列向软件定时器任务发送消息，这用来设置这个软件定时器命令队列长度
#define configTIMER_TASK_STACK_DEPTH (configMINIMAL_STACK_SIZE * 2) // 软件定时器任务的任务堆栈大小

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet 1
#define INCLUDE_uxTaskPriorityGet 1
#define INCLUDE_vTaskDelete 1
#define INCLUDE_vTaskCleanUpResources 1
#define INCLUDE_vTaskSuspend 1
#define INCLUDE_vTaskDelayUntil 1
#define INCLUDE_vTaskDelay 1

/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
#define configPRIO_BITS __NVIC_PRIO_BITS
#else
#define configPRIO_BITS 4 /* 15 priority levels */
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY 0xf

/*
The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! higher priorities are lower numeric values.
*/
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 8

/* Interrupt priorities used by the kernel port layer itself.  These are generic to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!! See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

/* Normal assert() semantics without relying on the provision of an assert.h header file. */
#if DEBUG_FLAG == 1
#if LOG_TYPE == 0
#define vAssertCalled(char, int) printf("Error:%s,%d\r\n", char, int) // 打印出文件和行号

#elif LOG_TYPE == 1
#include "log_rtt.h"
#define vAssertCalled(char, int) LOGE("Error:%s,%d\r\n", char, int) // 打印出文件和行号

#endif
/* 断言，检测传入参数是否合理 */
#define configASSERT(x)                    \
    if ((x) == 0)                          \
    {                                      \
        vAssertCalled(__FILE__, __LINE__); \
        taskDISABLE_INTERRUPTS();          \
        while (1)                          \
            ;                              \
    }
#endif

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
// #define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */
