/**
 **********************************************************************************************************************
 * @file    oledio.c
 * @brief   该文件提供 OLED 驱动硬件接口相关功能
 * @author  周鹏程    any question please send mail to const_zpc@163.com
 * @version V1.0.0
 * @date    2021-3-12
 *
 * @details  功能详细说明：
 *           + 接口驱动通信接口定义
 *           + 驱动初始化函数
 *           + 驱动通信函数
 *
 **********************************************************************************************************************
 *
 *
 **********************************************************************************************************************
 */

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "oledio.h"

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/
/* Private function --------------------------------------------------------------------------------------------------*/

#if _DRIVE_INTERFACE_TYPE == OLED_IIC_INTERFACE

/**
 * @brief      硬件底层初始化.
 * @retval     None.
 */
void OledDrv_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIO时钟开启 */
  OLED_SCLK_Port_Clk_Enable();
  OLED_SDIN_Port_Clk_Enable();

  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Pin = OLED_SCLK_Pin;
  HAL_GPIO_Init(OLED_SCLK_Port, &GPIO_InitStructure);

  GPIO_InitStructure.Pin = OLED_SDIN_Pin;
  HAL_GPIO_Init(OLED_SDIN_Port, &GPIO_InitStructure);

  HAL_GPIO_WritePin(OLED_SCLK_Port, OLED_SCLK_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(OLED_SDIN_Port, OLED_SDIN_Pin, GPIO_PIN_SET);
}

/**
 * @brief      IIC 通信开始.
 * @retval     None.
 */
void OledDrv_IICStart(void)
{
  OLED_SCLK_Set();
  OLED_SDIN_Set();
  OLED_SDIN_Clr();
  OLED_SCLK_Clr();
}

/**
 * @brief      IIC 通信停止.
 * @retval     None.
 */
void OledDrv_IICStop(void)
{
  OLED_SCLK_Clr();
  OLED_SDIN_Clr();
  OLED_SDIN_Set();
  OLED_SCLK_Set();
}

/**
 * @brief      IIC 写以一个字节.
 * @param[in]  data  字节数据.
 * @retval     None.
 */
void OledDrv_IICWriteByte(uint8_t data)
{
  char i = 8;

  while (i--)
  {
    OLED_SCLK_Clr();

    if (data & 0x80)
    {
      OLED_SDIN_Set();
    }
    else
    {
      OLED_SDIN_Clr();
    }

    OLED_SCLK_Set();
    data <<= 1;
  }

  OLED_SCLK_Clr();
  OLED_SCLK_Set();
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

  HAL_GPIO_WritePin(OLED_DIN_Port, OLED_DIN_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(OLED_CLK_Port, OLED_CLK_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(OLED_RST_Port, OLED_RST_Pin, GPIO_PIN_RESET);
  delay_ms(1);
  HAL_GPIO_WritePin(OLED_RST_Port, OLED_RST_Pin, GPIO_PIN_SET);
}

/**
 * @brief      SPI 写以一个字节.
 * @param[in]  data  字节数据.
 * @retval     None.
 */
void OledDrv_SPIWriteByte(uint8_t data)
{
  char i = 8;

  OLED_CLK_Clr();

  while (i--)
  {
    if (data & 0x80)
    {
      OLED_DIN_Set();
    }
    else
    {
      OLED_DIN_Clr();
    }

    OLED_CLK_Set();
    OLED_CLK_Clr();
    data <<= 1;
  }
}

#endif
