#include "mpu6050.h"
#include "math.h"
#include "filter.h"

u16 Temperature; // 温度变量
short gyro[3], accel[3], sensors;
float Roll, Pitch, Yaw;                       // 欧拉角，横滚角，俯仰角，偏航角
float Angle_Balance, Gyro_Balance, Gyro_Turn; // 平衡倾角 平衡陀螺仪 转向陀螺仪
float Acceleration_Z;                         // Z轴加速度计
float Modle_Angle_Balance = -3.75;            // 模型平衡角（平衡时的角）

/**********************************************
函数名称：MPU_IIC_Delay
函数功能：MPU IIC延时函数，延时2us
函数参数：无
函数返回值：无
**********************************************/
#pragma GCC push_options // 防止GCC优化掉延时函数
#pragma GCC optimize("O0")
void MPU_IIC_Delay(void)
{
    // u8 t = 10;

    // while (t--)
    // {
    // }
    delay_us(1);
}
#pragma GCC pop_options

/**********************************************
函数名称：MPU_IIC_Init
函数功能：MPU IIC初始化
函数参数：无
函数返回值：无
**********************************************/
void MPU_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure = {0};

    MPU6050_SCLK_Port_Clk_Enable();
    MPU6050_SDIN_Port_Clk_Enable();

    // PH4,5初始化设置
    GPIO_Initure.Pin = MPU6050_SCLK_Pin;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_OD;   // MPU6050要开漏输出
    GPIO_Initure.Pull = GPIO_NOPULL;           // 上拉
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; // 快速
    HAL_GPIO_Init(MPU6050_SCLK_Port, &GPIO_Initure);

    GPIO_Initure.Pin = MPU6050_SDIN_Pin;
    HAL_GPIO_Init(MPU6050_SDIN_Port, &GPIO_Initure);

    MPU6050_SDIN_Set();
    MPU6050_SCLK_Set();
    // delay_ms(100);
}

/**********************************************
函数名称：MPU_IIC_Start
函数功能：MPU IIC发送起始信号
函数参数：无
函数返回值：无
**********************************************/
void MPU_IIC_Start(void)
{
    MPU_SDA_OUT(); // SDA线 输出
    MPU6050_SDIN_Set();
    MPU_IIC_Delay();
    MPU_IIC_Delay();
    MPU6050_SCLK_Set();
    MPU_IIC_Delay();
    MPU6050_SDIN_Clr(); // START:当SCL线处于高电平时,SDA线突然从高变低,发送起始信号
    MPU_IIC_Delay();
    MPU6050_SCLK_Clr(); // 钳住I2C总线，准备发送或接收数据
}

/**********************************************
函数名称：MPU_IIC_Stop
函数功能：MPU IIC发送停止信号
函数参数：无
函数返回值：无
**********************************************/
void MPU_IIC_Stop(void)
{
    MPU_SDA_OUT(); // SDA线输出
    MPU6050_SCLK_Clr();
    MPU6050_SDIN_Clr(); // STOP:当SCL线处于高电平时,SDA线突然从低变高,发送停止信号
    MPU_IIC_Delay();
    MPU6050_SCLK_Set();
    MPU6050_SDIN_Set(); // 发送I2C总线结束信号
    MPU_IIC_Delay();
}

/**********************************************
函数名称：MPU_IIC_Wait_Ack
函数功能：MPU IIC等待信号到来
函数参数：无
函数返回值：1:接收应答信号成功  0:接收应答信号失败
**********************************************/
u8 MPU_IIC_Wait_Ack(void)
{
    u8 ucErrTime = 0;
    // MPU6050_SDIN_Set();
    MPU_SDA_IN(); // SDA设置为输入
    MPU_IIC_Delay();
    MPU_IIC_Delay();
    MPU6050_SCLK_Set();
    MPU_IIC_Delay();
    while (MPU6050_READ_SDIN())
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            MPU_IIC_Stop();
            return 1;
        }
    }
    MPU_IIC_Delay();
    MPU6050_SCLK_Clr(); // 时钟输出0
    return 0;
}

/**********************************************
函数名称：MPU_IIC_Ack
函数功能：MPU IIC产生应答信号
函数参数：无
函数返回值：无
**********************************************/
void MPU_IIC_Ack(void)
{
    MPU6050_SCLK_Clr();
    MPU_SDA_OUT();
    MPU_IIC_Delay();
    MPU6050_SDIN_Clr();
    MPU_IIC_Delay();
    MPU6050_SCLK_Set();
    MPU_IIC_Delay();
    MPU_IIC_Delay();
    MPU6050_SCLK_Clr();
}

/**********************************************
函数名称：MPU_IIC_NAck
函数功能：MPU IIC不产生应答信号
函数参数：无
函数返回值：无
**********************************************/
void MPU_IIC_NAck(void)
{
    MPU6050_SCLK_Clr();
    MPU_SDA_OUT();
    MPU_IIC_Delay();
    MPU6050_SDIN_Set();
    MPU_IIC_Delay();
    MPU6050_SCLK_Set();
    MPU_IIC_Delay();
    MPU_IIC_Delay();
    MPU6050_SCLK_Clr();
}

/**********************************************
函数名称：MPU_IIC_Send_Byte
函数功能：MPU IIC发送一个字节
函数参数：txd：要发送的数据
函数返回值：无
注意：IIC发送字节是一个一个位发送的，发送一个字节需要发送八次
**********************************************/
void MPU_IIC_Send_Byte(u8 txd)
{
    u8 t;
    MPU_SDA_OUT();
    MPU6050_SCLK_Clr(); // 拉低时钟开始数据传输
    for (t = 0; t < 8; t++)
    {
        MPU_IIC_Delay();
        if ((txd & 0x80) >> 7)
            MPU6050_SDIN_Set();
        else
            MPU6050_SDIN_Clr();
        txd <<= 1;
        MPU_IIC_Delay();
        MPU6050_SCLK_Set();
        MPU_IIC_Delay();
        MPU_IIC_Delay();
        MPU6050_SCLK_Clr();
    }
}

/**********************************************
函数名称：MPU_IIC_Read_Byte
函数功能：MPU IIC读取一个字节
函数参数：ack: 1,发送ACK   0,发送NACK
函数返回值：接收到的数据
注意：IIC读取字节是一个一个位读取的，读取一个字节需要读取八次
**********************************************/
u8 MPU_IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    MPU_SDA_IN(); // SDA设置为输入
    for (i = 0; i < 8; i++)
    {
        MPU6050_SCLK_Clr();
        MPU_IIC_Delay();
        MPU_IIC_Delay();
        MPU6050_SCLK_Set();
        MPU_IIC_Delay();
        receive <<= 1;
        if (MPU6050_READ_SDIN())
            receive++; // 如果读到了数据
        MPU_IIC_Delay();
    }
    if (!ack)
        MPU_IIC_NAck(); // 发送nACK
    else
        MPU_IIC_Ack(); // 发送ACK
    return receive;
}

/**********************************************
函数名称：MPU_Init
函数功能：初始化MPU6050
函数参数：无
函数返回值：0,初始化成功  其他,初始化失败
**********************************************/
u8 MPU_Init(void)
{
    u8 res;
    MPU_IIC_Init();                          // 初始化IIC总线
    MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X80); // 复位MPU6050
    delay_ms(100);
    MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X00); // 唤醒MPU6050
    MPU_Set_Gyro_Fsr(3);                     // 陀螺仪传感器,±2000dps
    MPU_Set_Accel_Fsr(0);                    // 加速度传感器,±2g
    MPU_Set_Rate(50);                        // 设置采样率50Hz
    MPU_Write_Byte(MPU_INT_EN_REG, 0X00);    // 关闭所有中断
    MPU_Write_Byte(MPU_USER_CTRL_REG, 0X00); // I2C主模式关闭
    MPU_Write_Byte(MPU_FIFO_EN_REG, 0X00);   // 关闭FIFO
    MPU_Write_Byte(MPU_INTBP_CFG_REG, 0X80); // INT引脚低电平有效

    res = MPU_Read_Byte(MPU_DEVICE_ID_REG);
    printf("MPU_ADDR =%x\r\n", res);
    if (res == MPU_ADDR) // 器件ID正确,即res = MPU_ADDR = 0x68
    {
        MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X01); // 设置CLKSEL,PLL X轴为参考
        MPU_Write_Byte(MPU_PWR_MGMT2_REG, 0X00); // 加速度与陀螺仪都工作
        MPU_Set_Rate(50);                        // 设置采样率为50Hz
    }
    else
        return 1; // 地址设置错误,返回1
    return 0;     // 地址设置正确,返回0
}
/**********************************************
函数名称：MPU_Set_Gyro_Fsr
函数功能：设置MPU6050陀螺仪传感器满量程范围
函数参数：fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
函数返回值：0,设置成功  其他,设置失败
**********************************************/
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
    return MPU_Write_Byte(MPU_GYRO_CFG_REG, fsr << 3); // 设置陀螺仪满量程范围
}

/**********************************************
函数名称：MPU_Set_Accel_Fsr
函数功能：设置MPU6050加速度传感器满量程范围
函数参数：fsr:0,±2g;1,±4g;2,±8g;3,±16g
函数返回值：0,设置成功  其他,设置失败
**********************************************/
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
    return MPU_Write_Byte(MPU_ACCEL_CFG_REG, fsr << 3); // 设置加速度传感器满量程范围
}

/**********************************************
函数名称：MPU_Set_LPF
函数功能：设置MPU6050的数字低通滤波器
函数参数：lpf:数字低通滤波频率(Hz)
函数返回值：0,设置成功  其他,设置失败
**********************************************/
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

/**********************************************
函数名称：MPU_Set_Rate
函数功能：设置MPU6050的采样率(假定Fs=1KHz)
函数参数：rate:4~1000(Hz)  初始化中rate取50
函数返回值：0,设置成功  其他,设置失败
**********************************************/
u8 MPU_Set_Rate(u16 rate)
{
    u8 data;
    if (rate > 1000)
        rate = 1000;
    if (rate < 4)
        rate = 4;
    data = 1000 / rate - 1;
    data = MPU_Write_Byte(MPU_SAMPLE_RATE_REG, data); // 设置数字低通滤波器
    return MPU_Set_LPF(rate / 2);                     // 自动设置LPF为采样率的一半
}

/**********************************************
函数名称：MPU_Get_Temperature
函数功能：得到温度传感器值
函数参数：无
函数返回值：温度值(扩大了100倍)
**********************************************/
short MPU_Get_Temperature(void)
{
    u8 buf[2];
    short raw;
    float temp;

    MPU_Read_Len(MPU_ADDR, MPU_TEMP_OUTH_REG, 2, buf);
    raw = ((u16)buf[0] << 8) | buf[1];
    temp = 36.53 + ((double)raw) / 340;
    return temp * 100;
}

/**********************************************
函数名称：MPU_Get_Gyroscope
函数功能：得到陀螺仪值(原始值)
函数参数：gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
函数返回值：0,读取成功  其他,读取失败
**********************************************/
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
}

/**********************************************
函数名称：MPU_Get_Accelerometer
函数功能：得到加速度值(原始值)
函数参数：ax,ay,az:加速度传感器x,y,z轴的原始读数(带符号)
函数返回值：0,读取成功  其他,读取失败
**********************************************/
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
}

/**********************************************
函数名称：MPU_Write_Len
函数功能：IIC连续写(写器件地址、寄存器地址、数据)
函数参数：addr:器件地址      reg:寄存器地址
                 len:写入数据的长度  buf:数据区
函数返回值：0,写入成功  其他,写入失败
**********************************************/
u8 MPU_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
    u8 i;

    MPU_IIC_Start();
    MPU_IIC_Send_Byte((addr << 1) | 0); // 发送器件地址+写命令(0为写,1为读)
    if (MPU_IIC_Wait_Ack())             // 等待应答
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Send_Byte(reg); // 写寄存器地址
    MPU_IIC_Wait_Ack();     // 等待应答
    for (i = 0; i < len; i++)
    {
        MPU_IIC_Send_Byte(buf[i]); // 发送数据
        if (MPU_IIC_Wait_Ack())    // 等待ACK
        {
            MPU_IIC_Stop();
            return 1;
        }
    }
    MPU_IIC_Stop();
    return 0;
}

/**********************************************
函数名称：MPU_Read_Len
函数功能：IIC连续读(写入器件地址后,读寄存器地址、数据)
函数参数：addr:器件地址        reg:要读的寄存器地址     len:要读取的数据长度  buf:读取到的数据存储区
函数返回值：0,读取成功  其他,读取失败
**********************************************/
u8 MPU_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((addr << 1) | 0); // 发送器件地址+写命令
    if (MPU_IIC_Wait_Ack())             // 等待应答
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Send_Byte(reg); // 写寄存器地址
    MPU_IIC_Wait_Ack();     // 等待应答
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((addr << 1) | 1); // 发送器件地址+读命令
    MPU_IIC_Wait_Ack();                 // 等待应答
    while (len)
    {
        if (len == 1)
            *buf = MPU_IIC_Read_Byte(0); // 读数据,发送nACK
        else
            *buf = MPU_IIC_Read_Byte(1); // 读数据,发送ACK
        len--;
        buf++;
    }
    MPU_IIC_Stop(); // 产生一个停止条件
    return 0;
}

/**********************************************
函数名称：MPU_Write_Byte
函数功能：IIC写一个字节
函数参数：data:写入的数据    reg:要写的寄存器地址
函数返回值：0,写入成功  其他,写入失败
**********************************************/
u8 MPU_Write_Byte(u8 reg, u8 data)
{
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((MPU_ADDR << 1) | 0); // 发送器件地址+写命令
    if (MPU_IIC_Wait_Ack())                 // 等待应答
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Send_Byte(reg); // 写寄存器地址
    if (MPU_IIC_Wait_Ack()) // 等待ACK
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Send_Byte(data); // 发送数据
    if (MPU_IIC_Wait_Ack())  // 等待ACK
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Stop();
    return 0;
}

/**********************************************
函数名称：MPU_Read_Byte
函数功能：IIC读一个字节
函数参数：reg:要读的寄存器地址
函数返回值：res:读取到的数据
**********************************************/
u8 MPU_Read_Byte(u8 reg)
{
    u8 res;

    MPU_IIC_Start();
    MPU_IIC_Send_Byte((MPU_ADDR << 1) | 0); // 发送器件地址+写命令
    MPU_IIC_Wait_Ack();                     // 等待应答
    MPU_IIC_Send_Byte(reg);                 // 写寄存器地址
    MPU_IIC_Wait_Ack();                     // 等待应答
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((MPU_ADDR << 1) | 1); // 发送器件地址+读命令
    MPU_IIC_Wait_Ack();                     // 等待应答
    res = MPU_IIC_Read_Byte(0);             // 读取数据,发送nACK
    MPU_IIC_Stop();                         // 产生一个停止条件
    return res;
}

/**************************************************************************
Function: Get angle
Input   : way：The algorithm of getting angle 1：DMP  2：kalman  3：Complementary filtering
Output  : none
函数功能：获取角度
入口参数：way：获取角度的算法 1：DMP  2：卡尔曼 3：互补滤波.不用dma的画需要周期性执行，看filter.h
返回  值：无
**************************************************************************/
void Get_Angle(u8 way)
{
    float Accel_Y, Accel_Z, Accel_X, Accel_Angle_x, Accel_Angle_y, Gyro_X, Gyro_Z, Gyro_Y;
    Temperature = MPU_Get_Temperature(); // 读取MPU6050内置温度传感器数据，近似表示主板温度,乘了10。
    if (way == 1)                        // DMP的读取在数据采集中断读取，严格遵循时序要求
    {
        mpu_dmp_get_data(&Pitch, &Roll, &Yaw); // 读取加速度、角速度、倾角
        Angle_Balance = Roll;                  // 更新平衡倾角,前倾为正，后倾为负
        Gyro_Balance = gyro[0];                // 更新平衡角速度,前倾为正，后倾为负
        Gyro_Turn = gyro[2];                   // 更新转向角速度
        Acceleration_Z = accel[2];             // 更新Z轴加速度计
    }
    else
    {
        Gyro_X = (MPU_Read_Byte(MPU_GYRO_XOUTH_REG) << 8) + MPU_Read_Byte(MPU_GYRO_XOUTL_REG);    // 读取X轴陀螺仪
        Gyro_Y = (MPU_Read_Byte(MPU_GYRO_YOUTH_REG) << 8) + MPU_Read_Byte(MPU_GYRO_YOUTL_REG);    // 读取Y轴陀螺仪
        Gyro_Z = (MPU_Read_Byte(MPU_GYRO_ZOUTH_REG) << 8) + MPU_Read_Byte(MPU_GYRO_ZOUTL_REG);    // 读取Z轴陀螺仪
        Accel_X = (MPU_Read_Byte(MPU_ACCEL_XOUTH_REG) << 8) + MPU_Read_Byte(MPU_ACCEL_XOUTL_REG); // 读取X轴加速度计
        Accel_Y = (MPU_Read_Byte(MPU_ACCEL_YOUTH_REG) << 8) + MPU_Read_Byte(MPU_ACCEL_YOUTL_REG); // 读取X轴加速度计
        Accel_Z = (MPU_Read_Byte(MPU_ACCEL_ZOUTH_REG) << 8) + MPU_Read_Byte(MPU_ACCEL_ZOUTL_REG); // 读取Z轴加速度计
        if (Gyro_X > 32768)
            Gyro_X -= 65536; // 数据类型转换  也可通过short强制类型转换
        if (Gyro_Y > 32768)
            Gyro_Y -= 65536; // 数据类型转换  也可通过short强制类型转换
        if (Gyro_Z > 32768)
            Gyro_Z -= 65536; // 数据类型转换
        if (Accel_X > 32768)
            Accel_X -= 65536; // 数据类型转换
        if (Accel_Y > 32768)
            Accel_Y -= 65536; // 数据类型转换
        if (Accel_Z > 32768)
            Accel_Z -= 65536;                               // 数据类型转换
        Gyro_Balance = -Gyro_X;                             // 更新平衡角速度
        Accel_Angle_x = atan2(Accel_Y, Accel_Z) * 180 / PI; // 计算倾角，转换单位为度
        Accel_Angle_y = atan2(Accel_X, Accel_Z) * 180 / PI; // 计算倾角，转换单位为度
        Gyro_X = Gyro_X / 16.4;                             // 陀螺仪量程转换，量程±2000°/s对应灵敏度16.4，可查手册
        Gyro_Y = Gyro_Y / 16.4;                             // 陀螺仪量程转换
        if (way == 2)
        {
            Pitch = -Kalman_Filter_x(Accel_Angle_x, Gyro_X); // 卡尔曼滤波
            Roll = -Kalman_Filter_y(Accel_Angle_y, Gyro_Y);
        }
        else if (way == 3)
        {
            Pitch = -Complementary_Filter_x(Accel_Angle_x, Gyro_X); // 互补滤波
            Roll = -Complementary_Filter_y(Accel_Angle_y, Gyro_Y);
        }
        Angle_Balance = Pitch;    // 更新平衡倾角
        Gyro_Turn = Gyro_Z;       // 更新转向角速度
        Acceleration_Z = Accel_Z; // 更新Z轴加速度计
    }
}