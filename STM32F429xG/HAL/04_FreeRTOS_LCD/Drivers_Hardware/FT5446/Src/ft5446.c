#include "ft5446.h"
#include "delay.h"
#include "lcd.h"
#include "myiic.h"
#include "stdio.h"

_m_tp_dev tp_dev;

const uint16_t FT5xxx_TPX_TBL[5] = {FT_TP1_REG, FT_TP2_REG, FT_TP3_REG,
                                    FT_TP4_REG, FT_TP5_REG};
uint8_t g_gt_tnum = 5; // 默认支持的触摸屏点数(5点触摸)

#define ACTIVE_WIDTH  800
#define ACTIVE_HEIGHT 480

// 电容触摸芯片IIC接口初始化
static void CT_IIC_Init(void)
{
    Drv_Init(FT5xxx_BIRCH_IIC_BUS_POINTER);
}

// 产生IIC起始信号
static void CT_IIC_Start(void)
{
    Drv_IICStart(FT5xxx_BIRCH_IIC_BUS_POINTER);
}

// 产生IIC停止信号
static void CT_IIC_Stop(void)
{
    Drv_IICStop(FT5xxx_BIRCH_IIC_BUS_POINTER);
}

// 等待应答信号到来
// 返回值：1，接收应答失败
//         0，接收应答成功
static uint8_t CT_IIC_Wait_Ack(void)
{
    return Drv_IICWaitAck(FT5xxx_BIRCH_IIC_BUS_POINTER);
}

// 产生ACK应答
static void CT_IIC_Ack(void)
{
    Drv_IICAck(FT5xxx_BIRCH_IIC_BUS_POINTER);
}

// 不产生ACK应答
static void CT_IIC_NAck(void)
{
    Drv_IICNAck(FT5xxx_BIRCH_IIC_BUS_POINTER);
}

// IIC发送一个字节
// 返回从机有无应答
// 1，有应答
// 0，无应答
static void CT_IIC_Send_Byte(uint8_t txd)
{
    Drv_IICSendByte(txd, FT5xxx_BIRCH_IIC_BUS_POINTER);
}

// 读1个字节，ack=1时，发送ACK，ack=0，发送nACK
static uint8_t CT_IIC_Read_Byte(unsigned char ack)
{
    return Drv_IICReadByte(ack, FT5xxx_BIRCH_IIC_BUS_POINTER);
}

// 向FT5xxx写入一次数据
uint8_t FT5xxx_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    uint8_t ret = 0;
    CT_IIC_Start();
    CT_IIC_Send_Byte(FT_CMD_WR); // 发送写命令
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg & 0XFF); // 发送低8位地址
    CT_IIC_Wait_Ack();
    for (i = 0; i < len; i++)
    {
        CT_IIC_Send_Byte(buf[i]); // 发数据
        ret = CT_IIC_Wait_Ack();
        if (ret)
            break;
    }
    CT_IIC_Stop(); // 产生一个停止条件
    return ret;
}

// 从FT5xxx读出一次数据
void FT5xxx_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    CT_IIC_Start();
    CT_IIC_Send_Byte(FT_CMD_WR); // 发送写命令
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg & 0XFF); // 发送低8位地址
    CT_IIC_Wait_Ack();
    CT_IIC_Start();
    CT_IIC_Send_Byte(FT_CMD_RD); // 发送读命令
    CT_IIC_Wait_Ack();
    for (i = 0; i < len; i++)
    {
        buf[i] = CT_IIC_Read_Byte(i == (len - 1) ? 0 : 1); // 发数据
    }
    CT_IIC_Stop(); // 产生一个停止条件
}

/**
 * @brief	初始化FT5xxx触摸屏
 * @param 	无
 * @arg
 * @note
 * @retval	0; 初始化成功
 * @retval	1; 初始化失败
 */
uint8_t FT5xxx_Init_Soft(void)
{
    uint8_t temp[2];

    CT_IIC_Init();

    /*初始化INT,RST低电平*/
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    INT_GPIO_CLK_ENABLE();
    RST_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin = INT_PIN;
    HAL_GPIO_Init(INT_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin = RST_PIN;
    HAL_GPIO_Init(RST_PORT, &GPIO_InitStruct);
    RST_OFF; // 复位
    HAL_Delay(20);
    RST_ON; // 释放复位
    HAL_Delay(50);

    temp[0] = 0;
    FT5xxx_WR_Reg(FT_DEVIDE_MODE, temp, 1); /* 进入正常操作模式 */
    temp[0] = 1;
    FT5xxx_WR_Reg(FT_ID_G_MODE, temp, 1);    /* 查询模式 0启用对主机中断,1禁止对主机中断*/
    temp[0] = 22;                            /* 触摸有效值，22，越小越灵敏 */
    FT5xxx_WR_Reg(FT_ID_G_THGROUP, temp, 1); /* 设置触摸有效值 */
    temp[0] = 12;                            /* 激活周期，不能小于12，最大14 */
    FT5xxx_WR_Reg(FT_ID_G_PERIODACTIVE, temp, 1);

    FT5xxx_RD_Reg(FT_ID_G_LIB_VERSION, &temp[0], 2);
    if ((temp[0] == 0X30 && temp[1] == 0X03) || temp[1] == 0X01 || temp[1] == 0X02 || (temp[0] == 0x0 && temp[1] == 0X0)) /* 版本:0X3003/0X0001/0X0002/CST340 */
    {
        char str[16];
        sprintf(str, "CTP ID:%x\r\n", ((uint16_t)temp[0] << 8) + temp[1]);
        LTDC_Show_String_sprintf(400, 240, 400, 24, 12, (u8 *)str, 0, GUI_Black);
        // printf("CTP ID:%x\r\n", ((uint16_t)temp[0] << 8) + temp[1]);
        return 0;
    }
    else
    {
        return 1;
    }
}

uint8_t FT5xxx_Init_Hard(void)
{
    uint8_t temp[2];

    I2C1_Init();

    /*初始化INT,RST低电平*/
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    INT_GPIO_CLK_ENABLE();
    RST_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin = INT_PIN;
    HAL_GPIO_Init(INT_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin = RST_PIN;
    HAL_GPIO_Init(RST_PORT, &GPIO_InitStruct);
    RST_OFF; // 复位
    HAL_Delay(20);
    RST_ON; // 释放复位
    HAL_Delay(50);

    temp[0] = 0;
    HAL_I2C_Mem_Write(&hi2c1, FT_ADDRESS << 1U, FT_DEVIDE_MODE, I2C_MEMADD_SIZE_8BIT, temp, 1, HAL_MAX_DELAY); /* 进入正常操作模式 */
    // while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    //     ;

    temp[0] = 1;
    HAL_I2C_Mem_Write(&hi2c1, FT_ADDRESS << 1U, FT_ID_G_MODE, I2C_MEMADD_SIZE_8BIT, temp, 1, HAL_MAX_DELAY); /* 查询模式 0启用对主机中断,1禁止对主机中断*/
    // while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    //     ;

    temp[0] = 22;
    HAL_I2C_Mem_Write(&hi2c1, FT_ADDRESS << 1U, FT_ID_G_THGROUP, I2C_MEMADD_SIZE_8BIT, temp, 1, HAL_MAX_DELAY); /* 触摸有效值，22，越小越灵敏 */
    // while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    //     ;

    temp[0] = 12;
    HAL_I2C_Mem_Write(&hi2c1, FT_ADDRESS << 1U, FT_ID_G_PERIODACTIVE, I2C_MEMADD_SIZE_8BIT, temp, 1, HAL_MAX_DELAY); /* 激活周期，不能小于12，最大14 */
    // while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    //     ;

    HAL_I2C_Mem_Read(&hi2c1, FT_ADDRESS << 1U, FT_ID_G_LIB_VERSION, 1, temp, 2, HAL_MAX_DELAY);
    uint16_t version = (temp[0] << 8) | temp[1];
    if (version == 0x3003 || version == 0x0001 || version == 0x0002 || version == 0x0000)
    {
        char str[16];
        sprintf(str, "CTP ID:%x\r\n", ((uint16_t)temp[0] << 8) + temp[1]);
        LTDC_Show_String_sprintf(400, 240, 400, 24, 12, (u8 *)str, 0, GUI_Black);
        // printf("CTP ID:%x\r\n", ((uint16_t)temp[0] << 8) + temp[1]);
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
 * @brief	FT5xxx周期运行,运行间隔ms
 * @param 	mode 0,正常扫描.
 * @arg
 * @note
 * @retval	0,触屏无触摸
 * @retval	1,触屏有触摸
 */
uint8_t FT5xxx_Scan_tick(uint8_t mode)
{
    // FT5xxx_RD_Reg(FT_REG_NUM_FINGER, &mode, 1); // 读取触摸点的状态
}

// 扫描触摸屏(采用查询方式)
// mode:0,正常扫描.
// 返回值:当前触屏状态.
// 0,触屏无触摸;1,触屏有触摸
uint8_t FT5xxx_Scan(uint8_t mode)
{
    uint8_t buf[4];
    uint8_t i = 0;
    uint8_t res = 0;
    uint8_t temp;
    uint16_t tempsta;
    static uint8_t t = 0; // 控制查询间隔,从而降低CPU占用率
    t++;
    if ((t % 10) == 0 ||
        t < 10) // 空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率
    {
        FT5xxx_RD_Reg(FT_REG_NUM_FINGER, &mode, 1); // 读取触摸点的状态
        if ((mode & 0XF) && ((mode & 0XF) <= g_gt_tnum))
        {
            temp = 0XFF << (mode & 0XF); // 将点的个数转换为1的位数,匹配tp_dev.sta定义
            tempsta = tp_dev.sta;        // 保存当前的tp_dev.sta值
            tp_dev.sta = (~temp) | TP_PRES_DOWN | TP_CATH_PRES;
            tp_dev.x[g_gt_tnum - 1] = tp_dev.x[0]; // 保存触点0的数据,保存在最后一个上
            tp_dev.y[g_gt_tnum - 1] = tp_dev.y[0];
            // delay_1ms(4);    //必要的延时，否则老是认为有按键按下
            for (i = 0; i < g_gt_tnum; i++)
            {
                if (tp_dev.sta & (1 << i)) // 触摸有效?
                {
                    FT5xxx_RD_Reg(FT5xxx_TPX_TBL[i], buf, 4); // 读取XY坐标值
                    if (tp_dev.touchtype & 0X01)              // 横屏
                    {
                        tp_dev.y[i] = ((uint16_t)(buf[0] & 0X0F) << 8) + buf[1];
                        tp_dev.x[i] = ((uint16_t)(buf[2] & 0X0F) << 8) + buf[3];
                    }
                    else
                    {
                        tp_dev.x[i] = (((uint16_t)(buf[0] & 0X0F) << 8) + buf[1]);
                        tp_dev.y[i] = ((uint16_t)(buf[2] & 0X0F) << 8) + buf[3];
                    }
                }
            }
            res = 1;
            if (tp_dev.x[0] > ACTIVE_WIDTH ||
                tp_dev.y[0] > ACTIVE_HEIGHT) // 非法数据(坐标超出了)
            {
                if ((mode & 0XF) >
                    1) // 有其他点有数据,则复第二个触点的数据到第一个触点.
                {
                    tp_dev.x[0] = tp_dev.x[1];
                    tp_dev.y[0] = tp_dev.y[1];
                    t = 0; // 触发一次,则会最少连续监测10次,从而提高命中率
                }
                else // 非法数据,则忽略此次数据(还原原来的)
                {
                    tp_dev.x[0] = tp_dev.x[g_gt_tnum - 1];
                    tp_dev.y[0] = tp_dev.y[g_gt_tnum - 1];
                    mode = 0X80;
                    tp_dev.sta = tempsta; // 恢复tp_dev.sta
                }
            }
            else
                t = 0; // 触发一次,则会最少连续监测10次,从而提高命中率
        }
    }

    if ((mode & 0X1F) == 0) // 无触摸点按下
    {
        if (tp_dev.sta & TP_PRES_DOWN) // 之前是被按下的
        {
            tp_dev.sta &= ~TP_PRES_DOWN; // 标记按键松开
        }
        else // 之前就没有被按下
        {
            tp_dev.x[0] = 0xffff;
            tp_dev.y[0] = 0xffff;
            tp_dev.sta &= 0XE0; // 清除点有效标记
        }
    }

    if (t > 240)
        t = 10; // 重新从10开始计数
    return res;
}
