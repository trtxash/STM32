#include "myiic.h"
#include "delay.h"
#include "oledio.h"
#include "stm32f4xx_hal_gpio.h"

birch_iic_bus_t birch_iic_bus[1] = {
    [0] = {.RST_GPIOx = OLED_RST_Port, .RST_Pin = OLED_RST_Pin, .SCL_GPIOx = OLED_SCLK_Port, .SCL_Pin = OLED_SCLK_Pin, .SDA_GPIOx = OLED_SDIN_Port, .SDA_Pin = OLED_SDIN_Pin, .Drv_IICDelay_Time = 0}, // 第一条IIC总线
    // [1] = {.RST_GPIOx = GPIOA, .RST_Pin = GPIO_PIN_5, .SCL_GPIOx = GPIOB, .SCL_Pin = GPIO_PIN_5, .SDA_GPIOx = GPIOB, .SDA_Pin = GPIO_PIN_6}  // 第一条IIC总线
};

static void enable_gpio_clk(GPIO_TypeDef *GPIOx)
{
    if (GPIOx == GPIOA)
        __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (GPIOx == GPIOB)
        __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (GPIOx == GPIOC)
        __HAL_RCC_GPIOC_CLK_ENABLE();
    else if (GPIOx == GPIOD)
        __HAL_RCC_GPIOD_CLK_ENABLE();
    else if (GPIOx == GPIOE)
        __HAL_RCC_GPIOE_CLK_ENABLE();
    else if (GPIOx == GPIOF)
        __HAL_RCC_GPIOF_CLK_ENABLE();
    else if (GPIOx == GPIOG)
        __HAL_RCC_GPIOG_CLK_ENABLE();
    else if (GPIOx == GPIOH)
        __HAL_RCC_GPIOH_CLK_ENABLE();
    else if (GPIOx == GPIOI)
        __HAL_RCC_GPIOI_CLK_ENABLE();
    else if (GPIOx == GPIOJ)
        __HAL_RCC_GPIOJ_CLK_ENABLE();
    else if (GPIOx == GPIOK)
        __HAL_RCC_GPIOK_CLK_ENABLE();
}

/**
 * @brief      硬件底层初始化.
 * @retval     None.
 */
void Drv_Init(birch_iic_bus_t *iic_bus)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    /* 统一处理GPIO时钟使能 */
    // enable_gpio_clk(iic_bus->RST_GPIOx);
    enable_gpio_clk(iic_bus->SCL_GPIOx);
    enable_gpio_clk(iic_bus->SDA_GPIOx);

    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    // GPIO_InitStructure.Pin = iic_bus->RST_Pin;
    // HAL_GPIO_Init(iic_bus->RST_GPIOx, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = iic_bus->SCL_Pin;
    HAL_GPIO_Init(iic_bus->SCL_GPIOx, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = iic_bus->SDA_Pin;
    HAL_GPIO_Init(iic_bus->SDA_GPIOx, &GPIO_InitStructure);

    HAL_GPIO_WritePin(iic_bus->SCL_GPIOx, iic_bus->SCL_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(iic_bus->SDA_GPIOx, iic_bus->SDA_Pin, GPIO_PIN_SET);
    // HAL_GPIO_WritePin(iic_bus->RST_GPIOx, iic_bus->RST_Pin, GPIO_PIN_RESET);
    // delay_ms(200);
    // HAL_GPIO_WritePin(iic_bus->RST_GPIOx, iic_bus->RST_Pin, GPIO_PIN_SET);
}

/**
 * @brief      IIC延时.
 * @retval     None.
 */
#pragma GCC push_options // 防止GCC优化掉延时函数
#pragma GCC optimize("O0")
void Drv_IICDelay(birch_iic_bus_t *iic_bus)
{
    u8 t = iic_bus->Drv_IICDelay_Time;

    while (t--)
    {
    }
}
#pragma GCC pop_options

static inline void SDIN_OUT(birch_iic_bus_t *iic_bus)
{
    uint32_t pin_pos = __builtin_ffs(iic_bus->SDA_Pin) - 1; // 获取引脚序号
    iic_bus->SDA_GPIOx->MODER &= ~(3 << (pin_pos * 2));     // 动态计算偏移量
    iic_bus->SDA_GPIOx->MODER |= 1 << (pin_pos * 2);        // 按实际引脚配置输出模式
    // iic_bus->SDA_GPIOx->MODER &= ~(3 << (7 * 2));
    // iic_bus->SDA_GPIOx->MODER |= 1 << 7 * 2;
}

static inline void SDIN_IN(birch_iic_bus_t *iic_bus)
{
    uint32_t pin_pos = __builtin_ffs(iic_bus->SDA_Pin) - 1; // 获取引脚序号
    iic_bus->SDA_GPIOx->MODER &= ~(3 << (pin_pos * 2));     // 动态计算偏移量
    iic_bus->SDA_GPIOx->MODER |= 0 << (pin_pos * 2);
}

static inline void SDIN_Set(birch_iic_bus_t *iic_bus)
{
    iic_bus->SDA_GPIOx->BSRR = iic_bus->SDA_Pin;
}

static inline void SDIN_Clr(birch_iic_bus_t *iic_bus)
{
    iic_bus->SDA_GPIOx->BSRR = (uint32_t)(iic_bus->SDA_Pin) << 16U;
}

static inline void SCLK_Set(birch_iic_bus_t *iic_bus)
{
    iic_bus->SCL_GPIOx->BSRR = iic_bus->SCL_Pin;
}

static inline void SCLK_Clr(birch_iic_bus_t *iic_bus)
{
    iic_bus->SCL_GPIOx->BSRR = (uint32_t)(iic_bus->SCL_Pin) << 16U;
}

static inline u8 SDIN_READ(birch_iic_bus_t *iic_bus)
{
    if (iic_bus->SDA_GPIOx->IDR & iic_bus->SDA_Pin)
        return 1;
    else
        return 0;
}

/**
 * @brief      IIC 通信开始.
 * @retval     None.
 */
void Drv_IICStart(birch_iic_bus_t *iic_bus)
{
    SDIN_OUT(iic_bus);
    SDIN_Set(iic_bus);
    Drv_IICDelay(iic_bus);
    Drv_IICDelay(iic_bus);
    SCLK_Set(iic_bus);
    Drv_IICDelay(iic_bus);
    SDIN_Clr(iic_bus); // START:当SCL线处于高电平时,SDA线突然从高变低,发送起始信号
    Drv_IICDelay(iic_bus);
    SCLK_Clr(iic_bus); // 钳住I2C总线，准备发送或接收数据
}

/**
 * @brief      IIC 通信停止.
 * @retval     None.
 */
void Drv_IICStop(birch_iic_bus_t *iic_bus)
{
    SDIN_OUT(iic_bus);
    SCLK_Clr(iic_bus);
    SDIN_Clr(iic_bus); // STOP:当SCL线处于高电平时,SDA线突然从低变高,发送停止信号
    Drv_IICDelay(iic_bus);
    SCLK_Set(iic_bus);
    SDIN_Set(iic_bus); // 发送I2C总线结束信号
    Drv_IICDelay(iic_bus);
}

/**
 * @brief      IIC 等待信号响应.
 * @retval     None.
 */
u8 Drv_IICWaitAck(birch_iic_bus_t *iic_bus)
{
    u8 ucErrTime = 0;

    SDIN_IN(iic_bus); // SDA设置为输入
    Drv_IICDelay(iic_bus);
    SCLK_Set(iic_bus);
    Drv_IICDelay(iic_bus);
    while (SDIN_READ(iic_bus))
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            Drv_IICStop(iic_bus);
            return 1;
        }
    }
    Drv_IICDelay(iic_bus);
    SCLK_Clr(iic_bus);

    return 0;
}

/**
 * @brief      IIC产生应答信号
 * @retval     None.
 */
void Drv_IICAck(birch_iic_bus_t *iic_bus)
{
    SCLK_Clr(iic_bus);
    SDIN_OUT(iic_bus);
    Drv_IICDelay(iic_bus);
    SDIN_Clr(iic_bus);
    Drv_IICDelay(iic_bus);
    SCLK_Set(iic_bus);
    Drv_IICDelay(iic_bus);
    Drv_IICDelay(iic_bus);
    SCLK_Clr(iic_bus);
}

/**
 * @brief      IIC不产生应答信号
 * @retval     None.
 */
void Drv_IICNAck(birch_iic_bus_t *iic_bus)
{
    SCLK_Clr(iic_bus);
    SDIN_OUT(iic_bus);
    Drv_IICDelay(iic_bus);
    SDIN_Set(iic_bus);
    Drv_IICDelay(iic_bus);
    SCLK_Set(iic_bus);
    Drv_IICDelay(iic_bus);
    Drv_IICDelay(iic_bus);
    SCLK_Clr(iic_bus);
}

/**
 * @brief      IIC 写以一个字节.
 * @param[in]  data  字节数据.
 * @retval     None.
 */
void Drv_IICSendByte(uint8_t data, birch_iic_bus_t *iic_bus)
{
    u8 t;
    SDIN_OUT(iic_bus);
    SCLK_Clr(iic_bus); // 拉低时钟开始数据传输
    for (t = 0; t < 8; t++)
    {
        Drv_IICDelay(iic_bus);
        if (data & 0x80)
            SDIN_Set(iic_bus);
        else
            SDIN_Clr(iic_bus);
        data <<= 1;
        Drv_IICDelay(iic_bus);
        SCLK_Set(iic_bus);
        Drv_IICDelay(iic_bus);
        Drv_IICDelay(iic_bus);
        SCLK_Clr(iic_bus);
    }
}

/**
 * @brief      IIC 读以一个字节.
 * @param[in]  ack 1,发送ACK   0,发送NACK.
 * @retval     接收到的数据.
 */
u8 Drv_IICReadByte(u8 ack, birch_iic_bus_t *iic_bus)
{
    u8 i, receive = 0;
    SDIN_IN(iic_bus); // SDA设置为输入
    for (i = 0; i < 8; i++)
    {
        SCLK_Clr(iic_bus);
        Drv_IICDelay(iic_bus);
        Drv_IICDelay(iic_bus);
        SCLK_Set(iic_bus);
        Drv_IICDelay(iic_bus);
        receive <<= 1;
        if (SDIN_READ(iic_bus))
            receive++; // 如果读到了数据
        Drv_IICDelay(iic_bus);
    }
    if (!ack)
        Drv_IICNAck(iic_bus); // 发送nACK
    else
        Drv_IICAck(iic_bus); // 发送ACK
    return receive;
}
