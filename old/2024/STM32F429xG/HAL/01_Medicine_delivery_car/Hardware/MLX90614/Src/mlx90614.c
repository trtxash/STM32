#include "MLX90614.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/*******************************************************************************
 * Function Name  : SMBus_StartBit
 * Description    : Generate START condition on SMBus
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SMBus_StartBit(void)
{
    SMBUS_SDA_H(); // Set SDA line
    delay_us(5);   // Wait a few microseconds
    SMBUS_SCK_H(); // Set SCL line
    delay_us(5);   // Generate bus free time between Stop
    SMBUS_SDA_L(); // Clear SDA line
    delay_us(5);   // Hold time after (Repeated) Start
    SMBUS_SCK_L(); // Clear SCL line
    delay_us(5);   // Wait a few microseconds
}

/*******************************************************************************
 * Function Name  : SMBus_StopBit
 * Description    : Generate STOP condition on SMBus
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SMBus_StopBit(void)
{
    SMBUS_SCK_L(); // Clear SCL line
    delay_us(5);   // Wait a few microseconds
    SMBUS_SDA_L(); // Clear SDA line
    delay_us(5);   // Wait a few microseconds
    SMBUS_SCK_H(); // Set SCL line
    delay_us(5);   // Stop condition setup time(Tsu:sto=4.0us min)
    SMBUS_SDA_H(); // Set SDA line?úSCK=1ê±￡??ì2aμ?SDAóé0μ?1±íê?í¨D??áê?￡¨é?éy??￡?
}

/*******************************************************************************
 * Function Name  : SMBus_SendByte
 * Description    : Send a byte on SMBus
 * Input          : Tx_buffer
 * Output         : None
 * Return         : None
 *******************************************************************************/
u8 SMBus_SendByte(u8 Tx_buffer)
{
    u8 Bit_counter;
    u8 Ack_bit;
    u8 bit_out;

    for (Bit_counter = 8; Bit_counter; Bit_counter--)
    {
        if (Tx_buffer & 0x80)
        {
            bit_out = 1; // If the current bit of Tx_buffer is 1 set bit_out
        }
        else
        {
            bit_out = 0; // else clear bit_out
        }
        SMBus_SendBit(bit_out); // Send the current bit on SDA
        Tx_buffer <<= 1;        // Get next bit for checking
    }

    Ack_bit = SMBus_ReceiveBit(); // Get acknowledgment bit
    return Ack_bit;
}

/*******************************************************************************
 * Function Name  : SMBus_SendBit
 * Description    : Send a bit on SMBus 82.5kHz
 * Input          : bit_out
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SMBus_SendBit(u8 bit_out)
{
    if (bit_out == 0)
    {
        SMBUS_SDA_L();
    }
    else
    {
        SMBUS_SDA_H();
    }
    delay_us(2);   // Tsu:dat = 250ns minimum
    SMBUS_SCK_H(); // Set SCL line
    delay_us(6);   // High Level of Clock Pulse
    SMBUS_SCK_L(); // Clear SCL line
    delay_us(3);   // Low Level of Clock Pulse
                   //	SMBUS_SDA_H();				    // Master release SDA line ,
    return;
}

/*******************************************************************************
 * Function Name  : SMBus_ReceiveBit
 * Description    : Receive a bit on SMBus
 * Input          : None
 * Output         : None
 * Return         : Ack_bit
 *******************************************************************************/
u8 SMBus_ReceiveBit(void)
{
    u8 Ack_bit;

    SMBUS_SDA_H(); //òy????ía2?μ?×èé?à-￡?μ±×÷ê?è?
    delay_us(2);   // High Level of Clock Pulse
    SMBUS_SCK_H(); // Set SCL line
    delay_us(5);   // High Level of Clock Pulse
    if (SMBUS_SDA_PIN())
    {
        Ack_bit = 1;
    }
    else
    {
        Ack_bit = 0;
    }
    SMBUS_SCK_L(); // Clear SCL line
    delay_us(3);   // Low Level of Clock Pulse

    return Ack_bit;
}

/*******************************************************************************
 * Function Name  : SMBus_ReceiveByte
 * Description    : Receive a byte on SMBus
 * Input          : ack_nack
 * Output         : None
 * Return         : RX_buffer
 *******************************************************************************/
u8 SMBus_ReceiveByte(u8 ack_nack)
{
    u8 RX_buffer;
    u8 Bit_Counter;

    for (Bit_Counter = 8; Bit_Counter; Bit_Counter--)
    {
        if (SMBus_ReceiveBit()) // Get a bit from the SDA line
        {
            RX_buffer <<= 1; // If the bit is HIGH save 1  in RX_buffer
            RX_buffer |= 0x01;
        }
        else
        {
            RX_buffer <<= 1; // If the bit is LOW save 0 in RX_buffer
            RX_buffer &= 0xfe;
        }
    }
    SMBus_SendBit(ack_nack); // Sends acknowledgment bit
    return RX_buffer;
}

void SMBus_GpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitTypeDef GPIO_InitStruct_2 = {0};

    SCK_GPIO_CLK_ENABLE();
    SDA_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = SCK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SCK_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct_2.Pin = SDA_Pin;
    GPIO_InitStruct_2.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct_2.Pull = GPIO_PULLUP;
    GPIO_InitStruct_2.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct_2);
}

/*******************************************************************************
 * Function Name  : SMBus_Init
 * Description    : SMBus3?ê??ˉ
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SMBus_Init(void)
{
    SMBus_GpioInit();
    SMBUS_SCK_H();
    SMBUS_SDA_H();
}

/*******************************************************************************
 * Function Name  : SMBus_ReadMemory
 * Description    : READ DATA FROM RAM/EEPROM
 * Input          : slaveAddress, command
 * Output         : None
 * Return         : Data
 *******************************************************************************/
u16 SMBus_ReadMemory(u8 slaveAddress, u8 command)
{
    u16 data;        // Data storage (DataH:DataL)
    u8 Pec;          // PEC byte storage
    u8 DataL = 0;    // Low data byte storage
    u8 DataH = 0;    // High data byte storage
    u8 arr[6];       // Buffer for the sent bytes
    u8 PecReg;       // Calculated PEC byte storage
    u8 ErrorCounter; // Defines the number of the attempts for communication with MLX90614

    ErrorCounter = 0x00; // Initialising of ErrorCounter
    slaveAddress <<= 1;  // 2-7??±íê?′ó?úμ??·

    do
    {
    repeat:
        SMBus_StopBit();   // If slave send NACK stop comunication
        --ErrorCounter;    // Pre-decrement ErrorCounter
        if (!ErrorCounter) // ErrorCounter=0?
        {
            break; // Yes,go out from do-while{}
        }

        SMBus_StartBit();                 // Start condition
        if (SMBus_SendByte(slaveAddress)) // Send SlaveAddress ×?μí??Wr=0±íê??ó??à′D′?üá?
        {
            goto repeat; // Repeat comunication again
        }
        if (SMBus_SendByte(command)) // Send command
        {
            goto repeat; // Repeat comunication again
        }

        SMBus_StartBit();                     // Repeated Start condition
        if (SMBus_SendByte(slaveAddress + 1)) // Send SlaveAddress ×?μí??Rd=1±íê??ó??à′?áêy?Y
        {
            goto repeat; // Repeat comunication again
        }

        DataL = SMBus_ReceiveByte(ACK); // Read low data,master must send ACK
        DataH = SMBus_ReceiveByte(ACK); // Read high data,master must send ACK
        Pec = SMBus_ReceiveByte(NACK);  // Read PEC byte, master must send NACK
        SMBus_StopBit();                // Stop condition

        arr[5] = slaveAddress;         //
        arr[4] = command;              //
        arr[3] = slaveAddress + 1;     // Load array arr
        arr[2] = DataL;                //
        arr[1] = DataH;                //
        arr[0] = 0;                    //
        PecReg = PEC_Calculation(arr); // Calculate CRC
    } while (PecReg != Pec);           // If received and calculated CRC are equal go out from do-while{}

    data = (DataH << 8) | DataL; // data=DataH:DataL
    return data;
}

/*******************************************************************************
 * Function Name  : PEC_calculation
 * Description    : Calculates the PEC of received bytes
 * Input          : pec[]
 * Output         : None
 * Return         : pec[0]-this byte contains calculated crc value
 *******************************************************************************/
u8 PEC_Calculation(u8 pec[])
{
    u8 crc[6];
    u8 BitPosition = 47;
    u8 shift;
    u8 i;
    u8 j;
    u8 temp;

    do
    {
        /*Load pattern value 0x000000000107*/
        crc[5] = 0;
        crc[4] = 0;
        crc[3] = 0;
        crc[2] = 0;
        crc[1] = 0x01;
        crc[0] = 0x07;

        /*Set maximum bit position at 47 ( six bytes byte5...byte0,MSbit=47)*/
        BitPosition = 47;

        /*Set shift position at 0*/
        shift = 0;

        /*Find first "1" in the transmited message beginning from the MSByte byte5*/
        i = 5;
        j = 0;
        while ((pec[i] & (0x80 >> j)) == 0 && i > 0)
        {
            BitPosition--;
            if (j < 7)
            {
                j++;
            }
            else
            {
                j = 0x00;
                i--;
            }
        } /*End of while */

        /*Get shift value for pattern value*/
        shift = BitPosition - 8;

        /*Shift pattern value */
        while (shift)
        {
            for (i = 5; i < 0xFF; i--)
            {
                if ((crc[i - 1] & 0x80) && (i > 0))
                {
                    temp = 1;
                }
                else
                {
                    temp = 0;
                }
                crc[i] <<= 1;
                crc[i] += temp;
            } /*End of for*/
            shift--;
        } /*End of while*/

        /*Exclusive OR between pec and crc*/
        for (i = 0; i <= 5; i++)
        {
            pec[i] ^= crc[i];
        }                      /*End of for*/
    } while (BitPosition > 8); /*End of do-while*/

    return pec[0];
}

/*******************************************************************************
 * Function Name  : SMBus_ReadTemp
 * Description    : Calculate and return the temperature
 * Input          : None
 * Output         : None
 * Return         : SMBus_ReadMemory(0x00, 0x07)*0.02-273.15
 *******************************************************************************/
float SMBus_ReadTemp(void)
{
    float temp;
    temp = SMBus_ReadMemory(SA, RAM_ACCESS | RAM_TOBJ1) * 0.02 - 273.15;
    return temp;
}