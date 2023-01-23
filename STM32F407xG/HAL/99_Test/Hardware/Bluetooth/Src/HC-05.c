#include "HC-05.h"

/**
 * @brief   HC-05初始化函数
 * @param   none
 * @arg		none
 * @note
 * @retval  u8，1成功，0失败
 */
u8 HC_05_init()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    /* GPIO时钟开启 */
    HC_05_STATE_Port_Clk_Enable();
    HC_05_EN_Port_Clk_Enable();

    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStructure.Pin = HC_05_STATE_Pin;
    HAL_GPIO_Init(HC_05_STATE_Port, &GPIO_InitStructure);

    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Pin = HC_05_EN_Pin;
    HAL_GPIO_Init(HC_05_EN_Port, &GPIO_InitStructure);

    HC_05_EN_Set();
    delay_ms(100);
    return HC_05_READ_STATE();
}