#ifndef __MPU6050_H
#define __MPU6050_H
#include "sys.h"
#include "delay.h"
#include "usart.h"

#define MPU6050_SCLK_Port GPIOB
#define MPU6050_SCLK_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define MPU6050_SCLK_Pin GPIO_PIN_6

#define MPU6050_SDIN_Port GPIOB
#define MPU6050_SDIN_Port_Clk_Enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define MPU6050_SDIN_Pin GPIO_PIN_7

// IO方向设置
#define MPU_SDA_IN()                                 \
    {                                                \
        MPU6050_SDIN_Port->MODER &= ~(3 << (7 * 2)); \
        MPU6050_SDIN_Port->MODER |= 0 << 7 * 2;      \
    } // PB9输入模式
#define MPU_SDA_OUT()                                \
    {                                                \
        MPU6050_SDIN_Port->MODER &= ~(3 << (7 * 2)); \
        MPU6050_SDIN_Port->MODER |= 1 << 7 * 2;      \
    } // PB9输出模式

#define MPU6050_SCLK_Clr() MPU6050_SCLK_Port->BSRR = (uint32_t)MPU6050_SCLK_Pin << 16U
#define MPU6050_SCLK_Set() MPU6050_SCLK_Port->BSRR = MPU6050_SCLK_Pin

#define MPU6050_SDIN_Clr() MPU6050_SDIN_Port->BSRR = (uint32_t)MPU6050_SDIN_Pin << 16U
#define MPU6050_SDIN_Set() MPU6050_SDIN_Port->BSRR = MPU6050_SDIN_Pin
#define MPU6050_READ_SDIN() MPU6050_SDIN_Port->IDR &MPU6050_SDIN_Pin // 0 or 1

// #define MPU_ACCEL_OFFS_REG		0X06	//accel_offs寄存器,可读取版本号,寄存器手册未提到
// #define MPU_PROD_ID_REG			0X0C	//prod id寄存器,在寄存器手册未提到
#define MPU_SELF_TESTX_REG 0X0D   // 自检寄存器X
#define MPU_SELF_TESTY_REG 0X0E   // 自检寄存器Y
#define MPU_SELF_TESTZ_REG 0X0F   // 自检寄存器Z
#define MPU_SELF_TESTA_REG 0X10   // 自检寄存器A
#define MPU_SAMPLE_RATE_REG 0X19  // 采样频率分频器
#define MPU_CFG_REG 0X1A          // 配置寄存器
#define MPU_GYRO_CFG_REG 0X1B     // 陀螺仪配置寄存器
#define MPU_ACCEL_CFG_REG 0X1C    // 加速度计配置寄存器
#define MPU_MOTION_DET_REG 0X1F   // 运动检测阀值设置寄存器
#define MPU_FIFO_EN_REG 0X23      // FIFO使能寄存器
#define MPU_I2CMST_CTRL_REG 0X24  // IIC主机控制寄存器
#define MPU_I2CSLV0_ADDR_REG 0X25 // IIC从机0器件地址寄存器
#define MPU_I2CSLV0_REG 0X26      // IIC从机0数据地址寄存器
#define MPU_I2CSLV0_CTRL_REG 0X27 // IIC从机0控制寄存器
#define MPU_I2CSLV1_ADDR_REG 0X28 // IIC从机1器件地址寄存器
#define MPU_I2CSLV1_REG 0X29      // IIC从机1数据地址寄存器
#define MPU_I2CSLV1_CTRL_REG 0X2A // IIC从机1控制寄存器
#define MPU_I2CSLV2_ADDR_REG 0X2B // IIC从机2器件地址寄存器
#define MPU_I2CSLV2_REG 0X2C      // IIC从机2数据地址寄存器
#define MPU_I2CSLV2_CTRL_REG 0X2D // IIC从机2控制寄存器
#define MPU_I2CSLV3_ADDR_REG 0X2E // IIC从机3器件地址寄存器
#define MPU_I2CSLV3_REG 0X2F      // IIC从机3数据地址寄存器
#define MPU_I2CSLV3_CTRL_REG 0X30 // IIC从机3控制寄存器
#define MPU_I2CSLV4_ADDR_REG 0X31 // IIC从机4器件地址寄存器
#define MPU_I2CSLV4_REG 0X32      // IIC从机4数据地址寄存器
#define MPU_I2CSLV4_DO_REG 0X33   // IIC从机4写数据寄存器
#define MPU_I2CSLV4_CTRL_REG 0X34 // IIC从机4控制寄存器
#define MPU_I2CSLV4_DI_REG 0X35   // IIC从机4读数据寄存器

#define MPU_I2CMST_STA_REG 0X36 // IIC主机状态寄存器
#define MPU_INTBP_CFG_REG 0X37  // 中断/旁路设置寄存器
#define MPU_INT_EN_REG 0X38     // 中断使能寄存器
#define MPU_INT_STA_REG 0X3A    // 中断状态寄存器

#define MPU_ACCEL_XOUTH_REG 0X3B // 加速度值,X轴高8位寄存器
#define MPU_ACCEL_XOUTL_REG 0X3C // 加速度值,X轴低8位寄存器
#define MPU_ACCEL_YOUTH_REG 0X3D // 加速度值,Y轴高8位寄存器
#define MPU_ACCEL_YOUTL_REG 0X3E // 加速度值,Y轴低8位寄存器
#define MPU_ACCEL_ZOUTH_REG 0X3F // 加速度值,Z轴高8位寄存器
#define MPU_ACCEL_ZOUTL_REG 0X40 // 加速度值,Z轴低8位寄存器

#define MPU_TEMP_OUTH_REG 0X41 // 温度值高八位寄存器
#define MPU_TEMP_OUTL_REG 0X42 // 温度值低8位寄存器

#define MPU_GYRO_XOUTH_REG 0X43 // 陀螺仪值,X轴高8位寄存器
#define MPU_GYRO_XOUTL_REG 0X44 // 陀螺仪值,X轴低8位寄存器
#define MPU_GYRO_YOUTH_REG 0X45 // 陀螺仪值,Y轴高8位寄存器
#define MPU_GYRO_YOUTL_REG 0X46 // 陀螺仪值,Y轴低8位寄存器
#define MPU_GYRO_ZOUTH_REG 0X47 // 陀螺仪值,Z轴高8位寄存器
#define MPU_GYRO_ZOUTL_REG 0X48 // 陀螺仪值,Z轴低8位寄存器

#define MPU_I2CSLV0_DO_REG 0X63 // IIC从机0数据寄存器
#define MPU_I2CSLV1_DO_REG 0X64 // IIC从机1数据寄存器
#define MPU_I2CSLV2_DO_REG 0X65 // IIC从机2数据寄存器
#define MPU_I2CSLV3_DO_REG 0X66 // IIC从机3数据寄存器

#define MPU_I2CMST_DELAY_REG 0X67 // IIC主机延时管理寄存器
#define MPU_SIGPATH_RST_REG 0X68  // 信号通道复位寄存器
#define MPU_MDETECT_CTRL_REG 0X69 // 运动检测控制寄存器
#define MPU_USER_CTRL_REG 0X6A    // 用户控制寄存器
#define MPU_PWR_MGMT1_REG 0X6B    // 电源管理寄存器1
#define MPU_PWR_MGMT2_REG 0X6C    // 电源管理寄存器2
#define MPU_FIFO_CNTH_REG 0X72    // FIFO计数寄存器高八位
#define MPU_FIFO_CNTL_REG 0X73    // FIFO计数寄存器低八位
#define MPU_FIFO_RW_REG 0X74      // FIFO读写寄存器
#define MPU_DEVICE_ID_REG 0X75    // 器件ID寄存器

// 如果AD0脚(9脚)接地,IIC地址为0X68(不包含最低位).
// 如果接V3.3,则IIC地址为0X69(不包含最低位).
#define MPU_ADDR 0X68

// 因为模块AD0默认接GND,所以转为读写地址后,为0XD1和0XD0(如果接VCC,则为0XD3和0XD2)
// #define MPU_READ    				0XD1
// #define MPU_WRITE   				0XD0

// IIC所有操作函数
void MPU_IIC_Delay(void);                // IIC延时2ms函数
void MPU_IIC_Init(void);                 // 初始化IIC的IO口
void MPU_IIC_Start(void);                // 发送IIC开始信号
void MPU_IIC_Stop(void);                 // 发送IIC停止信号
void MPU_IIC_Send_Byte(u8 txd);          // IIC发送一个字节
u8 MPU_IIC_Read_Byte(unsigned char ack); // IIC读取一个字节
u8 MPU_IIC_Wait_Ack(void);               // IIC等待ACK信号
void MPU_IIC_Ack(void);                  // IIC发送ACK信号
void MPU_IIC_NAck(void);                 // IIC不发送ACK信号
void IMPU_IC_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr, u8 addr);

// MPU所有操作函数
u8 MPU_Init(void);                                  // 初始化MPU6050
u8 MPU_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf); // IIC连续写
u8 MPU_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf);  // IIC连续读
u8 MPU_Write_Byte(u8 reg, u8 data);                 // IIC写一个字节
u8 MPU_Read_Byte(u8 reg);                           // IIC读一个字节

u8 MPU_Set_Gyro_Fsr(u8 fsr);
u8 MPU_Set_Accel_Fsr(u8 fsr);
u8 MPU_Set_LPF(u16 lpf);
u8 MPU_Set_Rate(u16 rate);
u8 MPU_Set_Fifo(u8 sens);

short MPU_Get_Temperature(void);
u8 MPU_Get_Gyroscope(short *gx, short *gy, short *gz);
u8 MPU_Get_Accelerometer(short *ax, short *ay, short *az);

#endif
