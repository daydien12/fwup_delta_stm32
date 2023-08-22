#include "handle_msg.h"
#include "string.h"

static uploadValueAll_t 	 upload_value;
static responseValueAll_t  response_value;


static void UpdateFile(const messageFrameMsg_t datain);

void Handle_InitValueAll(void)
{
	
	upload_value.size_file_bin   = 0;
	upload_value.size_file_bin   = 0;
	upload_value.count_size_file = 0;
	memcpy(upload_value.Name, "", sizeof(upload_value.Name));
	
	response_value.flag = 0;
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
  uploadData_t     *data_temp;
  uploadMetaData_t *meta_data_temp;

  switch (datain.Data[0])
  {
    case OTA_STATE_START:
      meta_data_temp = (uploadMetaData_t*)datain.Data;
      if (meta_data_temp->cmd == OTA_STATE_START)
      {
        upload_value.size_file_bin   = 0;
        upload_value.size_file_bin   = 0;
        upload_value.count_size_file = 0;
        memcpy(upload_value.Name, "", sizeof(upload_value.Name));

        upload_value.check_sum       = meta_data_temp->package_crc;
        upload_value.size_file_bin   = meta_data_temp->package_size;
        memcpy(upload_value.Name, meta_data_temp->name, strlen(meta_data_temp->name));
				
        printf("string len    : %d\n", strlen(meta_data_temp->name));
        printf("Name          : %s\n", upload_value.Name);
        printf("check_sum     : %x\n", upload_value.check_sum );
        printf("size_file_bin : %d\n", upload_value.size_file_bin);
      }
      else
      {
        upload_value.size_file_bin   = 0;
        upload_value.size_file_bin   = 0;
        upload_value.count_size_file = 0;
        memcpy(upload_value.Name, "", sizeof(upload_value.Name));
      }
      break;

    case OTA_STATE_DATA:
      data_temp = (uploadData_t*)datain.Data;
      if (data_temp->cmd == OTA_STATE_DATA)
      {
        printf("cmd     : %x\n", data_temp->cmd);
        printf("length  : %d\n", data_temp->length);
        printf("offset  : %d\n", data_temp->offset);
        printf("data    : %x\n", data_temp->data[0]);

        upload_value.count_size_file += data_temp->length;
        if (upload_value.count_size_file >= upload_value.size_file_bin)
        {
          printf("Done file!");
        }
        else
        {
          printf("Done!");
        }
      }
      break;

    case OTA_STATE_END:
      upload_value.check_sum       = 0;
      upload_value.size_file_bin   = 0;
      upload_value.count_size_file = 0;
      memcpy(upload_value.Name, "", sizeof(upload_value.Name));
      break;

    case OTA_STATE_DELETE:

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
