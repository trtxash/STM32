/**
 * @file	oled.c
 * @brief 	OLED显示,150Hz没问题
 *          GND    电源地
 *          VCC  接5V或3.3v电源
 *          D0   接（SCL）,可在oled.h里更改
 *          D1   接（SDA）,可在oled.h里更改
 *          RES  PD2（SPI模块改成IIC模块需要接此引脚，IIC模块用户请忽略）
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2022年7月14号15点23分
 */
#include "oled.h"
#include "oledfont.h"

// OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
#if OLEDIIC == 1

/**
**  设置SDA为输出
**/
void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructer;
	I2CSDAPortClkEnable();
	GPIO_InitStructer.Pin = I2CSDAPin;
	GPIO_InitStructer.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructer.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(I2CSDAPort, &GPIO_InitStructer);
}
/**
**  设置SDA为输入
**/
void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructer;
	I2CSDAPortClkEnable();
	GPIO_InitStructer.Pin = I2CSDAPin;
	GPIO_InitStructer.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructer.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(I2CSDAPort, &GPIO_InitStructer);
}
//开始信号
void IIC_Start(void)
{
	SDA_OUT();
	OledSDA_H();
	OledSCL_H();
	delay_us(2);
	OledSDA_L();
	delay_us(2);
	OledSCL_L();
	delay_us(2);
}
void IIC_Stop(void)
{
	OledSCL_H();
	OledSDA_L();
	delay_us(2);
	OledSDA_H();
	delay_us(2);
}

/*
 *   返回1--应答出错
 *   放回0--应答正确
 */
u8 IIC_Wait_Ask(void)
{
	int count = 0;
	SDA_IN();
	OledSCL_H();
	delay_us(2);
	while (OledSDA_Read())
	{
		count++;
		if (count > 250)
		{
			IIC_Stop();
			return 1;
		}
	}
	OledSCL_L();
	delay_us(2);
	return 0;
}
//写一个字节
void IIC_WriteByte(u8 data)
{
	u8 i;
	SDA_OUT();
	for (i = 0; i < 8; i++)
	{
		OledSCL_L();
		delay_us(2);
		if (data & 0x80) // MSB,从高位开始一位一位传输
			OledSDA_H();
		else
			OledSDA_L();
		OledSCL_H();
		delay_us(2);
		OledSCL_L();
		data <<= 1;
	}
}
u8 IIC_ReadByte(void)
{
	u8 data, i;
	OledSDA_H();
	delay_us(3);
	for (i = 0; i < 8; i++)
	{
		data <<= 1;
		OledSCL_L();
		delay_us(1);
		OledSCL_H();
		delay_us(1);
		if (OledSDA_Read())
			data = data | 0x01;
		else
			data = data & 0xFE;
	}
	OledSCL_L();
	delay_us(3);
	return data;
}

void WriteCmd(u8 command)
{
	IIC_Start();
	IIC_WriteByte(0x78); // OLED地址
	IIC_Wait_Ask();
	IIC_WriteByte(0x00); //寄存器地址
	IIC_Wait_Ask();
	IIC_WriteByte(command);
	IIC_Wait_Ask();
	IIC_Stop();
}
void WriteDat(u8 data)
{
	IIC_Start();
	IIC_WriteByte(0x78); // OLED地址
	IIC_Wait_Ask();
	IIC_WriteByte(0x40); //寄存器地址
	IIC_Wait_Ask();
	IIC_WriteByte(data);
	IIC_Wait_Ask();
	IIC_Stop();
}
#else
#if OLED_MODE == 1 // 8080并口
/*****************************************************************
	向SSD1306写入一个字节
	dat:要写入的数据/命令
	cmd:数据/命令标志 0,表示命令;1,表示数据
*****************************************************************/
void OLED_WR_Byte(u8 dat, u8 cmd)
{
	DATAOUT(dat);
	if (cmd)
		OledRS_H();
	else
		OledRS_L();
	OledCs_L();
	OledWR_L();
	OledWR_H();
	OledCs_H();
	OledRS_H();
}
#else
void OLED_WR_Byte(u8 dat, u8 cmd)
{
	char i;
	OledCs_L();
#if OLED_SPI_X == 1
	OledSck_L();
	if (cmd)
	{
		OledSdin_H();
	}
	else
	{
		OledSdin_L();
	}
	OledSck_H();
#else
	if (cmd)
	{
		OledRS_H();
	}
	else
	{
		OledRS_L();
	}
#endif
	for (i = 0; i < 8; i++)
	{
		OledSck_L();
		if (dat & 0x80)
		{
			OledSdin_H();
		}
		else
		{
			OledSdin_L();
		}
		OledSck_H();
		dat <<= 1;
	}
	OledCs_H();
#if OLED_SPI_X == 0
	OledRS_H();
#endif
}
#endif
void WriteCmd(u8 command)
{
	OLED_WR_Byte(command, OLED_CMD);
}
void WriteDat(u8 data)
{
	OLED_WR_Byte(data, OLED_DATA);
}
#endif
// 公共部分

/*****************************************************************
	将OLED从休眠中唤醒
*****************************************************************/
void OLED_ON(void)
{
	WriteCmd(0X8D); //设置电荷泵
	WriteCmd(0X14); //开启电荷泵
	WriteCmd(0XAF); // OLED唤醒
}

/*****************************************************************
  设置光标
  x,光标x位置 0-127
  y,光标y位置 0-7
*****************************************************************/
void OLED_SetPos(u8 x, u8 y)
{
	WriteCmd(0xb0 + y);
	WriteCmd(((x & 0xf0) >> 4) | 0x10);
	WriteCmd((x & 0x0f) | 0x01);
}

/*****************************************************************
  填充整个屏幕
  fill_Data:要填充的数据
*****************************************************************/
void OLED_Fill(u8 fill_Data)
{
	u8 m, n;
	for (m = 0; m < 8; m++)
	{
		WriteCmd(0xb0 + m); // page0-page1
		WriteCmd(0x00);		// low column start address
		WriteCmd(0x10);		// high column start address
		for (n = 0; n < 128; n++)
		{
			WriteDat(fill_Data);
		}
	}
}

//清屏
void OLED_CLS(void)
{
	u8 i, n;
	for (i = 0; i < 8; i++)
	{
		WriteCmd(0xb0 + i);
		WriteCmd(0x00);
		WriteCmd(0x10);
		for (n = 0; n < 128; n++)
			WriteDat(0);
	}
}

/*****************************************************************
  显示oledfont.h中的ASCII字符,有6*8和8*16可选择
  x,y : 起始点坐标(x:0~127, y:0~7);
					 ch[] :- 要显示的字符串;
					 TextSize : 字符大小(1:6*8 ; 2:8*16)
*****************************************************************/
void OLED_ShowStr(u8 x, u8 y, u8 ch[], u8 TextSize)
{
	u8 c = 0, i = 0, j = 0;
	switch (TextSize)
	{
	case 1:
	{
		while (ch[j] != '\0')
		{
			c = ch[j] - 32;
			if (x > 126)
			{
				x = 0;
				y++;
			}
			OLED_SetPos(x, y);
			for (i = 0; i < 6; i++)
				WriteDat(F6x8[c][i]);
			x += 6;
			j++;
		}
	}
	break;
	case 2:
	{
		while (ch[j] != '\0')
		{
			c = ch[j] - 32;
			if (x > 120)
			{
				x = 0;
				y++;
			}
			OLED_SetPos(x, y);
			for (i = 0; i < 8; i++)
				WriteDat(F8X16[c * 16 + i]);
			OLED_SetPos(x, y + 1);
			for (i = 0; i < 8; i++)
				WriteDat(F8X16[c * 16 + i + 8]);
			x += 8;
			j++;
		}
	}
	break;
	}
}

void OLED_ShowChar(u8 x, u8 y, char chr, u8 Char_Size)
{
	u8 c = 0, i = 0;
	c = chr - ' ';
	if (x > 128 - 1)
	{
		x = 0;
		y = y + 2;
	}
	if (Char_Size == 16)
	{
		OLED_SetPos(x, y);
		for (i = 0; i < 8; i++)
			WriteDat(F8X16[c * 16 + i]);
		OLED_SetPos(x, y + 1);
		for (i = 0; i < 8; i++)
			WriteDat(F8X16[c * 16 + i + 8]);
	}
	else
	{
		OLED_SetPos(x, y);
		for (i = 0; i < 6; i++)
		{
			WriteDat(F6x8[c][i]);
		}
	}
}

/*****************************************************************
	显示字符串
	示例:OLED_ShowString(0,0,"4Wire SPI Runing",16);
*****************************************************************/
void OLED_ShowString(u8 x, u8 y, char *chr, u8 Char_Size)
{
	u8 j = 0;
	while (chr[j] != '\0')
	{
		OLED_ShowChar(x, y, chr[j], Char_Size);
		x += 8;
		if (x > 120)
		{
			x = 0;
			y += 2;
		}
		{
			j++;
		}
	}
}

/*****************************************************************
	显示中文
	no  :中文数组Hzk[] 编号
	示例:OLED_ShowCHinese(32,4,2);
*****************************************************************/
void OLED_ShowCHinese(u8 x, u8 y, u8 no)
{
	u8 t, adder = 0;
	OLED_SetPos(x, y);
	for (t = 0; t < 16; t++)
	{
		WriteDat(Hzk[2 * no][t]);
		adder += 1;
	}
	OLED_SetPos(x, y + 1);
	for (t = 0; t < 16; t++)
	{
		WriteDat(Hzk[2 * no + 1][t]);
		adder += 1;
	}
}

/*****************************************************************
	OLED初始化
*****************************************************************/
void OLED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
#if OLEDIIC == 1
	GPIO_InitStructure.Pin = I2CSCLPin | I2CSDAPin;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(I2CSCLPort, &GPIO_InitStructure);
#else
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pin = OledCsPin;
	HAL_GPIO_Init(OledCsPort, &GPIO_InitStructure);
	HAL_GPIO_WritePin(OledCsPort, OledCsPin, GPIO_PIN_SET);

	GPIO_InitStructure.Pin = OledRstPin;
	HAL_GPIO_Init(OledRstPort, &GPIO_InitStructure);
	HAL_GPIO_WritePin(OledRstPort, OledRstPin, GPIO_PIN_SET);

	GPIO_InitStructure.Pin = OledRSPin;
	HAL_GPIO_Init(OledRSPort, &GPIO_InitStructure);
	HAL_GPIO_WritePin(OledRSPort, OledRSPin, GPIO_PIN_SET);
#if OLED_MODE == 1
	GPIO_InitStructure.Pin = 0xFF; // PA0~7 OUT推挽输出
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	HAL_GPIO_WritePin(GPIOA, 0xFF, GPIO_PIN_SET); // PA0~7输出高

	GPIO_InitStructure.Pin = OledWRPin;
	HAL_GPIO_Init(OledWRPort, &GPIO_InitStructure);
	HAL_GPIO_WritePin(OledWRPort, OledWRPin, GPIO_PIN_SET);

	GPIO_InitStructure.Pin = OledRDPin;
	HAL_GPIO_Init(OledRDPort, &GPIO_InitStructure);
	HAL_GPIO_WritePin(OledRDPort, OledRDPin, GPIO_PIN_SET);
#else
	GPIO_InitStructure.Pin = OledSckPin;
	HAL_GPIO_Init(OledSckPort, &GPIO_InitStructure);
	HAL_GPIO_WritePin(OledSckPort, OledSckPin, GPIO_PIN_SET);

	GPIO_InitStructure.Pin = OledSdinPin;
	HAL_GPIO_Init(OledSdinPort, &GPIO_InitStructure);
	HAL_GPIO_WritePin(OledSdinPort, OledSdinPin, GPIO_PIN_SET);

#endif

	OledCs_H();
	OledRS_H();
	OledRst_L();
	delay_ms(100);
	OledRst_H();
#endif

	WriteCmd(0xAE); // display off
	WriteCmd(0x20); // Set Memory Addressing Mode
	WriteCmd(0x10); // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0); // Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8); // Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xdf); //亮度调节 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); // 0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
	OLED_CLS();
}

// //反显函数
// void OLED_ColorTurn(u8 i)
// {
// 	if (i == 0)
// 	{
// 		OLED_WR_Byte(0xA6, OLED_CMD); //正常显示
// 	}
// 	if (i == 1)
// 	{
// 		OLED_WR_Byte(0xA7, OLED_CMD); //反色显示
// 	}
// }

// //屏幕旋转180度
// void OLED_DisplayTurn(u8 i)
// {
// 	if (i == 0)
// 	{
// 		OLED_WR_Byte(0xC8, OLED_CMD); //正常显示
// 		OLED_WR_Byte(0xA1, OLED_CMD);
// 	}
// 	if (i == 1)
// 	{
// 		OLED_WR_Byte(0xC0, OLED_CMD); //反转显示
// 		OLED_WR_Byte(0xA0, OLED_CMD);
// 	}
// }

// //延时
// void IIC_delay(void)
// {
// 	u8 t = 3;
// 	while (t--)
// 		;
// }

// //起始信号
// void I2C_Start(void)
// {
// 	OLED_SDA_Set();
// 	OLED_SCL_Set();
// 	IIC_delay();
// 	OLED_SDA_Clr();
// 	IIC_delay();
// 	OLED_SCL_Clr();
// 	IIC_delay();
// }

// //结束信号
// void I2C_Stop(void)
// {
// 	OLED_SDA_Clr();
// 	OLED_SCL_Set();
// 	IIC_delay();
// 	OLED_SDA_Set();
// }

// //等待信号响应
// void I2C_WaitAck(void) //测数据信号的电平
// {
// 	OLED_SDA_Set();
// 	IIC_delay();
// 	OLED_SCL_Set();
// 	IIC_delay();
// 	OLED_SCL_Clr();
// 	IIC_delay();
// }

// //写入一个字节
// void Send_Byte(u8 dat)
// {
// 	u8 i;
// 	for (i = 0; i < 8; i++)
// 	{
// 		if (dat & 0x80) //将dat的8位从最高位依次写入
// 		{
// 			OLED_SDA_Set();
// 		}
// 		else
// 		{
// 			OLED_SDA_Clr();
// 		}
// 		IIC_delay();
// 		OLED_SCL_Set();
// 		IIC_delay();
// 		OLED_SCL_Clr(); //将时钟信号设置为低电平
// 		dat <<= 1;
// 	}
// }

// //发送一个字节
// // mode:数据/命令标志 0,表示命令;1,表示数据;
// void OLED_WR_Byte(u8 dat, u8 mode)
// {
// 	I2C_Start();
// 	Send_Byte(0x78);
// 	I2C_WaitAck();
// 	if (mode)
// 	{
// 		Send_Byte(0x40);
// 	}
// 	else
// 	{
// 		Send_Byte(0x00);
// 	}
// 	I2C_WaitAck();
// 	Send_Byte(dat);
// 	I2C_WaitAck();
// 	I2C_Stop();
// }

// //开启OLED显示
// void OLED_DisPlay_On(void)
// {
// 	OLED_WR_Byte(0x8D, OLED_CMD); //电荷泵使能
// 	OLED_WR_Byte(0x14, OLED_CMD); //开启电荷泵
// 	OLED_WR_Byte(0xAF, OLED_CMD); //点亮屏幕
// }

// //关闭OLED显示
// void OLED_DisPlay_Off(void)
// {
// 	OLED_WR_Byte(0x8D, OLED_CMD); //电荷泵使能
// 	OLED_WR_Byte(0x10, OLED_CMD); //关闭电荷泵
// 	OLED_WR_Byte(0xAE, OLED_CMD); //关闭屏幕
// }

// //更新显存到OLED
// void OLED_Refresh(void)
// {
// 	u8 i, n;
// 	for (i = 0; i < 8; i++)
// 	{
// 		OLED_WR_Byte(0xb0 + i, OLED_CMD); //设置行起始地址
// 		OLED_WR_Byte(0x00, OLED_CMD);	  //设置低列起始地址
// 		OLED_WR_Byte(0x10, OLED_CMD);	  //设置高列起始地址
// 		I2C_Start();
// 		Send_Byte(0x78);
// 		I2C_WaitAck();
// 		Send_Byte(0x40);
// 		I2C_WaitAck();
// 		for (n = 0; n < 128; n++)
// 		{
// 			Send_Byte(OLED_GRAM[n][i]);
// 			I2C_WaitAck();
// 		}
// 		I2C_Stop();
// 	}
// }
// //清屏函数
// void OLED_Clear(void)
// {
// 	u8 i, n;
// 	for (i = 0; i < 8; i++)
// 	{
// 		for (n = 0; n < 128; n++)
// 		{
// 			OLED_GRAM[n][i] = 0; //清除所有数据
// 		}
// 	}
// 	OLED_Refresh(); //更新显示
// }

// //画点
// // x:0~127
// // y:0~63
// // t:1 填充 0,清空
// void OLED_DrawPoint(u8 x, u8 y, u8 t)
// {
// 	u8 i, m, n;
// 	i = y / 8;
// 	m = y % 8;
// 	n = 1 << m;
// 	if (t)
// 	{
// 		OLED_GRAM[x][i] |= n;
// 	}
// 	else
// 	{
// 		OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
// 		OLED_GRAM[x][i] |= n;
// 		OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
// 	}
// }

// //画线
// // x1,y1:起点坐标
// // x2,y2:结束坐标
// void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2, u8 mode)
// {
// 	u16 t;
// 	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
// 	int incx, incy, uRow, uCol;
// 	delta_x = x2 - x1; //计算坐标增量
// 	delta_y = y2 - y1;
// 	uRow = x1; //画线起点坐标
// 	uCol = y1;
// 	if (delta_x > 0)
// 		incx = 1; //设置单步方向
// 	else if (delta_x == 0)
// 		incx = 0; //垂直线
// 	else
// 	{
// 		incx = -1;
// 		delta_x = -delta_x;
// 	}
// 	if (delta_y > 0)
// 		incy = 1;
// 	else if (delta_y == 0)
// 		incy = 0; //水平线
// 	else
// 	{
// 		incy = -1;
// 		delta_y = -delta_x;
// 	}
// 	if (delta_x > delta_y)
// 		distance = delta_x; //选取基本增量坐标轴
// 	else
// 		distance = delta_y;
// 	for (t = 0; t < distance + 1; t++)
// 	{
// 		OLED_DrawPoint(uRow, uCol, mode); //画点
// 		xerr += delta_x;
// 		yerr += delta_y;
// 		if (xerr > distance)
// 		{
// 			xerr -= distance;
// 			uRow += incx;
// 		}
// 		if (yerr > distance)
// 		{
// 			yerr -= distance;
// 			uCol += incy;
// 		}
// 	}
// }
// // x,y:圆心坐标
// // r:圆的半径
// void OLED_DrawCircle(u8 x, u8 y, u8 r)
// {
// 	int a, b, num;
// 	a = 0;
// 	b = r;
// 	while (2 * b * b >= r * r)
// 	{
// 		OLED_DrawPoint(x + a, y - b, 1);
// 		OLED_DrawPoint(x - a, y - b, 1);
// 		OLED_DrawPoint(x - a, y + b, 1);
// 		OLED_DrawPoint(x + a, y + b, 1);

// 		OLED_DrawPoint(x + b, y + a, 1);
// 		OLED_DrawPoint(x + b, y - a, 1);
// 		OLED_DrawPoint(x - b, y - a, 1);
// 		OLED_DrawPoint(x - b, y + a, 1);

// 		a++;
// 		num = (a * a + b * b) - r * r; //计算画的点离圆心的距离
// 		if (num > 0)
// 		{
// 			b--;
// 			a--;
// 		}
// 	}
// }

// //在指定位置显示一个字符,包括部分字符
// // x:0~127
// // y:0~63
// // size1:选择字体 6x8/6x12/8x16/12x24
// // mode:0,反色显示;1,正常显示
// void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size1, u8 mode)
// {
// 	u8 i, m, temp, size2, chr1;
// 	u8 x0 = x, y0 = y;
// 	if (size1 == 8)
// 		size2 = 6;
// 	else
// 		size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2); //得到字体一个字符对应点阵集所占的字节数
// 	chr1 = chr - ' ';											   //计算偏移后的值
// 	for (i = 0; i < size2; i++)
// 	{
// 		if (size1 == 8)
// 		{
// 			temp = asc2_0806[chr1][i];
// 		} //调用0806字体
// 		else if (size1 == 12)
// 		{
// 			temp = asc2_1206[chr1][i];
// 		} //调用1206字体
// 		else if (size1 == 16)
// 		{
// 			temp = asc2_1608[chr1][i];
// 		} //调用1608字体
// 		else if (size1 == 24)
// 		{
// 			temp = asc2_2412[chr1][i];
// 		} //调用2412字体
// 		else
// 			return;
// 		for (m = 0; m < 8; m++)
// 		{
// 			if (temp & 0x01)
// 				OLED_DrawPoint(x, y, mode);
// 			else
// 				OLED_DrawPoint(x, y, !mode);
// 			temp >>= 1;
// 			y++;
// 		}
// 		x++;
// 		if ((size1 != 8) && ((x - x0) == size1 / 2))
// 		{
// 			x = x0;
// 			y0 = y0 + 8;
// 		}
// 		y = y0;
// 	}
// }

// //显示字符串
// // x,y:起点坐标
// //*chr:字符串起始地址
// // size1:字体大小
// // mode:0,反色显示;1,正常显示
// void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 size1, u8 mode)
// {
// 	while ((*chr >= ' ') && (*chr <= '~')) //判断是不是非法字符!
// 	{
// 		OLED_ShowChar(x, y, *chr, size1, mode);
// 		if (size1 == 8)
// 			x += 6;
// 		else
// 			x += size1 / 2;
// 		chr++;
// 	}
// }

// // m^n
// u32 OLED_Pow(u8 m, u8 n)
// {
// 	u32 result = 1;
// 	while (n--)
// 	{
// 		result *= m;
// 	}
// 	return result;
// }

// //显示数字
// // x,y :起点坐标
// // num :要显示的数字
// // len :数字的位数
// // size:字体大小
// // mode:0,反色显示;1,正常显示
// void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size1, u8 mode)
// {
// 	u8 t, temp, m = 0;
// 	if (size1 == 8)
// 		m = 2;
// 	for (t = 0; t < len; t++)
// 	{
// 		temp = (num / OLED_Pow(10, len - t - 1)) % 10;
// 		if (temp == 0)
// 		{
// 			OLED_ShowChar(x + (size1 / 2 + m) * t, y, '0', size1, mode);
// 		}
// 		else
// 		{
// 			OLED_ShowChar(x + (size1 / 2 + m) * t, y, temp + '0', size1, mode);
// 		}
// 	}
// }

// //显示汉字
// // x,y:起点坐标
// // num:汉字对应的序号
// // mode:0,反色显示;1,正常显示
// void OLED_ShowChinese(u8 x, u8 y, u8 num, u8 size1, u8 mode)
// {
// 	u8 m, temp;
// 	u8 x0 = x, y0 = y;
// 	u16 i, size3 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * size1; //得到字体一个字符对应点阵集所占的字节数
// 	for (i = 0; i < size3; i++)
// 	{
// 		if (size1 == 16)
// 		{
// 			temp = Hzk1[num][i];
// 		} //调用16*16字体
// 		else if (size1 == 24)
// 		{
// 			temp = Hzk2[num][i];
// 		} //调用24*24字体
// 		else if (size1 == 32)
// 		{
// 			temp = Hzk3[num][i];
// 		} //调用32*32字体
// 		else if (size1 == 64)
// 		{
// 			temp = Hzk4[num][i];
// 		} //调用64*64字体
// 		else
// 			return;
// 		for (m = 0; m < 8; m++)
// 		{
// 			if (temp & 0x01)
// 				OLED_DrawPoint(x, y, mode);
// 			else
// 				OLED_DrawPoint(x, y, !mode);
// 			temp >>= 1;
// 			y++;
// 		}
// 		x++;
// 		if ((x - x0) == size1)
// 		{
// 			x = x0;
// 			y0 = y0 + 8;
// 		}
// 		y = y0;
// 	}
// }

// // num 显示汉字的个数
// // space 每一遍显示的间隔
// // mode:0,反色显示;1,正常显示
// void OLED_ScrollDisplay(u8 num, u8 space, u8 mode)
// {
// 	u8 i, n, t = 0, m = 0, r;
// 	while (1)
// 	{
// 		if (m == 0)
// 		{
// 			OLED_ShowChinese(128, 24, t, 16, mode); //写入一个汉字保存在OLED_GRAM[][]数组中
// 			t++;
// 		}
// 		if (t == num)
// 		{
// 			for (r = 0; r < 16 * space; r++) //显示间隔
// 			{
// 				for (i = 1; i < 144; i++)
// 				{
// 					for (n = 0; n < 8; n++)
// 					{
// 						OLED_GRAM[i - 1][n] = OLED_GRAM[i][n];
// 					}
// 				}
// 				OLED_Refresh();
// 			}
// 			t = 0;
// 		}
// 		m++;
// 		if (m == 16)
// 		{
// 			m = 0;
// 		}
// 		for (i = 1; i < 144; i++) //实现左移
// 		{
// 			for (n = 0; n < 8; n++)
// 			{
// 				OLED_GRAM[i - 1][n] = OLED_GRAM[i][n];
// 			}
// 		}
// 		OLED_Refresh();
// 	}
// }

// // x,y：起点坐标
// // sizex,sizey,图片长宽
// // BMP[]：要写入的图片数组
// // mode:0,反色显示;1,正常显示
// void OLED_ShowPicture(u8 x, u8 y, u8 sizex, u8 sizey, u8 BMP[], u8 mode)
// {
// 	u16 j = 0;
// 	u8 i, n, temp, m;
// 	u8 x0 = x, y0 = y;
// 	sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
// 	for (n = 0; n < sizey; n++)
// 	{
// 		for (i = 0; i < sizex; i++)
// 		{
// 			temp = BMP[j];
// 			j++;
// 			for (m = 0; m < 8; m++)
// 			{
// 				if (temp & 0x01)
// 					OLED_DrawPoint(x, y, mode);
// 				else
// 					OLED_DrawPoint(x, y, !mode);
// 				temp >>= 1;
// 				y++;
// 			}
// 			x++;
// 			if ((x - x0) == sizex)
// 			{
// 				x = x0;
// 				y0 = y0 + 8;
// 			}
// 			y = y0;
// 		}
// 	}
// }
// // OLED的初始化
// // GND  电源地
// // VCC  接5V或3.3v电源
// // D0   接PA14（SCL）
// // D1   接PA15（SDA）
// // RES  PD2（SPI模块改成IIC模块需要接此引脚，IIC模块用户请忽略）
// void OLED_Init(void)
// {
// 	GPIO_InitTypeDef GPIO_InitStruct;
// 	GPIO_InitTypeDef GPIO_InitStruct_2;
// 	GPIO_InitTypeDef GPIO_InitStruct_3;

// 	OLED_GPIO_SCL_ENABLE();
// 	OLED_GPIO_SDA_ENABLE();

// 	GPIO_InitStruct.Pin = OLED_GPIO_SCL_Pin;
// 	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
// 	GPIO_InitStruct.Pull = GPIO_PULLUP;
// 	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
// 	HAL_HAL_GPIO_Init(OLED_GPIO_SCL_Port, &GPIO_InitStruct);
// 	HAL_GPIO_WritePin(OLED_GPIO_SCL_Port, OLED_GPIO_SCL_Pin, GPIO_PIN_SET);

// 	GPIO_InitStruct_2.Pin = OLED_GPIO_SDA_Pin;
// 	GPIO_InitStruct_2.Mode = GPIO_MODE_OUTPUT_OD;
// 	GPIO_InitStruct_2.Pull = GPIO_PULLUP;
// 	GPIO_InitStruct_2.Speed = GPIO_SPEED_FREQ_HIGH;
// 	HAL_HAL_GPIO_Init(OLED_GPIO_SDA_Port, &GPIO_InitStruct_2);
// 	HAL_GPIO_WritePin(OLED_GPIO_SDA_Port, OLED_GPIO_SDA_Pin, GPIO_PIN_SET);

// 	/*Configure GPIO pin : PD2 */
// 	GPIO_InitStruct_3.Pin = GPIO_PIN_2;
// 	GPIO_InitStruct_3.Mode = GPIO_MODE_OUTPUT_PP;
// 	GPIO_InitStruct_3.Pull = GPIO_PULLUP;
// 	GPIO_InitStruct_3.Speed = GPIO_SPEED_FREQ_HIGH;
// 	HAL_HAL_GPIO_Init(GPIOD, &GPIO_InitStruct_3);
// 	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);

// 	OLED_RES_Clr();
// 	delay_ms(200);
// 	OLED_RES_Set();

// 	// OLED_WR_Byte(0xAE, OLED_CMD); //--turn off oled panel
// 	// OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
// 	// OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
// 	// OLED_WR_Byte(0x40, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
// 	// OLED_WR_Byte(0x81, OLED_CMD); //--set contrast control register
// 	// OLED_WR_Byte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
// 	// OLED_WR_Byte(0xA1, OLED_CMD); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
// 	// OLED_WR_Byte(0xC8, OLED_CMD); // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
// 	// OLED_WR_Byte(0xA6, OLED_CMD); //--set normal display
// 	// OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
// 	// OLED_WR_Byte(0x3f, OLED_CMD); //--1/64 duty
// 	// OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
// 	// OLED_WR_Byte(0x00, OLED_CMD); //-not offset
// 	// OLED_WR_Byte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
// 	// OLED_WR_Byte(0x80, OLED_CMD); //--set divide ratio, Set Clock as 100 Frames/Sec
// 	// OLED_WR_Byte(0xD9, OLED_CMD); //--set pre-charge period
// 	// OLED_WR_Byte(0xF1, OLED_CMD); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
// 	// OLED_WR_Byte(0xDA, OLED_CMD); //--set com pins hardware configuration
// 	// OLED_WR_Byte(0x12, OLED_CMD);
// 	// OLED_WR_Byte(0xDB, OLED_CMD); //--set vcomh
// 	// OLED_WR_Byte(0x40, OLED_CMD); // Set VCOM Deselect Level
// 	// OLED_WR_Byte(0x20, OLED_CMD); //-Set Page Addressing Mode (0x00/0x01/0x02)
// 	// OLED_WR_Byte(0x02, OLED_CMD); //
// 	// OLED_WR_Byte(0x8D, OLED_CMD); //--set Charge Pump enable/disable
// 	// OLED_WR_Byte(0x14, OLED_CMD); //--set(0x10) disable
// 	// OLED_WR_Byte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
// 	// OLED_WR_Byte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
// 	// OLED_Clear();
// 	// OLED_WR_Byte(0xAF, OLED_CMD);

// 	OLED_WR_Byte(0xAE, OLED_CMD); //--display off
// 	OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
// 	OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
// 	OLED_WR_Byte(0x40, OLED_CMD); //--set start line address
// 	OLED_WR_Byte(0xB0, OLED_CMD); //--set page address
// 	OLED_WR_Byte(0x81, OLED_CMD); // contract control
// 	OLED_WR_Byte(0xFF, OLED_CMD); //--128
// 	OLED_WR_Byte(0xA1, OLED_CMD); // set segment remap
// 	OLED_WR_Byte(0xA6, OLED_CMD); //--normal / reverse
// 	OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
// 	OLED_WR_Byte(0x3F, OLED_CMD); //--1/32 duty
// 	OLED_WR_Byte(0xC8, OLED_CMD); // Com scan direction
// 	OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset
// 	OLED_WR_Byte(0x00, OLED_CMD); //
// 	OLED_WR_Byte(0xD5, OLED_CMD); // set osc division
// 	OLED_WR_Byte(0x80, OLED_CMD); //
// 	OLED_WR_Byte(0xD8, OLED_CMD); // set area color mode off
// 	OLED_WR_Byte(0x05, OLED_CMD); //
// 	OLED_WR_Byte(0xD9, OLED_CMD); // Set Pre-Charge Period
// 	OLED_WR_Byte(0xF1, OLED_CMD); //
// 	OLED_WR_Byte(0xDA, OLED_CMD); // set com pin configuartion
// 	OLED_WR_Byte(0x12, OLED_CMD); //
// 	OLED_WR_Byte(0xDB, OLED_CMD); // set Vcomh
// 	OLED_WR_Byte(0x30, OLED_CMD); //
// 	OLED_WR_Byte(0x8D, OLED_CMD); // set charge pump enable
// 	OLED_WR_Byte(0x14, OLED_CMD); //
// 	OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel
// }

// /**
//  * @brief	OLED初始化后的一些显示
//  * @param 	none
//  * @arg		none
//  * @note  	显示初始化
//  * @retval	none
//  */
// void OLED_DisplayInit(void)
// {
// 	// OLED_Clear();
// 	// OLED_ShowString(0, 0, "Init OK!", 16, 1);
// 	// OLED_Refresh();
// 	OLED_ShowString(0, 0, "PWM", 16, 1);		 // 显示：PWM
// 	OLED_ShowChinese(48 - 24, 0, 0, 16, 1);		 // 显示：通
// 	OLED_ShowChinese(64 - 24, 0, 1, 16, 1);		 // 显示：道
// 	OLED_ShowChar(80 - 24, 0, ':', 16, 1);		 // 显示：：
// 	OLED_ShowString(128 - 32, 0, "(Hz)", 16, 1); // 显示：(Hz)

// 	OLED_ShowString(0, 16, "1:", 16, 1);
// 	OLED_ShowString(0, 32, "2:", 16, 1);
// 	OLED_ShowString(0, 48, "3:", 16, 1);
// 	OLED_ShowString(64, 16, "4:", 16, 1);
// 	OLED_Refresh();
// }

// /**
//  * @brief	OLED显示
//  * @param 	none
//  * @arg		none
//  * @note  	显示
//  * @retval	none
//  */
// void OLED_Display(void)
// {
// 	OLED_ShowString(0, 0, "S1:     S2:     ", 16, 1);
// 	OLED_ShowString(0, 16, "V1:     V2:     ", 16, 1);
// 	OLED_Refresh();
// }