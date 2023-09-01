#include "handle_msg.h"
#include "string.h"
#include "lib_sd.h"
#include "delta.h"

char path0[500];
#define FILE_OLD    "demo/blinky-k64f-old.bin"
#define FILE_PATCH  "demo/blinky-k64f.patch"
#define FILE_CREATE "demo/blinky5.bin"

static uint32_t check_sum;
char update_name_files[50] = "";
static uint32_t update_size_file_bin;
static uint32_t update_count_size_file = 0;
static uint32_t update_check_sum = 0xFFFFFFFFUL;
responseValueAll_t  response_value;

static void UpdateFile(const messageFrameMsg_t datain, void(*Uart_SendByte)(char));
static void UpdateResponseMsg(uint8_t cmd,  void(*Uart_SendByte)(char));
static void DeltaResponseMsg(uint8_t  cmd,  void(*Uart_SendByte)(char));
static uint32_t crc32(unsigned long crc, const unsigned char *data, size_t length);

static void Handle_Delay(uint8_t times);

void Handle_InitValueAll(void)
{
  update_size_file_bin   = 0;
  update_count_size_file = 0;

  response_value.flag            = 0;
  response_value.count_time_send = 0;
}

void Handle_GetMsg(const messageFrameMsg_t datain, void(*Uart_SendByte)(char))
{
	deltaDiff_t *delta_data;
  switch (datain.TypeMessage)
  {
    case TYPE_MSG_MODE_BOTLOADER:

      break;

    case TYPE_MSG_UPDATE_FILE:	
      UpdateFile(datain,Uart_SendByte);
      break;

    case TYPE_MSG_DELTA:
			delta_data = (deltaDiff_t*)datain.Data;
			f_unlink(delta_data->name_create);
			Delta_Run(delta_data->name_old, delta_data->name_patch, delta_data->name_create);
			printf("Kich thuoc file %s la %ld bytes\n", delta_data->name_create, SD_getFileSize(delta_data->name_create));
			//Handle_Delay(5);
			DeltaResponseMsg(1, Uart_SendByte);
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

static void UpdateFile(const messageFrameMsg_t datain, void(*Uart_SendByte)(char))
{
  uploadData_t       *data_temp;
  uploadMetaData_t   *meta_data_temp;
  uploadDeleteFile_t *delete_file;
  switch (datain.Data[0])
  {
    case OTA_STATE_START:
      meta_data_temp = (uploadMetaData_t*)datain.Data;
			update_size_file_bin   = 0;
			update_size_file_bin   = 0;
			update_count_size_file = 0;
			update_check_sum = 0xFFFFFFFFUL;
			
			for(int i=0; i<sizeof(update_name_files); i++)
			{
				update_name_files[i] = 0;
			}
			
			check_sum = meta_data_temp->package_crc;
			update_size_file_bin   = meta_data_temp->package_size;
			memcpy(update_name_files, meta_data_temp->name, strlen(meta_data_temp->name));
			
			printf("string len    : %d\n", strlen(meta_data_temp->name));
			printf("name          : %s\n", update_name_files);
			printf("check_sum     : %x\n", check_sum );
			printf("size          : %d\n", update_size_file_bin);
			f_unlink(update_name_files);
			UpdateResponseMsg(OTA_STATE_START, Uart_SendByte); 
      break;

    case OTA_STATE_DATA:
			data_temp = (uploadData_t*)datain.Data;
			printf("cmd     : %x\n", data_temp->cmd);
			printf("length  : %d\n", data_temp->length);
			printf("offset  : %d\n", data_temp->offset);
			
			update_check_sum = crc32(update_check_sum, data_temp->data, data_temp->length);
			SD_WriteFile((char*)update_name_files, data_temp->data, data_temp->length, data_temp->offset);
			printf("Kich thuoc file %s la %ld bytes\n", update_name_files, SD_getFileSize(update_name_files));
			

			if(update_count_size_file == data_temp->offset)
			{
				printf("compe: %d - %d!\n", update_count_size_file, data_temp->offset);
				update_count_size_file += data_temp->length;
			}
			
			if (update_count_size_file >= update_size_file_bin)
			{
				update_check_sum ^= 0xFFFFFFFFUL;
				printf("Check_Sum: %x\n", update_check_sum);
				if(update_check_sum == check_sum)
				{
					printf("Done file: %d!\n", update_count_size_file);
				}
			}
			else
			{
				printf("Done: %d!\n", update_count_size_file);
			}
			//Handle_Delay(2);
			UpdateResponseMsg(OTA_STATE_DATA, Uart_SendByte); 
			
      break;

    case OTA_STATE_END:
      check_sum              = 0;
      update_size_file_bin   = 0;
      update_count_size_file = 0;
      break;

    case OTA_STATE_DELETE:
      delete_file = (uploadDeleteFile_t*)datain.Data;
      if (delete_file->cmd == OTA_STATE_DELETE)
      {
        f_unlink(delete_file->name);
        SD_ScanFiles(path0);
      }
      break;
  }
}

static void UpdateResponseMsg(uint8_t cmd,  void(*Uart_SendByte)(char))
{
	uint8_t arr[2], length_arr;
	arr[0]= cmd;
	length_arr = CreateMessage(TYPE_MSG_UPDATE_FILE,1, arr, arr);
	for(int i=0; i<length_arr; i++)
	{
		Uart_SendByte(arr[i]);
	}
}

static void DeltaResponseMsg(uint8_t  cmd,  void(*Uart_SendByte)(char))
{
	uint8_t arr[2], length_arr;
	arr[0]= cmd;
	length_arr = CreateMessage(TYPE_MSG_DELTA, 1, arr, arr);
	for(int i=0; i<length_arr; i++)
	{
		Uart_SendByte(arr[i]);
	}
}

uint32_t crc32(unsigned long crc, const unsigned char *data, size_t length)
{
    for (size_t i = 0; i < length; i++) 
    {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) 
        {
            crc = (crc >> 1) ^ ((crc & 1) ? POLYNOMIAL : 0);
        }
    }
    return crc;
}

static void Handle_Delay(uint8_t times)
{
	for(int i=0; i<times; i++)
	{
		Fn_DELAY_ms(1000);
	}
}