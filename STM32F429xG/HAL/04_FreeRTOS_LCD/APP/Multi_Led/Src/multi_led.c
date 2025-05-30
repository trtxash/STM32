#include "multi_led.h"

// led handle list head.
static struct Led_t *head_handle = NULL;

/**
 * @brief  Initializes the led struct handle.
 * @param  handle: the led handle struct.
 * @param  pin_level: set the HAL GPIO of the connected led level.
 * @param  active_level: pressed GPIO level.
 * @param  led_id: the led id.
 * @retval None
 */
void led_init(struct Led_t *handle, uint8_t (*pin_level)(uint8_t, uint8_t), uint8_t active_level, uint8_t led_id, LedMode mode)
{
    memset(handle, 0, sizeof(struct Led_t)); // handle的前sizeof(struct Led_t)字节设为0
    handle->mode = handle->mode_old = mode;
    handle->cycle = handle->dutycycle = 0; // 初始周期为0 ticks
    handle->hal_led_Level = pin_level;     // 抽象层GPIO写入函数指针
    handle->active_level = active_level;   // led GPIO有效电平
    handle->led_level = !active_level;     // led GPIO初始状态
    handle->led_id = led_id;               // led GPIO编号
}

void led_set(struct Led_t *handle, LedMode mode, uint32_t cycle, uint32_t dutycycle, uint8_t level)
{
    handle->mode = mode;
    handle->cycle = cycle;
    handle->dutycycle = dutycycle;
    handle->led_level = level;
}

/**
 * @brief  Start the led work, add the handle into work list.
 * @param  handle: target handle struct.
 * @retval 0: succeed. -1: already exist.
 */
int led_start(struct Led_t *handle)
{
    struct Led_t *target = head_handle;
    while (target)
    {
        if (target == handle)
            return -1; // already exist.
        target = target->next;
    }
    handle->next = head_handle; // 这意味着当前led将成为链表的新头节点，而之前的头节点将成为它的下一个节点。
    head_handle = handle;
    return 0;
}

/**
 * @brief  Stop the led work, remove the handle off work list.
 * @param  handle: target handle struct.
 * @retval None
 */
void led_stop(struct Led_t *handle)
{
    struct Led_t **curr;
    for (curr = &head_handle; *curr;)
    {
        struct Led_t *entry = *curr;
        if (entry == handle)
        {
            *curr = entry->next; // 砍头
            //			free(entry);
            return; // glacier add 2021-8-18
        }
        else
        {
            curr = &entry->next;
        }
    }
}

/**
 * @brief  led driver core function, driver state machine.
 * @param  handle: the led handle struct.
 * @retval None
 */
static void led_handler(struct Led_t *handle)
{
    // ticks counter working..
    if ((handle->state) > 0)
        handle->ticks++;

    // led mode change check..
    if (handle->mode != handle->mode_old)
    {
        handle->mode_old = handle->mode;
        handle->state = 0;
    }

    // led mode working..
    switch (handle->mode)
    {
    case SOLID: // 不用处理,直接输出
    {
    }
    break;
    case BLINK:
    {
        /*-----------------State machine-------------------*/
        if (handle->dutycycle > handle->cycle)
            handle->dutycycle = handle->cycle;

        switch (handle->state)
        {
        case 0:
            handle->led_level = handle->active_level; // led on
            handle->state = 1;
            break;
        case 1:
            if (handle->ticks >= handle->dutycycle)
            {
                handle->led_level = !handle->active_level; // led off
                handle->state = 2;
            }
            break;
        case 2:
            if (handle->ticks >= handle->cycle)
            {
                handle->led_level = handle->active_level; // led on
                handle->state = 1;
                handle->ticks = 0;
            }
            break;
        default:
            handle->state = 0; // reset
            break;
        }
    }
    break;
    case BREATHE: // 暂未实现
    {
        /*-----------------State machine-------------------*/
        switch (handle->state)
        {
        // case 0:
        //     break;
        default:
            handle->state = 0; // reset
            break;
        }
    }
    break;
    }

    handle->hal_led_Level(handle->led_id, handle->led_level); // 调用抽象层GPIO写入函数
}

/**
 * @brief  background ticks, timer repeat invoking interval 5ms.
 * @param  None.
 * @retval None
 */
void led_ticks(void)
{
    struct Led_t *target;
    for (target = head_handle; target; target = target->next)
    {
        led_handler(target);
    }
}
