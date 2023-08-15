
#include "bts_frame_message.h"


uint16_t CheckSum(uint8_t *buf, uint8_t len);


/**
 * @brief : Create device update frame array data from array of device data
 * 
 * @param datain : Array of device data
 * @param dataout : Device frame array data
 * @return int16_t : Size of device update frame array data
 */
int16_t BTS_Frame_Creat_Device(uint8_t *datain, uint8_t *dataout)
{
	uint16_t count_arr = 0;
	uint8_t *data_sensor_temp;
	messageFrameMsg_t frame_message_temp;
	data_sensor_temp =(uint8_t *) &frame_message_temp;
	/*---------------------------------(Start)---------------------------------*/
	frame_message_temp.Start = START_BYTE;
	/*---------------------------------(TypeMessage)---------------------------------*/
	frame_message_temp.TypeMessage = TYPE_MESSAGE_UPDATE_DEVICE;
	/*---------------------------------(Data)---------------------------------*/
	for(count_arr = 0; count_arr < DEFAULT_MAX_NUMBER_DEVICE; count_arr++)
    {
		frame_message_temp.Data[count_arr] = datain[count_arr];
    }
	/*---------------------------------(Data)---------------------------------*/
	frame_message_temp.Length =  DEFAULT_MAX_NUMBER_DEVICE + DEFAULT_BYTE_CHECKSUM;
	
	frame_message_temp.Crc = CheckSum(data_sensor_temp, (DEFAULT_BYTE + (frame_message_temp.Length - DEFAULT_BYTE_CHECKSUM)));
    for (count_arr = 0; count_arr < (DEFAULT_BYTE + frame_message_temp.Length - DEFAULT_BYTE_CHECKSUM); count_arr++)
    {
        dataout[count_arr] = data_sensor_temp[count_arr];
    }
    /*Ghi giá trị checksum tính được vào cuối mảng*/
    dataout[count_arr]      = (frame_message_temp.Crc & 0xff);
    dataout[count_arr + 1]  = ((frame_message_temp.Crc >> 8) & 0xff);
    /*tăng giá trị mảng lên 2 lần vì đã thêm ở trên*/
    count_arr += 2;
    return count_arr;
}


/**
 * @brief : Detect of array data received from serial port in to the frame struct data of message
 * 
 * @param datain : Array data received 
 * @param dataout : Frame struct data of message received
 * @return int16_t : Return -1 is error, otherwise return size data is success detect
 */
int16_t DetectMessage(uint8_t *datain, messageFrameMsg_t *dataout)
{
    uint8_t count_temp = 0, count_array_data = 0;
	uint16_t check_sum = 0;

    /*---------------------------------(Start (2byte))---------------------------------*/
    dataout->Start = Bts_Convert_From_Bytes_To_Uint16(datain[count_temp], datain[count_temp+1]);
    if(dataout->Start != START_BYTE)
    {
#if DEBUG_ERROR
        BTS_Debug_Error("Start frame");    
#endif
        return RETRUN_ERROR;
    }
    count_temp += 2;

    /*---------------------------------(Type Message (2byte))---------------------------------*/
    dataout->TypeMessage = Bts_Convert_From_Bytes_To_Uint16(datain[count_temp], datain[count_temp+1]);
    count_temp += 2;
    switch (dataout->TypeMessage)
    {
        case TYPE_MESSAGE_UPDATE_SENSOR:
        case TYPE_MESSAGE_UPDATE_DEVICE:
    /*---------------------------------(Length Udate (2byte))---------------------------------*/
            dataout->Length =  Bts_Convert_From_Bytes_To_Uint16(datain[count_temp], datain[count_temp+1]);
            count_temp += 2;

    /*---------------------------------(Data (40byte))---------------------------------*/
            for(count_array_data = 0; count_array_data < (dataout->Length - 2); count_array_data++)
            {
                dataout->Data[count_array_data] = datain[count_temp];
                count_temp ++;
            }
        break;

        case TYPE_MESSAGE_CONTROL_DEVICE:
    /*---------------------------------(Length Udate (2byte))---------------------------------*/
            dataout->Length =  Bts_Convert_From_Bytes_To_Uint16(datain[count_temp], datain[count_temp+1]);
            count_temp += 2;

    /*---------------------------------(Data (2byte))---------------------------------*/
            for(count_array_data = 0; count_array_data < (dataout->Length - 2); count_array_data++)
            {
                if(count_array_data <= 1)
                {
                    dataout->Data[count_array_data] = datain[count_temp];
                }
                else
                {
                    dataout->Data[count_array_data] = 0x00;
                }
                count_temp ++;
            }
        break;
        default:
#if DEBUG_ERROR
            BTS_Debug_Error("Type Message");    
#endif
            return RETRUN_ERROR;
    }
    /*---------------------------------(Check Sum(2byte))---------------------------------*/
    check_sum = CheckSum(datain, (DEFAULT_BYTE + (dataout->Length - DEFAULT_BYTE_CHECKSUM)));
    dataout->Crc = Bts_Convert_From_Bytes_To_Uint16(datain[count_temp], datain[count_temp+1]);

    if(check_sum != dataout->Crc)
    {
#if DEBUG_ERROR
        BTS_Debug_Error("Check sum");    
#endif
        return RETRUN_ERROR;
    }
    count_temp += 2;
    return count_temp;
}

/**
 * @brief : Debug the data received from serial port
 * 
 * @param datain : Data received from serial port
 * @return int16_t : Return 0 if seccess
 */
int16_t DebugMessage(uint8_t *datain)
{
    messageFrameMsg_t dataout;
	uint8_t count_array_data = 0;
	uint16_t count_arr = 0;
	uint16_t size_data;
    size_data = DetectMessage(datain, &dataout);
    printf("\n-(Size: %d)-\n", size_data);
    printf("Star	: %x\n", dataout.Start);
    printf("Type	: %x\n", dataout.TypeMessage);
    if(dataout.TypeMessage == TYPE_MESSAGE_UPDATE_SENSOR)
    {
		printf("Value : ");
        for(count_array_data = 0; count_array_data < ((size_data - 8)/4); count_array_data++)
        {
            
            printf("%d ", (int)(Bts_Convert_From_Bytes_To_Float(dataout.Data[count_arr], dataout.Data[count_arr+1], dataout.Data[count_arr+2], dataout.Data[count_arr+3])*100));
            count_arr += 4;
        }
        printf("\n");
    }
    else if (dataout.TypeMessage == TYPE_MESSAGE_UPDATE_DEVICE)
    {
		printf("Value : ");
        for(count_array_data = 0; count_array_data < ((size_data - 8)); count_array_data++)
        {
            
            printf("%d ", (dataout.Data[count_array_data]));
        }
        printf("\n");
    }
    else
    {
        printf("Name DV : %d\n", dataout.Data[0]);
        printf("Value : %d\n", dataout.Data[1]);
    }
    printf("Check sum : %x\n", dataout.Crc);
	return 0;
}

/**
 * @brief : Creat the checksum of the message
 * 
 * @param buf : Data of message
 * @param len : Length of message
 * @return uint16_t : Checksum of message
 */
uint16_t CheckSum(uint8_t *buf, uint8_t len)
{
	uint16_t crc = 0xFFFF, pos = 0, i = 0;
	for (pos = 0; pos < len; pos++)
	{
		crc ^= (uint16_t)buf[pos]; // XOR byte into least sig. byte of crc
		for (i = 8; i != 0; i--)   // Loop over each bit
		{
			if ((crc & 0x0001) != 0) // If the LSB is set
			{
				crc >>= 1; // Shift right and XOR 0xA001
				crc ^= 0xA001;
			}
			else // Else LSB is not set
			{
				crc >>= 1; // Just shift right
			}
		}
	}
	return crc;
}
