
#include "bts_frame_message.h"
#include <string.h>
#include "usart.h"
uint16_t CheckSum(uint8_t *buf, uint8_t len);

int16_t CreateMessage(uint8_t type_msg, uint16_t length, uint8_t *datain, uint8_t *dataout)
{
	uint8_t *data_frame_temp;
	uint16_t count_length_arr = 0;
	
  messageFrameMsg_t frame_message_temp;
  data_frame_temp = (uint8_t *) &frame_message_temp;
	
	if (length > DEFAULT_MAX_SIZE_DATA)
  {
#if DEBUG_ERROR
    BTS_Debug_Error("Length > DEFAULT_MAX_SIZE_DATA");
#endif
    return RETRUN_ERROR;
  }
	
  /*---------------------------------(Start)---------------------------------*/
  frame_message_temp.Start = START_BYTE;
  /*---------------------------------(TypeMessage)---------------------------------*/
  frame_message_temp.TypeMessage = type_msg;
	/*---------------------------------(Length)---------------------------------*/
  frame_message_temp.Length = length + DEFAULT_BYTE_CHECKSUM;
  /*---------------------------------(Data)---------------------------------*/
	memcpy(frame_message_temp.Data, datain, length);
	/*---------------------------------(CRC)---------------------------------*/
	count_length_arr = (DEFAULT_BYTE + (frame_message_temp.Length - DEFAULT_BYTE_CHECKSUM));
  frame_message_temp.Crc = CheckSum(data_frame_temp, count_length_arr);
	/*---------------------------------(CRC)---------------------------------*/
	memcpy(dataout, data_frame_temp, count_length_arr);
  /*Ghi giá trị checksum tính được vào cuối mảng*/
  dataout[count_length_arr]      = (frame_message_temp.Crc & 0xff);
  dataout[count_length_arr + 1]  = ((frame_message_temp.Crc >> 8) & 0xff);
  /*tăng giá trị mảng lên 2 lần vì đã thêm ở trên*/
  count_length_arr += 2;
  return count_length_arr;
}

int16_t DetectMessage(uint8_t *datain, messageFrameMsg_t *dataout)
{
	uint16_t count_length_arr = 0;
	messageFrameMsg_t *frame_temp;
	frame_temp = (messageFrameMsg_t*)datain;
	
	/*---------------------------------(Get CRC)---------------------------------*/
	frame_temp->Crc = Bts_Convert_From_Bytes_To_Uint16(frame_temp->Data[frame_temp->Length-2], frame_temp->Data[frame_temp->Length-1]);
	
	/*---------------------------------(Copy struct frame_temp -> dataout)---------------------------------*/
	dataout->Start       = frame_temp->Start;
	dataout->TypeMessage = frame_temp->TypeMessage;
	dataout->Length      = frame_temp->Length;
	memcpy(dataout->Data, frame_temp->Data, (dataout->Length-DEFAULT_BYTE_CHECKSUM));
	dataout->Crc         = frame_temp->Crc;
	
	/*---------------------------------(Calculator byte crc)---------------------------------*/
	count_length_arr = (DEFAULT_BYTE + (dataout->Length - DEFAULT_BYTE_CHECKSUM));
	
	/*---------------------------------(Check START_BYTE)---------------------------------*/
	if (dataout->Start != START_BYTE)
  {
#if DEBUG_ERROR
    BTS_Debug_Error("Start frame");
#endif
    return RETRUN_ERROR;
  }
	
	/*---------------------------------(Check CRC)---------------------------------*/
  if (dataout->Crc != (uint16_t)CheckSum(datain, count_length_arr))
  {
#if DEBUG_ERROR
    BTS_Debug_Error("Check sum");
#endif
    return RETRUN_ERROR;
  }
	
  return (count_length_arr+DEFAULT_BYTE_CHECKSUM);
}

int16_t DebugMessage(uint8_t *datain)
{
	uint8_t i = 0;
  messageFrameMsg_t dataout;
  
  printf("\n-(Size    : %d)-\n", DetectMessage(datain, &dataout));
  printf("Start     : %x\n", dataout.Start);
  printf("TypeMSG   : %x\n", dataout.TypeMessage);
	printf("Length    : %d\n", dataout.Length);
	printf("Value     : ");
	for (i = 0; i < dataout.Length - DEFAULT_BYTE_CHECKSUM; i++)
	{
		printf("%d ", dataout.Data[i]);
	}
  printf("\nCheck sum : %x\n", dataout.Crc);
  return 0;
}

uint16_t CheckSum(uint8_t *buf, uint8_t len)
{
  uint16_t crc = 0xFFFF, pos = 0, i = 0;
  for (pos = 0; pos < len; pos++)
  {
    crc ^= (uint16_t)buf[pos]; 	// XOR byte into least sig. byte of crc
    for (i = 8; i != 0; i--)   	// Loop over each bit
    {
      if ((crc & 0x0001) != 0)	// If the LSB is set
      {
        crc >>= 1; 						 	// Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else 											// Else LSB is not set
      {
        crc >>= 1; 							// Just shift right
      }
    }
  }
  return crc;
}
