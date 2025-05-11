#include "lcd.h"
// #include "lv_port_disp.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define HORIZONTAL_SYNCHRONOUS_PULSE 10
#define HORIZONTAL_BACK_PORCH        150
#define ACTIVE_WIDTH                 800
#define HORIZONTAL_FRONT_PORCH       15

#define VERTICAL_SYNCHRONOUS_PULSE 10
#define VERTICAL_BACK_PORCH        140
#define ACTIVE_HEIGHT              480
#define VERTICAL_FRONT_PORCH       40

#define LCD_FRAME_BUF_ADDR 0XC0000000
uint16_t ltdc_lcd_framebuf0[800][480] __attribute__((at(LCD_FRAME_BUF_ADDR)));

static void tli_gpio_config(void);
static void tli_config(void);
static void ipa_config(void);
static void lcd_disp_en_config(void);
static void lcd_disp_off(void);
static void lcd_disp_on(void);

void lcd_disp_config(void)
{
    lcd_disp_en_config();
    lcd_disp_off();

    /* configure the GPIO of TLI */
    tli_gpio_config();
    lcd_disp_on();

    tli_config();
    tli_layer_enable(LAYER0);
    tli_reload_config(TLI_FRAME_BLANK_RELOAD_EN);
    tli_enable();
    ipa_config();
}

static void tli_config(void)
{
    tli_parameter_struct tli_init_struct;
    tli_layer_parameter_struct tli_layer_init_struct;

    rcu_periph_clock_enable(RCU_TLI);
    tli_gpio_config();
    /* configure PLLSAI to generate TLI clock */
    // if(ERROR == rcu_pllsai_config(216, 2, 3)){
    if (ERROR == rcu_pllsai_config(192, 2, 3))
    {
        while (1)
            ;
    }
    rcu_tli_clock_div_config(RCU_PLLSAIR_DIV2);

    rcu_osci_on(RCU_PLLSAI_CK);

    if (ERROR == rcu_osci_stab_wait(RCU_PLLSAI_CK))
    {
        while (1)
            ;
    }

    /* configure TLI parameter struct */
    tli_init_struct.signalpolarity_hs = TLI_HSYN_ACTLIVE_LOW;
    tli_init_struct.signalpolarity_vs = TLI_VSYN_ACTLIVE_LOW;
    tli_init_struct.signalpolarity_de = TLI_DE_ACTLIVE_LOW;
    tli_init_struct.signalpolarity_pixelck = TLI_PIXEL_CLOCK_TLI;
    /* LCD display timing configuration */
    tli_init_struct.synpsz_hpsz = HORIZONTAL_SYNCHRONOUS_PULSE - 1;
    tli_init_struct.synpsz_vpsz = VERTICAL_SYNCHRONOUS_PULSE - 1;
    tli_init_struct.backpsz_hbpsz = HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH - 1;
    tli_init_struct.backpsz_vbpsz = VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH - 1;
    tli_init_struct.activesz_hasz = HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH + ACTIVE_WIDTH - 1;
    tli_init_struct.activesz_vasz = VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH + ACTIVE_HEIGHT - 1;
    tli_init_struct.totalsz_htsz = HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH + ACTIVE_WIDTH + HORIZONTAL_FRONT_PORCH - 1;
    tli_init_struct.totalsz_vtsz = VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH + ACTIVE_HEIGHT + VERTICAL_FRONT_PORCH - 1;
    /* configure LCD background R,G,B values */
    tli_init_struct.backcolor_red = 0xFF;
    tli_init_struct.backcolor_green = 0xFF;
    tli_init_struct.backcolor_blue = 0xFF;
    tli_init(&tli_init_struct);

#if 1
    /* TLI layer0 configuration */
    /* TLI window size configuration */
    tli_layer_init_struct.layer_window_leftpos = HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH;
    tli_layer_init_struct.layer_window_rightpos = (ACTIVE_WIDTH + HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH - 1);
    tli_layer_init_struct.layer_window_toppos = VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH;
    tli_layer_init_struct.layer_window_bottompos = (ACTIVE_HEIGHT + VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH - 1);
    /* TLI window pixel format configuration */
    tli_layer_init_struct.layer_ppf = LAYER_PPF_RGB565;
    /* TLI window specified alpha configuration */
    tli_layer_init_struct.layer_sa = 255;
    /* TLI layer default alpha R,G,B value configuration */
    tli_layer_init_struct.layer_default_blue = 0x00;
    tli_layer_init_struct.layer_default_green = 0x00;
    tli_layer_init_struct.layer_default_red = 0x00;
    tli_layer_init_struct.layer_default_alpha = 0x00;
    /* TLI window blend configuration */
    tli_layer_init_struct.layer_acf1 = LAYER_ACF1_SA;
    tli_layer_init_struct.layer_acf2 = LAYER_ACF2_SA;
    /* TLI layer frame buffer base address configuration */
    tli_layer_init_struct.layer_frame_bufaddr = (uint32_t)ltdc_lcd_framebuf0;
    tli_layer_init_struct.layer_frame_line_length = ((ACTIVE_WIDTH * 2) + 3);
    tli_layer_init_struct.layer_frame_buf_stride_offset = (ACTIVE_WIDTH * 2);
    tli_layer_init_struct.layer_frame_total_line_number = ACTIVE_HEIGHT;
    tli_layer_init(LAYER0, &tli_layer_init_struct);
    tli_dither_config(TLI_DITHER_ENABLE);
#endif
}

/*!
    \brief      IPA initialize and configuration
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void ipa_config(void)
{
    rcu_periph_clock_enable(RCU_IPA);
    nvic_irq_enable(IPA_IRQn, 0, 2);
}

static void tli_gpio_config(void)
{
    /* enable the periphral clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOF);
    rcu_periph_clock_enable(RCU_GPIOG);

    /* configure HSYNC(PC6), VSYNC(PA4), PCLK(PG7), DE(PF10) */
    /* configure LCD_R7(PG6), LCD_R6(PA8), LCD_R5(PA12), LCD_R4(PA11), LCD_R3(PB0),
                 LCD_G7(PD3), LCD_G6(PC7), LCD_G5(PB11), LCD_G4(PB10), LCD_G3(PG10), LCD_G2(PA6),
                 LCD_B7(PB9), LCD_B6(PB8), LCD_B5(PA3), LCD_B4(PG12), LCD_B3(PG11) */
    gpio_af_set(GPIOA, GPIO_AF_14, GPIO_PIN_3);
    gpio_af_set(GPIOA, GPIO_AF_14, GPIO_PIN_4);
    gpio_af_set(GPIOA, GPIO_AF_14, GPIO_PIN_6);
    gpio_af_set(GPIOA, GPIO_AF_14, GPIO_PIN_8);
    gpio_af_set(GPIOA, GPIO_AF_14, GPIO_PIN_11);
    gpio_af_set(GPIOA, GPIO_AF_14, GPIO_PIN_12);

    gpio_af_set(GPIOB, GPIO_AF_9, GPIO_PIN_0);
    gpio_af_set(GPIOB, GPIO_AF_14, GPIO_PIN_8);
    gpio_af_set(GPIOB, GPIO_AF_14, GPIO_PIN_9);
    gpio_af_set(GPIOB, GPIO_AF_14, GPIO_PIN_10);
    gpio_af_set(GPIOB, GPIO_AF_14, GPIO_PIN_11);

    gpio_af_set(GPIOC, GPIO_AF_14, GPIO_PIN_6);
    gpio_af_set(GPIOC, GPIO_AF_14, GPIO_PIN_7);

    gpio_af_set(GPIOD, GPIO_AF_14, GPIO_PIN_3);

    gpio_af_set(GPIOF, GPIO_AF_14, GPIO_PIN_10);

    gpio_af_set(GPIOG, GPIO_AF_14, GPIO_PIN_6);
    gpio_af_set(GPIOG, GPIO_AF_14, GPIO_PIN_7);
    gpio_af_set(GPIOG, GPIO_AF_9, GPIO_PIN_10);
    gpio_af_set(GPIOG, GPIO_AF_14, GPIO_PIN_11);
    gpio_af_set(GPIOG, GPIO_AF_9, GPIO_PIN_12);

    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_11 | GPIO_PIN_12);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_11 | GPIO_PIN_12);

    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11);

    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6 | GPIO_PIN_7);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7);

    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);

    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12);
}

/*!
    \brief      configure DISP ON/OFF GPIO
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void lcd_disp_en_config(void)
{
    /* enable the periphral clock */
    rcu_periph_clock_enable(GPIOD);
    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_13);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
}

/*!
    \brief      DISP GPIO OFF
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void lcd_disp_off(void)
{
    gpio_bit_reset(GPIOD, GPIO_PIN_13);
}

/*!
    \brief      DISP GPIO ON
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void lcd_disp_on(void)
{
    gpio_bit_set(GPIOD, GPIO_PIN_13);
}
