/**
 * @file	xfs5152.h
 * @brief 	移植Speech
 * @author 	TRTX-gamer      https://github.com/TRTX-gamer；
 *          突然吐血    https://space.bilibili.com/12890038;
 * @version 1.00
 * @date 	2022年10月29号16点48分
 */
#include "xfs5152.h"
// uft8编码下中文使用speech_text_utf8函数，其他用speech_text

u8 ack; /*应答标志位*/

void xfs5152Drv_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    /* GPIO时钟开启 */
    xfs5152_RST_Port_Clk_Enable();
    xfs5152_SCLK_Port_Clk_Enable();
    xfs5152_SDIN_Port_Clk_Enable();

    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; // 推挽输出
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Pin = xfs5152_RST_Pin;
    HAL_GPIO_Init(xfs5152_RST_Port, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = xfs5152_SCLK_Pin;
    HAL_GPIO_Init(xfs5152_SCLK_Port, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = xfs5152_SDIN_Pin;
    HAL_GPIO_Init(xfs5152_SDIN_Port, &GPIO_InitStructure);

    HAL_GPIO_WritePin(xfs5152_SCLK_Port, xfs5152_SCLK_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(xfs5152_SDIN_Port, xfs5152_SDIN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(xfs5152_RST_Port, xfs5152_RST_Pin, GPIO_PIN_RESET);
    delay_ms(200);
    HAL_GPIO_WritePin(xfs5152_RST_Port, xfs5152_RST_Pin, GPIO_PIN_SET);
}

/*******************************************************************
                     起动总线函数
函数原型: void  Start_I2c();
功能:     启动I2C总线,即发送I2C起始条件.
********************************************************************/
void Start_I2c()
{
    xfs5152_SDIN_Set(); /*发送起始条件的数据信号*/
    delay_us(1);
    xfs5152_SCLK_Set();
    delay_us(1); /*起始条件建立时间大于4.7us,延时*/
    delay_us(1);
    delay_us(1);
    delay_us(1);
    delay_us(1);
    xfs5152_SDIN_Clr(); /*发送起始信号*/
    delay_us(1);        /* 起始条件锁定时间大于4μs*/
    delay_us(1);
    delay_us(1);
    delay_us(1);
    delay_us(1);
    xfs5152_SCLK_Clr(); /*钳住I2C总线，准备发送或接收数据 */
    delay_us(1);
    delay_us(1);
}

/*******************************************************************
                      结束总线函数
函数原型: void  Stop_I2c();
功能:     结束I2C总线,即发送I2C结束条件.
********************************************************************/
void Stop_I2c()
{
    xfs5152_SDIN_Clr(); /*发送结束条件的数据信号*/
    delay_us(1);        /*发送结束条件的时钟信号*/
    xfs5152_SCLK_Set(); /*结束条件建立时间大于4μs*/
    delay_us(1);
    delay_us(1);
    delay_us(1);
    delay_us(1);
    delay_us(1);
    xfs5152_SDIN_Set(); /*发送I2C总线结束信号*/
    delay_us(1);
    delay_us(1);
    delay_us(1);
    delay_us(1);
}

/*******************************************************************
                 字节数据发送函数
函数原型: void  I2C_SendByte(UCHAR c);
功能:     将数据c发送出去,可以是地址,也可以是数据,发完后等待应答,并对
          此状态位进行操作.(不应答或非应答都使ack=0)
           发送数据正常，ack=1; ack=0表示被控器无应答或损坏。
********************************************************************/
void I2C_SendByte(unsigned char c)
{
    unsigned char i;

    for (i = 0; i < 8; i++) /*要传送的数据长度为8位*/
    {
        if ((c << i) & 0x80)
            xfs5152_SDIN_Set(); /*判断发送位*/
        else
            xfs5152_SDIN_Clr();
        delay_us(1);
        xfs5152_SCLK_Set(); /*置时钟线为高，通知被控器开始接收数据位*/
        delay_us(1);
        delay_us(1); /*保证时钟高电平周期大于4μs*/
        delay_us(1);
        delay_us(1);
        delay_us(1);
        xfs5152_SCLK_Clr();
    }
    delay_us(1);
    delay_us(1);
    delay_us(1);
    delay_us(1);
    xfs5152_SDIN_Set(); /*8位发送完后释放数据线，准备接收应答位*/
    delay_us(1);
    delay_us(1);
    delay_us(1);
    delay_us(1);
    xfs5152_SCLK_Set();
    delay_us(1);
    delay_us(1);
    delay_us(1);
    if (xfs5152_READ_SDIN() == 1)
        ack = 0;
    else
        ack = 1; /*判断是否接收到应答信号*/
    xfs5152_SCLK_Clr();
    delay_us(1);
    delay_us(1);
}

/*******************************************************************
                 字节数据接收函数
函数原型: UCHAR  I2C_RcvByte();
功能:        用来接收从器件传来的数据,并判断总线错误(不发应答信号)，
          发完后请用应答函数应答从机。
********************************************************************/
unsigned char I2C_RcvByte()
{
    unsigned char retc = 0, i = 0;
    xfs5152_SDIN_Set(); /*置数据线为输入方式*/
    for (i = 0; i < 8; i++)
    {
        delay_us(1);
        xfs5152_SCLK_Clr(); /*置时钟线为低，准备接收数据位*/
        delay_us(1);
        delay_us(1); /*时钟低电平周期大于4.7μs*/
        delay_us(1);
        delay_us(1);
        delay_us(1);
        xfs5152_SCLK_Set(); /*置时钟线为高使数据线上数据有效*/
        delay_us(1);
        delay_us(1);
        retc = retc << 1;
        if (xfs5152_READ_SDIN() == 1)
            retc = retc + 1; /*读数据位,接收的数据位放入retc中 */
        delay_us(1);
        delay_us(1);
    }
    xfs5152_SCLK_Clr();
    delay_us(1);
    delay_us(1);
    return (retc);
}

/********************************************************************
                     应答子函数
函数原型:  void Ack_I2c(u8 a);
功能:      主控器进行应答信号(可以是应答或非应答信号，由位参数a决定)
********************************************************************/
void Ack_I2c(u8 a)
{
    if (a == 0)
        xfs5152_SDIN_Clr(); /*在此发出应答或非应答信号 */
    else
        xfs5152_SDIN_Set(); /*0为发出应答，1为非应答信号 */
    delay_us(1);
    delay_us(1);
    delay_us(1);
    xfs5152_SCLK_Set();
    delay_us(1);
    delay_us(1); /*时钟低电平周期大于4μs*/
    delay_us(1);
    delay_us(1);
    delay_us(1);
    xfs5152_SCLK_Clr(); /*清时钟线，住I2C总线以便继续接收*/
    delay_us(1);
    delay_us(1);
}

u8 I2C_ByteWrite(u8 dat)
{
    Start_I2c();            // 启动总线
    I2C_SendByte(I2C_ADDR); // 发送器件地址
    if (ack == 0)
        return (0);
    I2C_SendByte(dat); // 发送数据
    if (ack == 0)
        return (0);
    Stop_I2c(); // 结束总线
    delay_us(30);
    return (1);
}

void I2C_Writes_Bytes(u8 *buff, int number)
{
    int i;
    for (i = 0; i < number; i++)
    {
        I2C_ByteWrite(buff[i]);
    }
}

u8 GetChipStatus()
{
    u8 dat = 0xff;
    u8 *pBuffer = 0xff;
    u8 AskState[4] = {0xFD, 0x00, 0x01, 0x21};

    I2C_Writes_Bytes(AskState, 4);

    Start_I2c();                // 启动总线
    I2C_SendByte(I2C_ADDR + 1); // 发送器件地址
    if (ack == 0)
        return (0);
    delay_us(10);        // 必须加一点延时
    dat = I2C_RcvByte(); // 读取数据

    Ack_I2c(1); // 发送非应答信号
    Stop_I2c(); // 结束总线
    return (dat);
}

// uft8编码下中文使用这个函数，其他用speech_text
void speech_text_utf8(u8 *str)
{
    u8 temp[128] = {0};
    Utf8ToUnicode(str, temp);
    u16 size = strlen(temp) + 2;

    XFS_Protocol_TypeDef DataPack;

    DataPack.DataHead = DATAHEAD;
    DataPack.Length_HH = size >> 8;
    DataPack.Length_LL = size;
    DataPack.Commond = 0x01;
    DataPack.EncodingFormat = UNICODE;
    DataPack.Text = temp;

    I2C_ByteWrite(DataPack.DataHead);
    I2C_ByteWrite(DataPack.Length_HH);
    I2C_ByteWrite(DataPack.Length_LL);
    I2C_ByteWrite(DataPack.Commond);
    I2C_ByteWrite(DataPack.EncodingFormat);

    I2C_Writes_Bytes(DataPack.Text, strlen(DataPack.Text));
}

void speech_text(u8 *str, u8 encoding_format)
{
    u16 size = strlen(str) + 2;

    XFS_Protocol_TypeDef DataPack;

    DataPack.DataHead = DATAHEAD;
    DataPack.Length_HH = size >> 8;
    DataPack.Length_LL = size;
    DataPack.Commond = 0x01;
    DataPack.EncodingFormat = encoding_format;
    DataPack.Text = str;

    I2C_ByteWrite(DataPack.DataHead);
    I2C_ByteWrite(DataPack.Length_HH);
    I2C_ByteWrite(DataPack.Length_LL);
    I2C_ByteWrite(DataPack.Commond);
    I2C_ByteWrite(DataPack.EncodingFormat);

    I2C_Writes_Bytes(DataPack.Text, strlen(DataPack.Text));
}

void SetBase(u8 *str)
{
    u8 i;
    u16 size = strlen(str) + 2;

    XFS_Protocol_TypeDef DataPack;

    DataPack.DataHead = DATAHEAD;
    DataPack.Length_HH = size >> 8;
    DataPack.Length_LL = size;
    DataPack.Commond = 0x01;
    DataPack.EncodingFormat = 0x00;
    DataPack.Text = str;
    for (i = 0; i < size; i++)
    {
        printf("%x\r\n", str[i]);
    }

    I2C_ByteWrite(DataPack.DataHead);
    I2C_ByteWrite(DataPack.Length_HH);
    I2C_ByteWrite(DataPack.Length_LL);
    I2C_ByteWrite(DataPack.Commond);
    I2C_ByteWrite(DataPack.EncodingFormat);

    I2C_Writes_Bytes(DataPack.Text, strlen(DataPack.Text));
}

void TextCtrl(char c, int d)
{
    char str[10];
    if (d != -1)
        sprintf(str, "[%c%d]", c, d);
    else
        sprintf(str, "[%c]", c);
    SetBase(str);
}

void SetStyle(Style_Type style)
{
    TextCtrl('f', style);
    while (GetChipStatus() != ChipStatus_Idle)
    {
        // delay_us(3000);
    }
}

void SetLanguage(Language_Type language)
{
    TextCtrl('g', language);
    while (GetChipStatus() != ChipStatus_Idle)
    {
        // delay_us(3000);
    }
}

void SetArticulation(Articulation_Type articulation)
{
    TextCtrl('h', articulation);
    while (GetChipStatus() != ChipStatus_Idle)
    {
        // delay_us(3000);
    }
}

void SetSpell(Spell_Type spell)
{
    TextCtrl('i', spell);
    while (GetChipStatus() != ChipStatus_Idle)
    {
        // delay_us(3000);
    }
}

void SetReader(Reader_Type reader)
{
    TextCtrl('m', reader);
    while (GetChipStatus() != ChipStatus_Idle)
    {
        // delay_us(3000);
    }
}

void SetNumberHandle(NumberHandle_Type numberHandle)
{
    TextCtrl('n', numberHandle);
    while (GetChipStatus() != ChipStatus_Idle)
    {
        // delay_us(3000);
    }
}

void SetZeroPronunciation(ZeroPronunciation_Type zeroPronunciation)
{
    TextCtrl('o', zeroPronunciation);
    while (GetChipStatus() != ChipStatus_Idle)
    {
        // delay_us(3000);
    }
}

void SetNamePronunciation(NamePronunciation_Type namePronunciation)
{
    TextCtrl('r', namePronunciation);
    while (GetChipStatus() != ChipStatus_Idle)
    {
        // delay_us(3000);
    }
}

void SetSpeed(int speed)
{
    TextCtrl('s', speed);
}

void SetIntonation(int intonation)
{
    TextCtrl('t', intonation);
    while (GetChipStatus() != ChipStatus_Idle)
    {
        // delay_us(3000);
    }
}

void SetVolume(int volume)
{
    TextCtrl('v', volume);
    while (GetChipStatus() != ChipStatus_Idle)
    {
        // delay_us(3000);
    }
}

void SetOnePronunciation(OnePronunciation_Type onePronunciation)
{
    TextCtrl('y', onePronunciation);
    while (GetChipStatus() != ChipStatus_Idle)
    {
        // delay_us(3000);
    }
}

void SetRhythm(Rhythm_Type rhythm)
{
    TextCtrl('z', rhythm);
    while (GetChipStatus() != ChipStatus_Idle)
    {
        // delay_us(3000);
    }
}

void SetRestoreDefault()
{
    TextCtrl('d', -1);
    while (GetChipStatus() != ChipStatus_Idle)
    {
        // delay_us(3000);
    }
}

/* 编码转换函数，因为有可能不支持相应编码，所以要转换------------------------------------------------------------------ */

/*
Unicode符号范围     |        UTF-8编码方式
(十六进制)        |              （二进制）
----------------------+---------------------------------------------
0000 0000-0000 007F | 0xxxxxxx
0000 0080-0000 07FF | 110xxxxx 10xxxxxx
0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
*/

/*************************************************************************************************
 * 将UTF8编码转换成Unicode（UCS-2LE）编码  低地址存低位字节
 * 参数：
 *    char* pInput     输入字符串
 *    char*pOutput   输出字符串
 * 返回值：转换后的Unicode字符串的字节数，如果出错则返回-1
 **************************************************************************************************/
// utf8转unicode
int Utf8ToUnicode(char *pInput, char *pOutput)
{
    int outputSize = 0; // 记录转换后的Unicode字符串的字节数

    while (*pInput)
    {
        if (*pInput > 0x00 && *pInput <= 0x7F) // 处理单字节UTF8字符（英文字母、数字）
        {
            *pOutput = *pInput;
            // pOutput++;
            // *pOutput = 0; // 小端法表示，在高地址填补0
        }
        else if (((*pInput) & 0xE0) == 0xC0) // 处理双字节UTF8字符
        {
            char high = *pInput;
            pInput++;
            char low = *pInput;
            if ((low & 0xC0) != 0x80) // 检查是否为合法的UTF8字符表示
            {
                return -1; // 如果不是则报错
            }

            *pOutput = (high << 6) + (low & 0x3F);
            pOutput++;
            *pOutput = (high >> 2) & 0x07;
        }
        else if (((*pInput) & 0xF0) == 0xE0) // 处理三字节UTF8字符
        {
            char high = *pInput;
            pInput++;
            char middle = *pInput;
            pInput++;
            char low = *pInput;
            if (((middle & 0xC0) != 0x80) || ((low & 0xC0) != 0x80))
            {
                return -1;
            }
            *pOutput = (middle << 6) + (low & 0x3F); // 取出middle的低两位与low的低6位，组合成unicode字符的低8位
            pOutput++;
            *pOutput = (high << 4) + ((middle >> 2) & 0x0F); // 取出high的低四位与middle的中间四位，组合成unicode字符的高8位
        }
        else // 对于其他字节数的UTF8字符不进行处理
        {
            return -1;
        }
        pInput++; // 处理下一个utf8字符
        pOutput++;
        outputSize += 2;
    }
    // unicode字符串后面，有两个\0
    *pOutput = 0;
    pOutput++;
    *pOutput = 0;
    return outputSize;
}

int UnicodeToUtf8(char *pInput, char *pOutput)
{
    int len = 0; // 记录转换后的Utf8字符串的字节数
    while (*pInput)
    {
        // 处理一个unicode字符
        char low = *pInput; // 取出unicode字符的低8位
        pInput++;
        char high = *pInput; // 取出unicode字符的高8位
        int w = high << 8;
        unsigned wchar = (high << 8) + low; // 高8位和低8位组成一个unicode字符,加法运算级别高

        if (wchar <= 0x7F) // 英文字符
        {
            pOutput[len] = (char)wchar; // 取wchar的低8位
            len++;
        }
        else if (wchar >= 0x80 && wchar <= 0x7FF) // 可以转换成双字节pOutput字符
        {
            pOutput[len] = 0xc0 | ((wchar >> 6) & 0x1f); // 取出unicode编码低6位后的5位，填充到110yyyyy 10zzzzzz 的yyyyy中
            len++;
            pOutput[len] = 0x80 | (wchar & 0x3f); // 取出unicode编码的低6位，填充到110yyyyy 10zzzzzz 的zzzzzz中
            len++;
        }
        else if (wchar >= 0x800 && wchar < 0xFFFF) // 可以转换成3个字节的pOutput字符
        {
            pOutput[len] = 0xe0 | ((wchar >> 12) & 0x0f); // 高四位填入1110xxxx 10yyyyyy 10zzzzzz中的xxxx
            len++;
            pOutput[len] = 0x80 | ((wchar >> 6) & 0x3f); // 中间6位填入1110xxxx 10yyyyyy 10zzzzzz中的yyyyyy
            len++;
            pOutput[len] = 0x80 | (wchar & 0x3f); // 低6位填入1110xxxx 10yyyyyy 10zzzzzz中的zzzzzz
            len++;
        }

        else // 对于其他字节数的unicode字符不进行处理
        {
            return -1;
        }
        pInput++; // 处理下一个unicode字符
    }
    // utf8字符串后面，有个\0
    pOutput[len] = 0;
    return len;
}