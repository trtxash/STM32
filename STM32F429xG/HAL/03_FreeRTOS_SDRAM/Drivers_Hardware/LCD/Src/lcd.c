#include "lcd.h"
#include "dma2d.h"
#include "ltdc.h"

// #include "lv_port_disp.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

static void lcd_disp_en_config(void);

void lcd_disp_config(void)
{
    lcd_disp_en_config();
    MX_LTDC_Init();
    LCD_DISP_Set(); // 开背光
}

static void lcd_disp_en_config(void)
{
    GPIO_InitTypeDef GPIO_InitTure = {0};

    LCD_DISP_GPIO_CLK_ENABLE(); // 开启GPIO时钟
    /*ARM的芯片都是这样，外设通常都是给了时钟后，才能设置它的寄存器,这么做的目的是为了省电，使用了所谓时钟门控的技术。*/

    /*进行结构体内的参数配置，先找到下面HAL_GPIO_Init();的定义处，再对定义处的函数详细找参数*/
    GPIO_InitTure.Mode = GPIO_MODE_OUTPUT_PP;        // 推挽输出
    GPIO_InitTure.Pull = GPIO_NOPULL;                // 上拉
    GPIO_InitTure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // 高速
    GPIO_InitTure.Pin = LCD_DISP_PIN;                // 设置GPIOx的5口
    HAL_GPIO_Init(LCD_DISP_GPIO, &GPIO_InitTure);    // 先在上面四行设置GPIO的模式，上下拉，速度，再对GPIOB管脚初始化
}
