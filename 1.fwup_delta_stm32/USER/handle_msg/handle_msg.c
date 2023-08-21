#include "handle_msg.h"

uint8_t arr_msg_response[200];
static uploadValueAll_t upload_value;

static void UpdateFile(const messageFrameMsg_t datain);

void Handle_GetMsg(const messageFrameMsg_t datain)
{
	switch(datain.TypeMessage)
	{
		case TYPE_MSG_MODE_BOTLOADER:
			break;
		
		case TYPE_MSG_UPDATE_FILE:
				UpdateFile(datain);
			break;
		
		case TYPE_MSG_DELTA:
			break;
		
		case TYPE_MSG_UPDATE_FLASH:
			break;
		
		case TYPE_MSG_MODE_APP:
			break;
		
		default:
			break;
	}
}

void Handle_ResponseMsg(void)
{
	
}

static void UpdateFile(const messageFrameMsg_t datain)
{
	uploadData_t     *data_temp;
	uploadMetaData_t *meta_data_temp;

	switch(datain.Data[0])
	{
		case OTA_STATE_START:
			meta_data_temp = (uploadMetaData_t*)datain.Data;
			if(meta_data_temp->cmd == OTA_STATE_START)
			{
				upload_value.size_file_bin   = 0;
				upload_value.size_file_bin   = 0;
				upload_value.count_size_file = 0;
				
				upload_value.check_sum       = meta_data_temp->package_crc;
				upload_value.size_file_bin   = meta_data_temp->package_size;
				printf("check_sum        : %x\n", upload_value.check_sum );
				printf("size_file_bin    : %d\n", upload_value.size_file_bin);
			}
			else
			{
				upload_value.size_file_bin   = 0;
				upload_value.size_file_bin   = 0;
				upload_value.count_size_file = 0;
			}
			break;
		
		case OTA_STATE_DATA:
			
			break;
		
		case OTA_STATE_END:
			
			break;
		
		default:
			break;
	}
	
	/*
	printf("Start     : %x\n", datain.Start);
  printf("TypeMSG   : %x\n", datain.TypeMessage);
	printf("Length    : %d\n", datain.Length);
	printf("Value     : ");
	for (int i = 0; i < datain.Length - DEFAULT_BYTE_CHECKSUM; i++)
	{
		printf("%d ", datain.Data[i]);
	}
  printf("\nCheck sum : %x\n", datain.Crc);
	*/
}
