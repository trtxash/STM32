#include "lcd.h"
#include "dma2d.h"
#include "ltdc.h"
#include "sys.h"

// #include "lv_port_disp.h"

u16 *ltdc_lcd_framebuf = (u16 *)LCD_FRAME_BUF_ADDR; // LTDC LCD帧缓存数组指针,必须指向对应大小的内存区域
// u16 ltdc_lcd_framebuf[ACTIVE_HEIGHT][ACTIVE_WIDTH] __attribute__((at(LCD_FRAME_BUF_ADDR))); // 定义最大屏分辨率时,LCD所需的帧缓存数组大小
u32 *ltdc_framebuf[2];
_ltdc_dev lcdltdc; // 管理LCD LTDC的重要参数

/**
 * @brief	LCD的blk背光IO初始化
 * @param 	无
 * @arg
 * @note  	额外的LCD初始化
 * @retval	无
 */
void LCD_BLK_Init(void)
{
    GPIO_InitTypeDef GPIO_InitTure = {0};

    LCD_BLK_GPIO_CLK_ENABLE(); // 开启GPIO时钟

    /*进行结构体内的参数配置，先找到下面HAL_GPIO_Init();的定义处，再对定义处的函数详细找参数*/
    GPIO_InitTure.Mode = GPIO_MODE_OUTPUT_PP; // 推挽输出
    GPIO_InitTure.Pull = GPIO_NOPULL;
    GPIO_InitTure.Speed = GPIO_SPEED_FREQ_LOW; // 高速
    GPIO_InitTure.Pin = LCD_BLK_PIN;
    HAL_GPIO_Init(LCD_BLK_GPIO, &GPIO_InitTure); // 先在上面四行设置GPIO的模式，上下拉，速度，再对GPIOB管脚初始化
}

/**
 * @brief	RGB LCD初始化
 * @param 	无
 * @arg
 * @note  	加载了LTDC，DMA2D
 * @retval	无
 */
void LCD_Init(void)
{
    LCD_BLK_Init();  // 背光IO初始化
    LTDC_Init();     // LTDC初始化
    MX_DMA2D_Init(); // DMA2D初始化
    LCD_BLK_Set();   // 开背光
}
