#include "mpu6050.h"

// 初始化MPU6050
// 返回值:0,成功
//     其他,错误代码2
u8 MPU6050_Init(void)
{
	u8 res;
	MPU6050Drv_Init();						 // 初始化IIC总线
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X80); // 复位MPU6050
	delay_ms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X00); // 唤醒MPU6050
	MPU_Set_Gyro_Fsr(3);					 // 陀螺仪传感器,±2000dps
	MPU_Set_Accel_Fsr(0);					 // 加速度传感器,±2g
	MPU_Set_Rate(50);						 // 设置采样率50Hz
	MPU_Write_Byte(MPU_INT_EN_REG, 0X00);	 // 关闭所有中断
	MPU_Write_Byte(MPU_USER_CTRL_REG, 0X00); // I2C主模式关闭
	MPU_Write_Byte(MPU_FIFO_EN_REG, 0X00);	 // 关闭FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG, 0X80); // INT引脚低电平有效
	res = MPU_Read_Byte(MPU_DEVICE_ID_REG);
	if (res == MPU_ADDR) // 器件ID正确
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X01); // 设置CLKSEL,PLL X轴为参考
		MPU_Write_Byte(MPU_PWR_MGMT2_REG, 0X00); // 加速度与陀螺仪都工作
		MPU_Set_Rate(50);						 // 设置采样率为50Hz
	}
	else
		return 1;
	return 0;
}
// 设置MPU6050陀螺仪传感器满量程范围
// fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
// 返回值:0,设置成功
//     其他,设置失败
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG, fsr << 3); // 设置陀螺仪满量程范围
}
// 设置MPU6050加速度传感器满量程范围
// fsr:0,±2g;1,±4g;2,±8g;3,±16g
// 返回值:0,设置成功
//     其他,设置失败
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG, fsr << 3); // 设置加速度传感器满量程范围
}
// 设置MPU6050的数字低通滤波器
// lpf:数字低通滤波频率(Hz)
// 返回值:0,设置成功
//     其他,设置失败
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data = 0;
	if (lpf >= 188)
		data = 1;
	else if (lpf >= 98)
		data = 2;
	else if (lpf >= 42)
		data = 3;
	else if (lpf >= 20)
		data = 4;
	else if (lpf >= 10)
		data = 5;
	else
		data = 6;
	return MPU_Write_Byte(MPU_CFG_REG, data); // 设置数字低通滤波器
}
// 设置MPU6050的采样率(假定Fs=1KHz)
// rate:4~1000(Hz)
// 返回值:0,设置成功
//     其他,设置失败
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if (rate > 1000)
		rate = 1000;
	if (rate < 4)
		rate = 4;
	data = 1000 / rate - 1;
	data = MPU_Write_Byte(MPU_SAMPLE_RATE_REG, data); // 设置数字低通滤波器
	return MPU_Set_LPF(rate / 2);					  // 自动设置LPF为采样率的一半
}

// 得到温度值
// 返回值:温度值(扩大了100倍)
short MPU_Get_Temperature(void)
{
	u8 buf[2];
	short raw;
	float temp;
	MPU_Read_Len(MPU_ADDR, MPU_TEMP_OUTH_REG, 2, buf);
	raw = ((u16)buf[0] << 8) | buf[1];
	temp = 36.53 + ((double)raw) / 340;
	return temp * 100;
	;
}
// 得到陀螺仪值(原始值)
// gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
// 返回值:0,成功
//     其他,错误代码
u8 MPU_Get_Gyroscope(short *gx, short *gy, short *gz)
{
	u8 buf[6], res;
	res = MPU_Read_Len(MPU_ADDR, MPU_GYRO_XOUTH_REG, 6, buf);
	if (res == 0)
	{
		*gx = ((u16)buf[0] << 8) | buf[1];
		*gy = ((u16)buf[2] << 8) | buf[3];
		*gz = ((u16)buf[4] << 8) | buf[5];
	}
	return res;
	;
}
// 得到加速度值(原始值)
// gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
// 返回值:0,成功
//     其他,错误代码
u8 MPU_Get_Accelerometer(short *ax, short *ay, short *az)
{
	u8 buf[6], res;
	res = MPU_Read_Len(MPU_ADDR, MPU_ACCEL_XOUTH_REG, 6, buf);
	if (res == 0)
	{
		*ax = ((u16)buf[0] << 8) | buf[1];
		*ay = ((u16)buf[2] << 8) | buf[3];
		*az = ((u16)buf[4] << 8) | buf[5];
	}
	return res;
	;
}
// IIC连续写
// addr:器件地址
// reg:寄存器地址
// len:写入长度
// buf:数据区
// 返回值:0,正常
//     其他,错误代码
u8 MPU_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
	u8 i;
	MPU6050Drv_IICStart();
	MPU6050Drv_IICSendByte((addr << 1) | 0); // 发送器件地址+写命令
	if (MPU6050Drv_IICWaitAck())			 // 等待应答
	{
		MPU6050Drv_IICStop();
		return 1;
	}
	MPU6050Drv_IICSendByte(reg); // 写寄存器地址
	MPU6050Drv_IICWaitAck();	 // 等待应答
	for (i = 0; i < len; i++)
	{
		MPU6050Drv_IICSendByte(buf[i]); // 发送数据
		if (MPU6050Drv_IICWaitAck())	// 等待ACK
		{
			MPU6050Drv_IICStop();
			return 1;
		}
	}
	MPU6050Drv_IICStop();
	return 0;
}
// IIC连续读
// addr:器件地址
// reg:要读取的寄存器地址
// len:要读取的长度
// buf:读取到的数据存储区
// 返回值:0,正常
//     其他,错误代码
u8 MPU_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
	MPU6050Drv_IICStart();
	MPU6050Drv_IICSendByte((addr << 1) | 0); // 发送器件地址+写命令
	if (MPU6050Drv_IICWaitAck())			 // 等待应答
	{
		MPU6050Drv_IICStop();
		return 1;
	}
	MPU6050Drv_IICSendByte(reg); // 写寄存器地址
	MPU6050Drv_IICWaitAck();	 // 等待应答
	MPU6050Drv_IICStart();
	MPU6050Drv_IICSendByte((addr << 1) | 1); // 发送器件地址+读命令
	MPU6050Drv_IICWaitAck();				 // 等待应答
	while (len)
	{
		if (len == 1)
			*buf = MPU6050Drv_IICReadByte(0); // 读数据,发送nACK
		else
			*buf = MPU6050Drv_IICReadByte(1); // 读数据,发送ACK
		len--;
		buf++;
	}
	MPU6050Drv_IICStop(); // 产生一个停止条件
	return 0;
}
// IIC写一个字节
// reg:寄存器地址
// data:数据
// 返回值:0,正常
//     其他,错误代码
u8 MPU_Write_Byte(u8 reg, u8 data)
{
	MPU6050Drv_IICStart();
	MPU6050Drv_IICSendByte((MPU_ADDR << 1) | 0); // 发送器件地址+写命令
	if (MPU6050Drv_IICWaitAck())				 // 等待应答
	{
		MPU6050Drv_IICStop();
		return 1;
	}
	MPU6050Drv_IICSendByte(reg);  // 写寄存器地址
	MPU6050Drv_IICWaitAck();	  // 等待应答
	MPU6050Drv_IICSendByte(data); // 发送数据
	if (MPU6050Drv_IICWaitAck())  // 等待ACK
	{
		MPU6050Drv_IICStop();
		return 1;
	}
	MPU6050Drv_IICStop();
	return 0;
}
// IIC读一个字节
// reg:寄存器地址
// 返回值:读到的数据
u8 MPU_Read_Byte(u8 reg)
{
	u8 res;
	MPU6050Drv_IICStart();
	MPU6050Drv_IICSendByte((MPU_ADDR << 1) | 0); // 发送器件地址+写命令
	MPU6050Drv_IICWaitAck();					 // 等待应答
	MPU6050Drv_IICSendByte(reg);				 // 写寄存器地址
	MPU6050Drv_IICWaitAck();					 // 等待应答
	MPU6050Drv_IICStart();
	MPU6050Drv_IICSendByte((MPU_ADDR << 1) | 1); // 发送器件地址+读命令
	MPU6050Drv_IICWaitAck();					 // 等待应答
	res = MPU6050Drv_IICReadByte(0);			 // 读取数据,发送nACK
	MPU6050Drv_IICStop();						 // 产生一个停止条件
	return res;
}

/* -------------------------------------------------------------------- */

/**
 * @brief      硬件底层初始化.
 * @retval     None.
 */
void MPU6050Drv_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};

	/* GPIO时钟开启 */
	MPU6050_RST_Port_Clk_Enable();
	MPU6050_SCLK_Port_Clk_Enable();
	MPU6050_SDIN_Port_Clk_Enable();

	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pin = MPU6050_RST_Pin;
	HAL_GPIO_Init(MPU6050_RST_Port, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = MPU6050_SCLK_Pin;
	HAL_GPIO_Init(MPU6050_SCLK_Port, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = MPU6050_SDIN_Pin;
	HAL_GPIO_Init(MPU6050_SDIN_Port, &GPIO_InitStructure);

	HAL_GPIO_WritePin(MPU6050_SCLK_Port, MPU6050_SCLK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MPU6050_SDIN_Port, MPU6050_SDIN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MPU6050_RST_Port, MPU6050_RST_Pin, GPIO_PIN_RESET);
	delay_ms(200);
	HAL_GPIO_WritePin(MPU6050_RST_Port, MPU6050_RST_Pin, GPIO_PIN_SET);
}

/**
 * @brief      IIC延时.
 * @retval     None.
 */
#pragma GCC push_options // 防止GCC优化掉延时函数
#pragma GCC optimize("O0")
void MPU6050Drv_IICDelay(void)
{
	u8 t = MPU6050Drv_IICDelay_Time;

	while (t--)
	{
	}
}
#pragma GCC pop_options

/**
 * @brief      IIC 通信开始.
 * @retval     None.
 */
void MPU6050Drv_IICStart(void)
{
	MPU6050_SDIN_Set();
	MPU6050_SCLK_Set();
	MPU6050Drv_IICDelay();
	MPU6050_SDIN_Clr();
	MPU6050Drv_IICDelay();
	MPU6050_SCLK_Clr();
	MPU6050Drv_IICDelay();
}

/**
 * @brief      IIC 通信停止.
 * @retval     None.
 */
void MPU6050Drv_IICStop(void)
{
	MPU6050_SDIN_Clr();
	MPU6050_SCLK_Clr();
	MPU6050Drv_IICDelay();
	MPU6050_SDIN_Set();
}

/**
 * @brief      IIC 等待信号响应.
 * @retval     1，接收应答失败;0，接收应答成功
 */
u8 MPU6050Drv_IICWaitAck(void)
{
	u16 ucErrTime = 0;
	MPU6050_SDIN_IN(); // SDIN设置为输入
	MPU6050_SDIN_Set();
	MPU6050Drv_IICDelay();
	MPU6050_SCLK_Set();
	while (MPU6050_READ_SDIN()) // 等待应答信号
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			MPU6050Drv_IICStop();
			printf("IIC_WaitAck_Error\r\n");
			return 1;
		}
	}
	MPU6050_SDIN_OUT(); // SDIN设置为输出
	MPU6050_SCLK_Clr();
	MPU6050Drv_IICDelay();
	return 0;
}

/**
 * @brief      IIC 写以一个字节.
 * @param[in]  data  字节数据.
 * @retval     None.
 */
void MPU6050Drv_IICSendByte(uint8_t data)
{
	u8 i;

	for (i = 0; i < 8; i++)
	{
		if (data & 0x80)
		{
			MPU6050_SDIN_Set();
		}
		else
		{
			MPU6050_SDIN_Clr();
		}
		MPU6050Drv_IICDelay();
		MPU6050_SCLK_Set();
		MPU6050Drv_IICDelay();
		MPU6050_SCLK_Clr();
		data <<= 1;
	}
}

/**
 * @brief   IIC 读1个字节
 * @param  	ack  字节数据.
 * @retval	ack=1时，发送ACK，ack=0，发送nACK
 */
u8 MPU6050Drv_IICReadByte(unsigned char ack)
{
	unsigned char i, receive = 0;
	MPU6050_SDIN_IN(); // SDIN设置为输入
	for (i = 0; i < 8; i++)
	{
		MPU6050_SCLK_Set();
		MPU6050Drv_IICDelay();
		receive <<= 1;
		if (MPU6050_READ_SDIN())
			receive++;
		MPU6050Drv_IICDelay();
		MPU6050_SCLK_Clr();
		MPU6050Drv_IICDelay();
	}
	MPU6050_SDIN_OUT(); // SDIN设置为输出
	if (!ack)
		MPU6050Drv_IIC_NAck(); // 发送nACK
	else
		MPU6050Drv_IIC_Ack(); // 发送ACK

	printf("IIC_Receive:%x\r\n", receive);

	return receive;
}

/**
 * @brief   产生ACK应答
 * @param  	NONE
 * @retval	NONE
 */
void MPU6050Drv_IIC_Ack(void)
{
	MPU6050_SCLK_Clr();
	MPU6050_SDIN_Clr();
	MPU6050Drv_IICDelay();
	MPU6050_SCLK_Set();
	MPU6050Drv_IICDelay();
	MPU6050_SCLK_Clr();
}

/**
 * @brief   不产生ACK应答
 * @param  	NONE
 * @retval	NONE
 */
void MPU6050Drv_IIC_NAck(void)
{
	MPU6050_SCLK_Clr();
	MPU6050_SDIN_Set();
	MPU6050Drv_IICDelay();
	MPU6050_SCLK_Set();
	MPU6050Drv_IICDelay();
	MPU6050_SCLK_Clr();
}