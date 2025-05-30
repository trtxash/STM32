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
void led_init(struct Led_t *handle, uint8_t (*pin_level)(uint8_t, uint8_t), uint8_t active_level, uint8_t led_id)
{
    memset(handle, 0, sizeof(struct Led_t)); // handle的前sizeof(struct Led_t)字节设为0
    handle->cycle = 0;                       // 初始周期为0 ticks
    handle->hal_led_Level = pin_level;       // 抽象层GPIO写入函数指针
    handle->active_level = active_level;     // led GPIO有效电平
    handle->led_level = !active_level;       // led GPIO初始状态
    handle->led_id = led_id;                 // led GPIO编号
}

/**
 * @brief  led driver core function, driver state machine.
 * @param  handle: the led handle struct.
 * @retval None
 */
static void led_handler(struct Led_t *handle)
{
    handle->hal_led_Level(handle->led_id, handle->led_level); // 调用抽象层GPIO写入函数

    // uint8_t read_gpio_level = handle->hal_led_Level(handle->led_id);

    // // ticks counter working..
    // if ((handle->state) > 0)
    //     handle->ticks++;

    // /*------------led debounce handle---------------*/
    // if (read_gpio_level != handle->led_level)
    // { // not equal to prev one
    //     // continue read 3 times same new level change
    //     if (++(handle->debounce_cnt) >= DEBOUNCE_TICKS)
    //     {
    //         handle->led_level = read_gpio_level;
    //         handle->debounce_cnt = 0;
    //     }
    // }
    // else
    // { // level not change ,counter reset.
    //     handle->debounce_cnt = 0;
    // }

    // /*-----------------State machine-------------------*/
    // switch (handle->state)
    // {
    // case 0:
    //     if (handle->led_level == handle->active_level)
    //     { // start press down
    //         handle->event = (uint8_t)PRESS_DOWN;
    //         EVENT_CB(PRESS_DOWN);
    //         handle->ticks = 0;
    //         handle->repeat = 1;
    //         handle->state = 1;
    //     }
    //     else
    //     {
    //         handle->event = (uint8_t)NONE_PRESS;
    //     }
    //     break;

    // case 1:
    //     if (handle->led_level != handle->active_level)
    //     { // released press up
    //         handle->event = (uint8_t)PRESS_UP;
    //         EVENT_CB(PRESS_UP);
    //         handle->ticks = 0;
    //         handle->state = 2;
    //     }
    //     else if (handle->ticks > LONG_TICKS)
    //     {
    //         handle->event = (uint8_t)LONG_PRESS_START;
    //         EVENT_CB(LONG_PRESS_START);
    //         handle->state = 5;
    //     }
    //     break;

    // case 2:
    //     if (handle->led_level == handle->active_level)
    //     { // press down again
    //         handle->event = (uint8_t)PRESS_DOWN;
    //         EVENT_CB(PRESS_DOWN);
    //         if (handle->repeat != PRESS_REPEAT_MAX_NUM)
    //         {
    //             handle->repeat++;
    //         }
    //         EVENT_CB(PRESS_REPEAT); // repeat hit
    //         handle->ticks = 0;
    //         handle->state = 3;
    //     }
    //     else if (handle->ticks > SHORT_TICKS)
    //     { // released timeout
    //         if (handle->repeat == 1)
    //         {
    //             handle->event = (uint8_t)SINGLE_CLICK;
    //             EVENT_CB(SINGLE_CLICK);
    //         }
    //         else if (handle->repeat == 2)
    //         {
    //             handle->event = (uint8_t)DOUBLE_CLICK;
    //             EVENT_CB(DOUBLE_CLICK); // repeat hit
    //         }
    //         handle->state = 0;
    //     }
    //     break;

    // case 3:
    //     if (handle->led_level != handle->active_level)
    //     { // released press up
    //         handle->event = (uint8_t)PRESS_UP;
    //         EVENT_CB(PRESS_UP);
    //         if (handle->ticks < SHORT_TICKS)
    //         {
    //             handle->ticks = 0;
    //             handle->state = 2; // repeat press
    //         }
    //         else
    //         {
    //             handle->state = 0;
    //         }
    //     }
    //     else if (handle->ticks > SHORT_TICKS)
    //     { // SHORT_TICKS < press down hold time < LONG_TICKS
    //         handle->state = 1;
    //     }
    //     break;

    // case 5:
    //     if (handle->led_level == handle->active_level)
    //     {
    //         // continue hold trigger
    //         handle->event = (uint8_t)LONG_PRESS_HOLD;
    //         EVENT_CB(LONG_PRESS_HOLD);
    //     }
    //     else
    //     { // released
    //         handle->event = (uint8_t)PRESS_UP;
    //         EVENT_CB(PRESS_UP);
    //         handle->state = 0; // reset
    //     }
    //     break;
    // default:
    //     handle->state = 0; // reset
    //     break;
    // }
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
