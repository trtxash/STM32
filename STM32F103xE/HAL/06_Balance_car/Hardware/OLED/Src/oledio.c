/**
 * @file	  oledio.c
 * @brief 	OLED相关io配置
 * @author 	TRTX-gamer      https://github.com/TRTX-gamer；
 *          突然吐血    https://space.bilibili.com/12890038;
 * @version 1.1
 * @date 	  2022年10月28号14点07分
 */

#include "oledio.h"

#if _SOFT_OR_HARE == OLED_SOFT // 是否为软件模拟

#if _DRIVE_INTERFACE_TYPE == OLED_IIC_INTERFACE // 是否为IIC通信

/**
 * @brief      硬件底层初始化.
 * @retval     None.
 */
void OledDrv_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure = {0};

  /* GPIO时钟开启 */
  OLED_RST_Port_Clk_Enable();
  OLED_SCLK_Port_Clk_Enable();
  OLED_SDIN_Port_Clk_Enable();

  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Pin = OLED_RST_Pin;
  HAL_GPIO_Init(OLED_RST_Port, &GPIO_InitStructure);
  GPIO_InitStructure.Pin = OLED_SCLK_Pin;
  HAL_GPIO_Init(OLED_SCLK_Port, &GPIO_InitStructure);
  GPIO_InitStructure.Pin = OLED_SDIN_Pin;
  HAL_GPIO_Init(OLED_SDIN_Port, &GPIO_InitStructure);

  HAL_GPIO_WritePin(OLED_SCLK_Port, OLED_SCLK_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(OLED_SDIN_Port, OLED_SDIN_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(OLED_RST_Port, OLED_RST_Pin, GPIO_PIN_RESET);
  delay_ms(200);
  HAL_GPIO_WritePin(OLED_RST_Port, OLED_RST_Pin, GPIO_PIN_SET);
}

/**
 * @brief      IIC延时.
 * @retval     None.
 */
#pragma GCC push_options // 防止GCC优化掉延时函数
#pragma GCC optimize("O0")
void OledDrv_IICDelay(void)
{
  u8 t = OledDrv_IICDelay_Time;

  while (t--)
  {
  }
}
#pragma GCC pop_options

/**
 * @brief      IIC 通信开始.
 * @retval     None.
 */
void OledDrv_IICStart(void)
{
  OLED_SDA_OUT(); // SDA线 输出
  OLED_SDIN_Set();
  OledDrv_IICDelay();
  OledDrv_IICDelay();
  OLED_SCLK_Set();
  OledDrv_IICDelay();
  OLED_SDIN_Clr(); // START:当SCL线处于高电平时,SDA线突然从高变低,发送起始信号
  OledDrv_IICDelay();
  OLED_SCLK_Clr(); // 钳住I2C总线，准备发送或接收数据
}

/**
 * @brief      IIC 通信停止.
 * @retval     None.
 */
void OledDrv_IICStop(void)
{
  OLED_SDA_OUT(); // SDA线输出
  OLED_SCLK_Clr();
  OLED_SDIN_Clr(); // STOP:当SCL线处于高电平时,SDA线突然从低变高,发送停止信号
  OledDrv_IICDelay();
  OLED_SCLK_Set();
  OLED_SDIN_Set(); // 发送I2C总线结束信号
  OledDrv_IICDelay();
}

/**
 * @brief      IIC 等待信号响应.
 * @retval     None.
 */
u8 OledDrv_IICWaitAck(void)
{
  u8 ucErrTime = 0;
  // OLED_SDIN_Set();
  OLED_SDA_IN(); // SDA设置为输入
  OledDrv_IICDelay();
  OledDrv_IICDelay();
  OLED_SCLK_Set();
  OledDrv_IICDelay();
  while (OLED_READ_SDIN())
  {
    ucErrTime++;
    if (ucErrTime > 250)
    {
      OledDrv_IICStop();
      return 1;
    }
  }
  OledDrv_IICDelay();
  OLED_SCLK_Clr(); // 时钟输出0
  return 0;
}

/**
 * @brief      IIC产生应答信号
 * @retval     None.
 */
void OledDrv_IICAck(void)
{
  OLED_SCLK_Clr();
  OLED_SDA_OUT();
  OledDrv_IICDelay();
  OLED_SDIN_Clr();
  OledDrv_IICDelay();
  OLED_SCLK_Set();
  OledDrv_IICDelay();
  OledDrv_IICDelay();
  OLED_SCLK_Clr();
}

/**
 * @brief      IIC不产生应答信号
 * @retval     None.
 */
void OledDrv_IICNAck(void)
{
  OLED_SCLK_Clr();
  OLED_SDA_OUT();
  OledDrv_IICDelay();
  OLED_SDIN_Set();
  OledDrv_IICDelay();
  OLED_SCLK_Set();
  OledDrv_IICDelay();
  OledDrv_IICDelay();
  OLED_SCLK_Clr();
}

/**
 * @brief      IIC 写以一个字节.
 * @param[in]  data  字节数据.
 * @retval     None.
 */
void OledDrv_IICSendByte(uint8_t data)
{
  u8 t;
  OLED_SDA_OUT();
  OLED_SCLK_Clr(); // 拉低时钟开始数据传输
  for (t = 0; t < 8; t++)
  {
    OledDrv_IICDelay();
    if ((data & 0x80) >> 7)
      OLED_SDIN_Set();
    else
      OLED_SDIN_Clr();
    data <<= 1;
    OledDrv_IICDelay();
    OLED_SCLK_Set();
    OledDrv_IICDelay();
    OledDrv_IICDelay();
    OLED_SCLK_Clr();
  }
}

/**
 * @brief      IIC 读以一个字节.
 * @param[in]  ack 1,发送ACK   0,发送NACK.
 * @retval     接收到的数据.
 */
u8 OledDrv_IICReadByte(u8 ack)
{
  u8 i, receive = 0;
  OLED_SDA_IN(); // SDA设置为输入
  for (i = 0; i < 8; i++)
  {
    OLED_SCLK_Clr();
    OledDrv_IICDelay();
    OledDrv_IICDelay();
    OLED_SCLK_Set();
    OledDrv_IICDelay();
    receive <<= 1;
    if (OLED_READ_SDIN())
      receive++; // 如果读到了数据
    OledDrv_IICDelay();
  }
  if (!ack)
    OledDrv_IICNAck(); // 发送nACK
  else
    OledDrv_IICAck(); // 发送ACK
  return receive;
}

#elif _DRIVE_INTERFACE_TYPE == OLED_SPI_INTERFACE // SPI通信

/**
 * @brief      硬件底层初始化.
 * @retval     None.
 */
void OledDrv_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure = {0};

  /* GPIO时钟开启 */
  OLED_CS_Port_Clk_Enable();
  OLED_DC_Port_Clk_Enable();
  OLED_RST_Port_Clk_Enable();
  OLED_DIN_Port_Clk_Enable();
  OLED_CLK_Port_Clk_Enable();

  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; // 推挽输出
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Pull = GPIO_PULLUP;

  GPIO_InitStructure.Pin = OLED_CS_Pin;
  HAL_GPIO_Init(OLED_CS_Port, &GPIO_InitStructure);
  GPIO_InitStructure.Pin = OLED_DC_Pin;
  HAL_GPIO_Init(OLED_DC_Port, &GPIO_InitStructure);
  GPIO_InitStructure.Pin = OLED_RST_Pin;
  HAL_GPIO_Init(OLED_RST_Port, &GPIO_InitStructure);
  GPIO_InitStructure.Pin = OLED_DIN_Pin;
  HAL_GPIO_Init(OLED_DIN_Port, &GPIO_InitStructure);
  GPIO_InitStructure.Pin = OLED_CLK_Pin;
  HAL_GPIO_Init(OLED_CLK_Port, &GPIO_InitStructure);

  HAL_GPIO_WritePin(OLED_CS_Port, OLED_CS_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(OLED_DC_Port, OLED_DC_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(OLED_RST_Port, OLED_RST_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(OLED_CLK_Port, OLED_CLK_Pin, GPIO_PIN_SET);

  OLED_RST_Clr();
  delay_ms(200);
  OLED_RST_Set();
}

#endif

#elif _SOFT_OR_HARE == OLED_HARD // 硬件

#if _DRIVE_INTERFACE_TYPE == OLED_IIC_INTERFACE

/**
 * @brief      硬件底层初始化.
 * @retval     None.
 */
void OledDrv_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure = {0};

  /* GPIO时钟开启 */
  OLED_RST_Port_Clk_Enable();

  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; // 推挽输出
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Pin = OLED_RST_Pin;
  HAL_GPIO_Init(OLED_RST_Port, &GPIO_InitStructure);

  HAL_GPIO_WritePin(OLED_RST_Port, OLED_RST_Pin, GPIO_PIN_RESET);
  delay_ms(200);
  HAL_GPIO_WritePin(OLED_RST_Port, OLED_RST_Pin, GPIO_PIN_SET);
}

#elif _DRIVE_INTERFACE_TYPE == OLED_SPI_INTERFACE // SPI通信

/**
 * @brief      硬件底层初始化.
 * @retval     None.
 */
void OledDrv_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure = {0};

  /* GPIO时钟开启 */
  OLED_CS_Port_Clk_Enable();
  OLED_DC_Port_Clk_Enable();
  OLED_RST_Port_Clk_Enable();

  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; // 推挽输出
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Pull = GPIO_PULLUP;

  GPIO_InitStructure.Pin = OLED_CS_Pin;
  HAL_GPIO_Init(OLED_CS_Port, &GPIO_InitStructure);
  GPIO_InitStructure.Pin = OLED_DC_Pin;
  HAL_GPIO_Init(OLED_DC_Port, &GPIO_InitStructure);
  GPIO_InitStructure.Pin = OLED_RST_Pin;
  HAL_GPIO_Init(OLED_RST_Port, &GPIO_InitStructure);

  HAL_GPIO_WritePin(OLED_CS_Port, OLED_CS_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(OLED_DC_Port, OLED_DC_Pin, GPIO_PIN_SET);

  OLED_RST_Clr();
  delay_ms(200);
  OLED_RST_Set();
}

#endif

#endif