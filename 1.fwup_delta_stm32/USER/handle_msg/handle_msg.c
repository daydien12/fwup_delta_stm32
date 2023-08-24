#include "handle_msg.h"
#include "string.h"
#include "lib_sd.h"
#include "delta.h"

char path0[500];

static uint32_t check_sum;
 char update_name_files[50] = "";
static uint32_t update_size_file_bin;
static uint32_t update_count_size_file;

responseValueAll_t  response_value;

static void UpdateFile(const messageFrameMsg_t datain);

void Handle_InitValueAll(void)
{
  update_size_file_bin   = 0;
  update_count_size_file = 0;

  response_value.flag            = 0;
  response_value.count_time_send = 0;
}

void Handle_GetMsg(const messageFrameMsg_t datain)
{
  switch (datain.TypeMessage)
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

      break;

    case OTA_STATE_DATA:
			data_temp = (uploadData_t*)datain.Data;
			printf("cmd     : %x\n", data_temp->cmd);
			printf("length  : %d\n", data_temp->length);
			printf("offset  : %d\n", data_temp->offset);

			
			SD_WriteFile((char*)update_name_files, data_temp->data, data_temp->length, data_temp->offset);
			printf("Kich thuoc file %s la %ld bytes\n", update_name_files, SD_getFileSize(update_name_files));
		
			if(update_count_size_file == data_temp->offset)
			{
				update_count_size_file += data_temp->length;
			}
			if (update_count_size_file >= update_size_file_bin)
			{
				printf("Done file: %d!\n", update_count_size_file);
			}
			else
			{
				printf("Done: %d!\n", update_count_size_file);
			}
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
