#include "speech.h"

static __IO uint32_t I2CTimeout = I2CT_LONG_TIMEOUT;

/**
 * @brief  Basic management of the timeout situation.
 * @param  None.
 * @retval None.
 */
static uint8_t I2C_TIMEOUT_UserCallback(void)
{
	/* Block communication and all processes */
	// MPU_ERROR("I2C Timeout error!");

	return 0;
}

/**
 * @brief   写一个字节到I2C设备中
 * @param
 *		@arg pBuffer:写的值
 * @retval  正常返回1，异常返回0
 */
uint8_t I2C_ByteWrite(u8 pBuffer)
{
	// printf("%d\n", pBuffer);
	delay_us(10);
	if (HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDR, &pBuffer, 1, I2CT_FLAG_TIMEOUT) == HAL_OK)
		return 1;
	else
		return 0;

	// I2CTimeout = I2CT_FLAG_TIMEOUT;
	// while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
	// {
	// 	if ((I2CTimeout--) == 0)
	// 		return I2C_TIMEOUT_UserCallback();
	// }

	// /* Send STRAT condition */
	// I2C_GenerateSTART(I2C1, ENABLE);

	// I2CTimeout = I2CT_FLAG_TIMEOUT;

	// /* Test on EV5 and clear it */
	// while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
	// {
	// 	if ((I2CTimeout--) == 0)
	// 		return I2C_TIMEOUT_UserCallback();
	// }

	// /* Send slave address for write */
	// I2C_Send7bitAddress(I2C1, I2C_ADDR, I2C_Direction_Transmitter);

	// I2CTimeout = I2CT_FLAG_TIMEOUT;
	// /* Test on EV6 and clear it */
	// while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	// {
	// 	if ((I2CTimeout--) == 0)
	// 		return I2C_TIMEOUT_UserCallback();
	// }

	// /* Send the byte to be written */
	// I2C_SendData(I2C1, pBuffer);

	// I2CTimeout = I2CT_FLAG_TIMEOUT;

	// /* Test on EV8 and clear it */
	// while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	// {
	// 	if ((I2CTimeout--) == 0)
	// 		return I2C_TIMEOUT_UserCallback();
	// }
	// delay_us(200);
	// /* Send STOP condition */
	// I2C_GenerateSTOP(I2C1, ENABLE);
	// delay_us(200);
	// return 1; //正常返回1
}

void I2C_Writes_Bytes(u8 *buff, int number)
{
	int i;
	for (i = 0; i < number; i++)
	{
		I2C_ByteWrite(buff[i]);
	}
}

/**
 * @brief   获取芯片状态值
 * @param
 * @retval  返回状态值
 */
u8 GetChipStatus(void)
{
	u8 pBuffer = 0xff;
	u8 AskState[4] = {0xFD, 0x00, 0x01, 0x21};

	I2C_Writes_Bytes(AskState, 4);
	delay_us(100);

	while (HAL_I2C_Master_Receive(&hi2c1, I2C_ADDR, &pBuffer, 1, I2CT_FLAG_TIMEOUT) != HAL_OK)
	{
		// printf("Error:%d\n",HAL_I2C_GetError(&hi2c1));
	}
	return pBuffer;
	// u16 NumByteToRead = 1;
	// u8 pBuffer = 0xff;
	// u8 AskState[4] = {0xFD, 0x00, 0x01, 0x21};

	// I2C_Writes_Bytes(AskState, 4);

	// delay_us(350);
	// /* Send STRAT condition a second time */
	// I2C_GenerateSTART(I2C1, ENABLE);

	// I2CTimeout = I2CT_FLAG_TIMEOUT;
	// /* Test on EV5 and clear it */
	// while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
	// {
	// 	if ((I2CTimeout--) == 0)
	// 		return I2C_TIMEOUT_UserCallback();
	// }

	// /* Send slave address for read */
	// I2C_Send7bitAddress(I2C1, I2C_ADDR, I2C_Direction_Receiver);

	// I2CTimeout = I2CT_FLAG_TIMEOUT;

	// /* Test on EV6 and clear it */
	// while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	// {
	// 	if ((I2CTimeout--) == 0)
	// 		return I2C_TIMEOUT_UserCallback();
	// }

	// /* While there is data to be read */
	// while (NumByteToRead)
	// {
	// 	if (NumByteToRead == 1)
	// 	{
	// 		/* Disable Acknowledgement */
	// 		I2C_AcknowledgeConfig(I2C1, DISABLE);

	// 		/* Send STOP Condition */
	// 		I2C_GenerateSTOP(I2C1, ENABLE);
	// 	}

	// 	/* Test on EV7 and clear it */
	// 	if (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
	// 	{

	// 		/* Read a byte from the slave */
	// 		pBuffer = I2C_ReceiveData(I2C1);

	// 		/* Decrement the read bytes counter */
	// 		NumByteToRead--;
	// 	}
	// }
	// /* Enable Acknowledgement to be ready for another reception */

	// return pBuffer;
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
	u16 size = strlen(str) + 2;

	XFS_Protocol_TypeDef DataPack;

	DataPack.DataHead = DATAHEAD;
	DataPack.Length_HH = size >> 8;
	DataPack.Length_LL = size;
	DataPack.Commond = 0x01;
	DataPack.EncodingFormat = 0x00;
	DataPack.Text = str;

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
	// while (GetChipStatus() != ChipStatus_Idle)
	// {
	// 	delay_us(10);
	// }
}

void SetLanguage(Language_Type language)
{
	TextCtrl('g', language);
	// while (GetChipStatus() != ChipStatus_Idle)
	// {
	// 	delay_us(10);
	// }
}

void SetArticulation(Articulation_Type articulation)
{
	TextCtrl('h', articulation);
	// while (GetChipStatus() != ChipStatus_Idle)
	// {
	// 	delay_us(10);
	// }
}

void SetSpell(Spell_Type spell)
{
	TextCtrl('i', spell);
	// while (GetChipStatus() != ChipStatus_Idle)
	// {
	// 	delay_us(10);
	// }
}

void SetReader(Reader_Type reader)
{
	TextCtrl('m', reader);
	// while (GetChipStatus() != ChipStatus_Idle)
	// {
	// 	delay_us(10);
	// }
}

void SetNumberHandle(NumberHandle_Type numberHandle)
{
	TextCtrl('n', numberHandle);
	// while (GetChipStatus() != ChipStatus_Idle)
	// {
	// 	delay_us(10);
	// }
}

void SetZeroPronunciation(ZeroPronunciation_Type zeroPronunciation)
{
	TextCtrl('o', zeroPronunciation);
	// while (GetChipStatus() != ChipStatus_Idle)
	// {
	// 	delay_us(10);
	// }
}

void SetNamePronunciation(NamePronunciation_Type namePronunciation)
{
	TextCtrl('r', namePronunciation);
	// while (GetChipStatus() != ChipStatus_Idle)
	// {
	// 	delay_us(10);
	// }
}

void SetSpeed(int speed)
{
	TextCtrl('s', speed);
	// while (GetChipStatus() != ChipStatus_Idle)
	// {
	// 	delay_us(10);
	// }
}

void SetIntonation(int intonation)
{
	TextCtrl('t', intonation);
	// while (GetChipStatus() != ChipStatus_Idle)
	// {
	// 	delay_us(10);
	// }
}

void SetVolume(int volume)
{
	TextCtrl('v', volume);
	// while (GetChipStatus() != ChipStatus_Idle)
	// {
	// 	delay_us(10);
	// }
}

void SetOnePronunciation(OnePronunciation_Type onePronunciation)
{
	TextCtrl('y', onePronunciation);
	// while (GetChipStatus() != ChipStatus_Idle)
	// {
	// 	delay_us(10);
	// }
}

void SetRhythm(Rhythm_Type rhythm)
{
	TextCtrl('z', rhythm);
	// while (GetChipStatus() != ChipStatus_Idle)
	// {
	// 	delay_us(10);
	// }
}

void SetRestoreDefault()
{
	TextCtrl('d', -1);
	// while (GetChipStatus() != ChipStatus_Idle)
	// {
	// 	delay_us(10);
	// }
}