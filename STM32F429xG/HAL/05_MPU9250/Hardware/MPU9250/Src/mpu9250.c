#include "mpu9250.h"

/**********************************************
函数名称：MPU9250_IIC_Delay
函数功能：MPU IIC延时函数，延时2us
函数参数：无
函数返回值：无
**********************************************/
#pragma GCC push_options // 防止GCC优化掉延时函数
#pragma GCC optimize("O0")
void MPU9250_IIC_Delay(void)
{
    u8 t = 15;

    while (t--)
    {
    }
    // delay_us(2);
}
#pragma GCC pop_options

/**********************************************
函数名称：MPU9250_IIC_Init
函数功能：MPU IIC初始化
函数参数：无
函数返回值：无
**********************************************/
void MPU9250_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure = {0};

    MPU9250_SCLK_Port_Clk_Enable();
    MPU9250_SDIN_Port_Clk_Enable();

    // PH4,5初始化设置
    GPIO_Initure.Pin = MPU9250_SCLK_Pin;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;   
    GPIO_Initure.Pull = GPIO_PULLUP;           // 上拉
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; // 快速
    HAL_GPIO_Init(MPU9250_SCLK_Port, &GPIO_Initure);

    GPIO_Initure.Pin = MPU9250_SDIN_Pin;
    HAL_GPIO_Init(MPU9250_SDIN_Port, &GPIO_Initure);

    MPU9250_SDIN_Set();
    MPU9250_SCLK_Set();
    delay_ms(100);
}

/**********************************************
函数名称：MPU9250_IIC_Start
函数功能：MPU IIC发送起始信号
函数参数：无
函数返回值：无
**********************************************/
void MPU9250_IIC_Start(void)
{
    MPU9250_SDIN_OUT(); // SDIN线 输出
    MPU9250_SDIN_Set();
    MPU9250_IIC_Delay();
    MPU9250_IIC_Delay();
    MPU9250_SCLK_Set();
    MPU9250_IIC_Delay();
    MPU9250_SDIN_Clr(); // START:当SCL线处于高电平时,SDIN线突然从高变低,发送起始信号
    MPU9250_IIC_Delay();
    MPU9250_SCLK_Clr(); // 钳住I2C总线，准备发送或接收数据
}

/**********************************************
函数名称：MPU9250_IIC_Stop
函数功能：MPU IIC发送停止信号
函数参数：无
函数返回值：无
**********************************************/
void MPU9250_IIC_Stop(void)
{
    MPU9250_SDIN_OUT(); // SDIN线输出
    MPU9250_SCLK_Clr();
    MPU9250_SDIN_Clr(); // STOP:当SCL线处于高电平时,SDIN线突然从低变高,发送停止信号
    MPU9250_IIC_Delay();
    MPU9250_SCLK_Set();
    MPU9250_SDIN_Set(); // 发送I2C总线结束信号
    MPU9250_IIC_Delay();
}

/**********************************************
函数名称：MPU9250_IIC_Wait_Ack
函数功能：MPU IIC等待信号到来
函数参数：无
函数返回值：1:接收应答信号成功  0:接收应答信号失败
**********************************************/
u8 MPU9250_IIC_Wait_Ack(void)
{
    u8 ucErrTime = 0;
    // MPU9250_SDIN_Set();
    MPU9250_SDIN_IN(); // SDIN设置为输入
    MPU9250_IIC_Delay();
    MPU9250_IIC_Delay();
    MPU9250_SCLK_Set();
    MPU9250_IIC_Delay();
    while (MPU9250_READ_SDIN())
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            MPU9250_IIC_Stop();
            return 1;
        }
    }
    MPU9250_IIC_Delay();
    MPU9250_SCLK_Clr(); // 时钟输出0
    return 0;
}

/**********************************************
函数名称：MPU9250_IIC_Ack
函数功能：MPU IIC产生应答信号
函数参数：无
函数返回值：无
**********************************************/
void MPU9250_IIC_Ack(void)
{
    MPU9250_SCLK_Clr();
    MPU9250_SDIN_OUT();
    MPU9250_IIC_Delay();
    MPU9250_SDIN_Clr();
    MPU9250_IIC_Delay();
    MPU9250_SCLK_Set();
    MPU9250_IIC_Delay();
    MPU9250_IIC_Delay();
    MPU9250_SCLK_Clr();
}

/**********************************************
函数名称：MPU9250_IIC_NAck
函数功能：MPU IIC不产生应答信号
函数参数：无
函数返回值：无
**********************************************/
void MPU9250_IIC_NAck(void)
{
    MPU9250_SCLK_Clr();
    MPU9250_SDIN_OUT();
    MPU9250_IIC_Delay();
    MPU9250_SDIN_Set();
    MPU9250_IIC_Delay();
    MPU9250_SCLK_Set();
    MPU9250_IIC_Delay();
    MPU9250_IIC_Delay();
    MPU9250_SCLK_Clr();
}

/**********************************************
函数名称：MPU9250_IIC_Send_Byte
函数功能：MPU IIC发送一个字节
函数参数：txd：要发送的数据
函数返回值：无
注意：IIC发送字节是一个一个位发送的，发送一个字节需要发送八次
**********************************************/
void MPU9250_IIC_Send_Byte(u8 txd)
{
    u8 t;
    MPU9250_SDIN_OUT();
    MPU9250_SCLK_Clr(); // 拉低时钟开始数据传输
    for (t = 0; t < 8; t++)
    {
        MPU9250_IIC_Delay();
        if ((txd & 0x80) >> 7)
            MPU9250_SDIN_Set();
        else
            MPU9250_SDIN_Clr();
        txd <<= 1;
        MPU9250_IIC_Delay();
        MPU9250_SCLK_Set();
        MPU9250_IIC_Delay();
        MPU9250_IIC_Delay();
        MPU9250_SCLK_Clr();
    }
}

/**********************************************
函数名称：MPU9250_IIC_Read_Byte
函数功能：MPU IIC读取一个字节
函数参数：ack: 1,发送ACK   0,发送NACK
函数返回值：接收到的数据
注意：IIC读取字节是一个一个位读取的，读取一个字节需要读取八次
**********************************************/
u8 MPU9250_IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    MPU9250_SDIN_IN(); // SDIN设置为输入
    for (i = 0; i < 8; i++)
    {
        MPU9250_SCLK_Clr();
        MPU9250_IIC_Delay();
        MPU9250_IIC_Delay();
        MPU9250_SCLK_Set();
        MPU9250_IIC_Delay();
        receive <<= 1;
        if (MPU9250_READ_SDIN())
            receive++; // 如果读到了数据
        MPU9250_IIC_Delay();
    }
    if (!ack)
        MPU9250_IIC_NAck(); // 发送nACK
    else
        MPU9250_IIC_Ack(); // 发送ACK
    return receive;
}

// 初始化MPU9250
// 返回值:0,成功
//     其他,错误代码
u8 MPU9250_Init(void)
{
    u8 res = 0;
    MPU9250_IIC_Init();                                            // 初始化IIC总线
    MPU9250_Write_Byte(MPU9250_ADDR, MPU9250_PWR_MGMT1_REG, 0X80); // 复位MPU9250
    delay_ms(100);                                                 // 延时100ms
    MPU9250_Write_Byte(MPU9250_ADDR, MPU9250_PWR_MGMT1_REG, 0X00); // 唤醒MPU9250
    MPU9250_Set_Gyro_Fsr(3);                                       // 陀螺仪传感器,±2000dps
    MPU9250_Set_Accel_Fsr(0);                                      // 加速度传感器,±2g
    MPU9250_Set_Rate(50);                                          // 设置采样率50Hz
    MPU9250_Write_Byte(MPU9250_ADDR, MPU9250_INT_EN_REG, 0X00);    // 关闭所有中断
    MPU9250_Write_Byte(MPU9250_ADDR, MPU9250_USER_CTRL_REG, 0X00); // I2C主模式关闭
    MPU9250_Write_Byte(MPU9250_ADDR, MPU9250_FIFO_EN_REG, 0X00);   // 关闭FIFO
    MPU9250_Write_Byte(MPU9250_ADDR, MPU9250_INTBP_CFG_REG, 0X82); // INT引脚低电平有效，开启bypass模式，可以直接读取磁力计
    res = MPU9250_Read_Byte(MPU9250_ADDR, MPU9250_DEVICE_ID_REG);  // 读取MPU6500的ID
    if (res == MPU6500_ID)                                         // 器件ID正确
    {
        MPU9250_Write_Byte(MPU9250_ADDR, MPU9250_PWR_MGMT1_REG, 0X01); // 设置CLKSEL,PLL X轴为参考
        MPU9250_Write_Byte(MPU9250_ADDR, MPU9250_PWR_MGMT2_REG, 0X00); // 加速度与陀螺仪都工作
        MPU9250_Set_Rate(50);                                          // 设置采样率为50Hz
    }
    else
        return 1;

    res = MPU9250_Read_Byte(AK8963_ADDR, MAG_WIA); // 读取AK8963 ID
    if (res == AK8963_ID)
    {
        MPU9250_Write_Byte(AK8963_ADDR, MAG_CNTL1, 0X11); // 设置AK8963为单次测量模式
    }
    else
        return 1;

    return 0;
}

// 设置MPU9250陀螺仪传感器满量程范围
// fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
// 返回值:0,设置成功
//     其他,设置失败
u8 MPU9250_Set_Gyro_Fsr(u8 fsr)
{
    return MPU9250_Write_Byte(MPU9250_ADDR, MPU9250_GYRO_CFG_REG, fsr << 3); // 设置陀螺仪满量程范围
}
// 设置MPU9250加速度传感器满量程范围
// fsr:0,±2g;1,±4g;2,±8g;3,±16g
// 返回值:0,设置成功
//     其他,设置失败
u8 MPU9250_Set_Accel_Fsr(u8 fsr)
{
    return MPU9250_Write_Byte(MPU9250_ADDR, MPU9250_ACCEL_CFG_REG, fsr << 3); // 设置加速度传感器满量程范围
}

// 设置MPU9250的数字低通滤波器
// lpf:数字低通滤波频率(Hz)
// 返回值:0,设置成功
//     其他,设置失败
u8 MPU9250_Set_LPF(u16 lpf)
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
    return MPU9250_Write_Byte(MPU9250_ADDR, MPU9250_CFG_REG, data); // 设置数字低通滤波器
}

// 设置MPU9250的采样率(假定Fs=1KHz)
// rate:4~1000(Hz)
// 返回值:0,设置成功
//     其他,设置失败
u8 MPU9250_Set_Rate(u16 rate)
{
    u8 data;
    if (rate > 1000)
        rate = 1000;
    if (rate < 4)
        rate = 4;
    data = 1000 / rate - 1;
    data = MPU9250_Write_Byte(MPU9250_ADDR, MPU9250_SAMPLE_RATE_REG, data); // 设置数字低通滤波器
    return MPU9250_Set_LPF(rate / 2);                                       // 自动设置LPF为采样率的一半
}

// 得到温度值
// 返回值:温度值(扩大了100倍)
short MPU9250_Get_Temperature(void)
{
    u8 buf[2];
    short raw;
    float temp;
    MPU9250_Read_Len(MPU9250_ADDR, MPU9250_TEMP_OUTH_REG, 2, buf);
    raw = ((u16)buf[0] << 8) | buf[1];
    temp = 21 + ((double)raw) / 333.87;
    return temp * 100;
    ;
}
// 得到陀螺仪值(原始值)
// gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
// 返回值:0,成功
//     其他,错误代码
u8 MPU9250_Get_Gyroscope(short *gx, short *gy, short *gz)
{
    u8 buf[6], res;
    res = MPU9250_Read_Len(MPU9250_ADDR, MPU9250_GYRO_XOUTH_REG, 6, buf);
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
u8 MPU9250_Get_Accelerometer(short *ax, short *ay, short *az)
{
    u8 buf[6], res;
    res = MPU9250_Read_Len(MPU9250_ADDR, MPU9250_ACCEL_XOUTH_REG, 6, buf);
    if (res == 0)
    {
        *ax = ((u16)buf[0] << 8) | buf[1];
        *ay = ((u16)buf[2] << 8) | buf[3];
        *az = ((u16)buf[4] << 8) | buf[5];
    }
    return res;
    ;
}

// 得到磁力计值(原始值)
// mx,my,mz:磁力计x,y,z轴的原始读数(带符号)
// 返回值:0,成功
//     其他,错误代码
u8 MPU9250_Get_Magnetometer(short *mx, short *my, short *mz)
{
    u8 buf[6], res;
    res = MPU9250_Read_Len(AK8963_ADDR, MAG_XOUT_L, 6, buf);
    if (res == 0)
    {
        *mx = ((u16)buf[1] << 8) | buf[0];
        *my = ((u16)buf[3] << 8) | buf[2];
        *mz = ((u16)buf[5] << 8) | buf[4];
    }
    MPU9250_Write_Byte(AK8963_ADDR, MAG_CNTL1, 0X11); // AK8963每次读完以后都需要重新设置为单次测量模式
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
u8 MPU9250_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
    u8 i;
    MPU9250_IIC_Start();
    MPU9250_IIC_Send_Byte((addr << 1) | 0); // 发送器件地址+写命令
    if (MPU9250_IIC_Wait_Ack())             // 等待应答
    {
        MPU9250_IIC_Stop();
        return 1;
    }
    MPU9250_IIC_Send_Byte(reg); // 写寄存器地址
    MPU9250_IIC_Wait_Ack();     // 等待应答
    for (i = 0; i < len; i++)
    {
        MPU9250_IIC_Send_Byte(buf[i]); // 发送数据
        if (MPU9250_IIC_Wait_Ack())    // 等待ACK
        {
            MPU9250_IIC_Stop();
            return 1;
        }
    }
    MPU9250_IIC_Stop();
    return 0;
}

// IIC连续读
// addr:器件地址
// reg:要读取的寄存器地址
// len:要读取的长度
// buf:读取到的数据存储区
// 返回值:0,正常
//     其他,错误代码
u8 MPU9250_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
    MPU9250_IIC_Start();
    MPU9250_IIC_Send_Byte((addr << 1) | 0); // 发送器件地址+写命令
    if (MPU9250_IIC_Wait_Ack())             // 等待应答
    {
        MPU9250_IIC_Stop();
        return 1;
    }
    MPU9250_IIC_Send_Byte(reg); // 写寄存器地址
    MPU9250_IIC_Wait_Ack();     // 等待应答
    MPU9250_IIC_Start();
    MPU9250_IIC_Send_Byte((addr << 1) | 1); // 发送器件地址+读命令
    MPU9250_IIC_Wait_Ack();                 // 等待应答
    while (len)
    {
        if (len == 1)
            *buf = MPU9250_IIC_Read_Byte(0); // 读数据,发送nACK
        else
            *buf = MPU9250_IIC_Read_Byte(1); // 读数据,发送ACK
        len--;
        buf++;
    }
    MPU9250_IIC_Stop(); // 产生一个停止条件
    return 0;
}

// IIC写一个字节
// devaddr:器件IIC地址
// reg:寄存器地址
// data:数据
// 返回值:0,正常
//     其他,错误代码
u8 MPU9250_Write_Byte(u8 addr, u8 reg, u8 data)
{
    MPU9250_IIC_Start();
    MPU9250_IIC_Send_Byte((addr << 1) | 0); // 发送器件地址+写命令
    if (MPU9250_IIC_Wait_Ack())             // 等待应答
    {
        MPU9250_IIC_Stop();
        return 1;
    }
    MPU9250_IIC_Send_Byte(reg);  // 写寄存器地址
    MPU9250_IIC_Wait_Ack();      // 等待应答
    MPU9250_IIC_Send_Byte(data); // 发送数据
    if (MPU9250_IIC_Wait_Ack())  // 等待ACK
    {
        MPU9250_IIC_Stop();
        return 1;
    }
    MPU9250_IIC_Stop();
    return 0;
}

// IIC读一个字节
// reg:寄存器地址
// 返回值:读到的数据
u8 MPU9250_Read_Byte(u8 addr, u8 reg)
{
    u8 res;
    MPU9250_IIC_Start();
    MPU9250_IIC_Send_Byte((addr << 1) | 0); // 发送器件地址+写命令
    MPU9250_IIC_Wait_Ack();                 // 等待应答
    MPU9250_IIC_Send_Byte(reg);             // 写寄存器地址
    MPU9250_IIC_Wait_Ack();                 // 等待应答
    MPU9250_IIC_Start();
    MPU9250_IIC_Send_Byte((addr << 1) | 1); // 发送器件地址+读命令
    MPU9250_IIC_Wait_Ack();                 // 等待应答
    res = MPU9250_IIC_Read_Byte(0);         // 读数据,发送nACK
    MPU9250_IIC_Stop();                     // 产生一个停止条件
    return res;
}
