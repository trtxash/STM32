#include "oledio.h"

#if _SOFT_OR_HARE == OLED_SOFT

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
  OLED_SCLK_Port_Clk_Enable();
  OLED_SDIN_Port_Clk_Enable();

  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出
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
  delay_ms(100);
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
  OLED_SDIN_Set();
  OLED_SCLK_Set();
  OledDrv_IICDelay();
  OLED_SDIN_Clr();
  OledDrv_IICDelay();
  OLED_SCLK_Clr();
  OledDrv_IICDelay();
}

/**
 * @brief      IIC 通信停止.
 * @retval     None.
 */
void OledDrv_IICStop(void)
{
  OLED_SDIN_Clr();
  OLED_SCLK_Clr();
  OledDrv_IICDelay();
  OLED_SDIN_Set();
}

/**
 * @brief      IIC 等待信号响应.
 * @retval     None.
 */
void OledDrv_IICWaitAck(void)
{
  OLED_SDIN_Set();
  OledDrv_IICDelay();
  OLED_SCLK_Set();
  OledDrv_IICDelay();
  OLED_SCLK_Clr();
  OledDrv_IICDelay();
}

/**
 * @brief      IIC 写以一个字节.
 * @param[in]  data  字节数据.
 * @retval     None.
 */
void OledDrv_IICSendByte(uint8_t data)
{
  u8 i;

  for (i = 0; i < 8; i++)
  {
    if (data & 0x80)
    {
      OLED_SDIN_Set();
    }
    else
    {
      OLED_SDIN_Clr();
    }
    OledDrv_IICDelay();
    OLED_SCLK_Set();
    OledDrv_IICDelay();
    OLED_SCLK_Clr();
    data <<= 1;
  }
}

#else

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

  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
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
  delay_ms(100);
  OLED_RST_Set();
}

#endif

#else

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

  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Pin = OLED_RST_Pin;
  HAL_GPIO_Init(OLED_RST_Port, &GPIO_InitStructure);

  HAL_GPIO_WritePin(OLED_RST_Port, OLED_RST_Pin, GPIO_PIN_RESET);
  delay_ms(100);
  HAL_GPIO_WritePin(OLED_RST_Port, OLED_RST_Pin, GPIO_PIN_SET);
}

#else

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

  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出
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
  delay_ms(100);
  OLED_RST_Set();
}

#endif

#endif