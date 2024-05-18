/*
 * Copyright (c) 2023 感为智能科技(济南)
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 */

#include "sw_i2c.h"

#define ACK 0x0	 // acknowledge (SDA LOW)
#define NACK 0x1 // not acknowledge (SDA HIGH)

#define LOW 0x0
#define HIGH 0x1

#define I2C_READ 0x1
#define I2C_WRITE 0x0

static void sw_i2c_hal_start(sw_i2c_interface_t *i2c_interface);
static void sw_i2c_hal_stop(sw_i2c_interface_t *i2c_interface);

static void sw_i2c_hal_write_bit(sw_i2c_interface_t *i2c_interface, uint8_t bit);
static uint8_t sw_i2c_hal_read_bit(sw_i2c_interface_t *i2c_interface);

static uint8_t sw_i2c_hal_write_byte(sw_i2c_interface_t *i2c_interface, uint8_t byte);
static uint8_t sw_i2c_hal_read_byte(sw_i2c_interface_t *i2c_interface, uint8_t ack);

void sw_i2cinit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pins : PB8 PB9 */
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = SW_I2C1_PIN_SCL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = SW_I2C1_PIN_SDA;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* 定义sda输出函数 bit=0为低电平 bit=1为高电平 */
void sda_out(uint8_t bit, void *user_data)
{
	HAL_GPIO_WritePin(GPIOB, SW_I2C1_PIN_SDA, (GPIO_PinState)bit);
	/* IIC软件延迟 */
	delay_us(10);
}

/* 定义sda读取函数 bit 为返回的电平值 */
uint8_t sda_in(void *user_data)
{
	uint8_t bit;
	bit = HAL_GPIO_ReadPin(GPIOB, SW_I2C1_PIN_SDA);
	/* IIC软件延迟 */
	delay_us(10);
	return bit;
}

/* 定义scl时钟输出函数 bit=0为低电平 bit=1为高电平 */
void scl_out(uint8_t bit, void *user_data)
{
	HAL_GPIO_WritePin(GPIOC, SW_I2C1_PIN_SCL, (GPIO_PinState)bit);
	/* IIC软件延迟 */
	delay_us(10);
}

/**
 * i2c地址扫描
 * @param scan_addr 扫描出来的地址存放,数值不为0的为扫描到的地址，扫到的地址会挨个放在数组的最前面
 * @return 返回扫描到的设备数量, 0为无设备发现
 */
uint8_t i2c_scan(sw_i2c_interface_t *i2c_interface, uint8_t *scan_addr)
{
	int i;
	uint8_t count = 0;
	uint8_t data;
	int8_t ret;

	for (i = 1; i < 127; ++i)
	{
		ret = sw_i2c_read(i2c_interface, i << 1, &data, 1);
		if (ret == 0)
		{
			scan_addr[count] = i;
			++count;
		}
	}

	return count;
}

int8_t sw_i2c_read(sw_i2c_interface_t *i2c_interface, uint8_t dev_addr, uint8_t *data, uint8_t data_length)
{
	uint8_t i;
	uint8_t ack_bit;

	/* 起始位 */
	sw_i2c_hal_start(i2c_interface);

	/* 地址+读写位 */
	ack_bit = sw_i2c_hal_write_byte(i2c_interface, dev_addr | I2C_READ);
	if (ack_bit)
	{
		/* 从设备没有回复ACK,直接退出 */
		sw_i2c_hal_stop(i2c_interface);
		return 1;
	}

	/* 连续读取N-1个数据 给ACK */
	for (i = 0; i < data_length - 1; ++i)
	{
		data[i] = sw_i2c_hal_read_byte(i2c_interface, ACK);
	}

	/* 最后一个数据给 NACK */
	data[i] = sw_i2c_hal_read_byte(i2c_interface, NACK);

	/* 停止位 */
	sw_i2c_hal_stop(i2c_interface);
	return 0;
}

int8_t sw_i2c_write(sw_i2c_interface_t *i2c_interface, uint8_t dev_addr, const uint8_t *data, uint8_t data_length)
{
	uint8_t i;
	uint8_t ack_bit;

	/* 起始位 */
	sw_i2c_hal_start(i2c_interface);

	/* 地址+读写位 */
	ack_bit = sw_i2c_hal_write_byte(i2c_interface, dev_addr | I2C_WRITE);
	if (ack_bit)
	{
		/* 从设备没有回复ACK,直接退出 */
		sw_i2c_hal_stop(i2c_interface);
		return 1;
	}

	/* 连续写入N个数据, 每次读取1 bit的 ACK */
	for (i = 0; i < data_length; ++i)
	{
		ack_bit = sw_i2c_hal_write_byte(i2c_interface, data[i]);
	}

	/* 停止位 */
	sw_i2c_hal_stop(i2c_interface);
	return 0;
}

int8_t sw_i2c_read_byte(sw_i2c_interface_t *i2c_interface, uint8_t dev_addr, uint8_t *data)
{
	return sw_i2c_read(i2c_interface, dev_addr, data, 1);
}

int8_t sw_i2c_write_byte(sw_i2c_interface_t *i2c_interface, uint8_t dev_addr, const uint8_t data)
{
	return sw_i2c_write(i2c_interface, dev_addr, &data, 1);
}

int8_t sw_i2c_mem_read(sw_i2c_interface_t *i2c_interface,
					   uint8_t dev_addr, uint8_t mem_addr, uint8_t *data, uint8_t data_length)
{
	uint8_t ack_bit;
	sw_i2c_hal_start(i2c_interface);
	ack_bit = sw_i2c_hal_write_byte(i2c_interface, dev_addr | I2C_WRITE);
	if (ack_bit)
	{
		sw_i2c_hal_stop(i2c_interface);
		return 1;
	}
	ack_bit = sw_i2c_hal_write_byte(i2c_interface, mem_addr);

	return sw_i2c_read(i2c_interface, dev_addr, data, data_length);
}

int8_t sw_i2c_mem_write(sw_i2c_interface_t *i2c_interface, uint8_t dev_addr, uint8_t mem_addr, const uint8_t *data,
						uint8_t data_length)
{
	uint8_t ack_bit;
	sw_i2c_hal_start(i2c_interface);
	ack_bit = sw_i2c_hal_write_byte(i2c_interface, dev_addr | I2C_WRITE);
	if (ack_bit)
	{
		sw_i2c_hal_stop(i2c_interface);
		return 1;
	}
	ack_bit = sw_i2c_hal_write_byte(i2c_interface, mem_addr);

	return sw_i2c_write(i2c_interface, dev_addr, data, data_length);
}

/***************************
 * 基礎操作抽象层
 **************************/

/**
 * @brief send start bit by driving sda and scl LOW
 * @param i2c_interface
 */
static void sw_i2c_hal_start(sw_i2c_interface_t *i2c_interface)
{
	i2c_interface->sda_out(HIGH, i2c_interface->user_data);
	i2c_interface->scl_out(HIGH, i2c_interface->user_data);
	i2c_interface->sda_out(LOW, i2c_interface->user_data);
	i2c_interface->scl_out(LOW, i2c_interface->user_data);
}

/**
 * @brief send stop bit
 * @param i2c_interface
 */
static void sw_i2c_hal_stop(sw_i2c_interface_t *i2c_interface)
{
	i2c_interface->sda_out(LOW, i2c_interface->user_data);
	i2c_interface->scl_out(HIGH, i2c_interface->user_data);
	i2c_interface->sda_out(HIGH, i2c_interface->user_data);
}

/**
 * @brief 输出 sda 电平,然后 scl 输出一个时钟
 * @param i2c_interface
 * @param bit bit level to send, 0:LOW, 1:HIGH
 */
static void sw_i2c_hal_write_bit(sw_i2c_interface_t *i2c_interface, uint8_t bit)
{
	i2c_interface->sda_out(bit, i2c_interface->user_data);
	i2c_interface->scl_out(HIGH, i2c_interface->user_data);
	i2c_interface->scl_out(LOW, i2c_interface->user_data);
}

/**
 * @brief 读 sda 电平值,然后 scl 输出一个时钟
 * @param i2c_interface
 * @return 返回 SDA 电平值, 0:LOW, 1:HIGH
 */
static uint8_t sw_i2c_hal_read_bit(sw_i2c_interface_t *i2c_interface)
{
	uint8_t bit;
	i2c_interface->sda_out(HIGH, i2c_interface->user_data);
	i2c_interface->scl_out(HIGH, i2c_interface->user_data);
	bit = i2c_interface->sda_in(i2c_interface->user_data);
	i2c_interface->scl_out(LOW, i2c_interface->user_data);
	return bit;
}

/**
 * @brief 向IIC输出一个字节
 * @param i2c_interface
 * @param byte
 * @return 从设备反馈的 ACK 电平值
 */
static uint8_t sw_i2c_hal_write_byte(sw_i2c_interface_t *i2c_interface, uint8_t byte)
{
	uint8_t i;
	uint8_t ack;

	for (i = 0; i < 8; ++i)
	{
		sw_i2c_hal_write_bit(i2c_interface, byte & (0x80 >> i));
	}

	ack = sw_i2c_hal_read_bit(i2c_interface);
	return ack;
}

/**
 * @brief 从IIC总线上读取一个字节
 * @param i2c_interface
 * @param ack 向从设备反馈 ACK 或者 NACK
 * @return 读取到的字节
 */
static uint8_t sw_i2c_hal_read_byte(sw_i2c_interface_t *i2c_interface, uint8_t ack)
{
	uint8_t byte = 0;
	uint8_t i;

	i2c_interface->sda_out(HIGH, i2c_interface->user_data);
	for (i = 0; i < 8; ++i)
	{
		i2c_interface->scl_out(HIGH, i2c_interface->user_data);
		byte <<= 1;
		byte |= i2c_interface->sda_in(i2c_interface->user_data);
		i2c_interface->scl_out(LOW, i2c_interface->user_data);
	}

	sw_i2c_hal_write_bit(i2c_interface, ack);
	return byte;
}
