#include "handle_msg.h"
#include "string.h"
#include "lib_sd.h"
#include "delta.h"
#include "flash_if.h"

char path0[500];
#define FILE_OLD    "demo/blinky-k64f-old.bin"
#define FILE_PATCH  "demo/blinky-k64f.patch"
#define FILE_CREATE "demo/blinky5.bin"

static uint32_t check_sum;
char update_name_files[50] = "";
static uint32_t update_size_loop_bin;
static uint32_t update_count_size_loop = 0;
static uint32_t update_check_sum = 0xFFFFFFFFUL;
flashValueAll_t flash_value;
responseValueAll_t  response_value;

typedef void (*iapfun)(void);
void iap_load_app(u32 appxaddr);
iapfun jump2app;

void iap_load_app(uint32_t appxaddr)
{
  int data = (*(volatile uint32_t*)appxaddr);
  jump2app = (iapfun) * (volatile uint32_t*)(appxaddr + 4);
  __set_MSP(*(volatile uint32_t*)appxaddr);
  jump2app();
}


static void UpdateFile(const messageFrameMsg_t datain, void(*Uart_SendByte)(char));
static void UpdateResponseMsg(uint8_t cmd, uint32_t offset, void(*Uart_SendByte)(char));
static void DeltaResponseMsg(uint8_t  cmd, void(*Uart_SendByte)(char));
static void FlashResponseMsg(uint8_t  cmd, void(*Uart_SendByte)(char));
static void AppModeResponseMsg(uint8_t  cmd, void(*Uart_SendByte)(char));

static uint32_t crc32(unsigned long crc, const unsigned char *data, size_t length);

static void Handle_Delay(uint8_t times);

void Handle_InitValueAll(void)
{
  update_size_loop_bin   = 0;
  update_count_size_loop = 0;

  response_value.flag            = 0;
  response_value.count_time_send = 0;

  flash_value.offset = 0;
  flash_value.size_file_bin = 0;
  flash_value.count_SizeFileBin = FLASH_APP_ADDR;
}

void Handle_GetMsg(const messageFrameMsg_t datain, void(*Uart_SendByte)(char))
{
  deltaDiff_t *delta_data;
  flashUpdate_t *flash_data;
  appModeUpdate_t *app_data;
	
  switch (datain.TypeMessage)
  {
    case TYPE_MSG_MODE_BOTLOADER:
				
      break;

    case TYPE_MSG_UPDATE_FILE:
      UpdateFile(datain, Uart_SendByte);
      break;

    case TYPE_MSG_DELTA:
      delta_data = (deltaDiff_t*)datain.Data;
      f_unlink(delta_data->name_create);
      Delta_Run(delta_data->name_old, delta_data->name_patch, delta_data->name_create);
      printf("Kich thuoc file %s la %ld bytes\n", delta_data->name_create, SD_getFileSize(delta_data->name_create));
      DeltaResponseMsg(1, Uart_SendByte);
      break;

    case TYPE_MSG_UPDATE_FLASH:
      flash_data = (flashUpdate_t*)datain.Data;
      SD_Mount();
      flash_value.offset = 0;
      flash_value.size_file_bin = 0;
      flash_value.count_SizeFileBin = flash_data->flash_app_address;
      printf("Name: %s - address: %x\n", flash_data->name_file, flash_data->flash_app_address);

      flash_value.size_file_bin =  SD_getFileSize(flash_data->name_file);
      printf("Kich thuoc file %s la %d bytes\n", flash_data->name_file, flash_value.size_file_bin);

      FLASH_If_Init();
      FLASH_If_Erase(flash_value.count_SizeFileBin);

      while (flash_value.size_file_bin > flash_value.offset)
      {
        uint32_t *temp_data;
        uint8_t arr_data[128];
        if ((flash_value.size_file_bin - flash_value.offset) > 128)
        {
          flash_value.offset += SD_ReadFile(flash_data->name_file, arr_data, 128, flash_value.offset);
        }
        else
        {
          for (int j = 0; j < 128; j++)
          {
            arr_data[j] = 0;
          }
          flash_value.offset += SD_ReadFile(flash_data->name_file, arr_data, (flash_value.size_file_bin - flash_value.offset), flash_value.offset);
        }
        temp_data = (uint32_t*)arr_data;
        FLASH_If_Write(&flash_value.count_SizeFileBin, (uint32_t*)temp_data, (uint32_t)128 / 4);
      }

      printf("offset: %d\n", flash_value.offset);
      FLASH_If_Finish();
      FlashResponseMsg(1, Uart_SendByte);
      //Fn_DELAY_ms(100);
      //iap_load_app(FLASH_APP_ADDR);
      break;

    case TYPE_MSG_MODE_APP:
			app_data = (appModeUpdate_t*)datain.Data;
			printf("Address: %x\n",app_data->flash_app_address);
			AppModeResponseMsg(1, Uart_SendByte);
			Fn_DELAY_ms(100);
      iap_load_app(FLASH_APP_ADDR);
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
      update_size_loop_bin   = 0;
      update_size_loop_bin   = 0;
      update_count_size_loop = 0;
      update_check_sum = 0xFFFFFFFFUL;

      for (int i = 0; i < sizeof(update_name_files); i++)
      {
        update_name_files[i] = 0;
      }

      check_sum = meta_data_temp->package_crc;
      update_size_loop_bin   = meta_data_temp->package_size;
      memcpy(update_name_files, meta_data_temp->name, strlen(meta_data_temp->name));

      printf("string len    : %d\n", strlen(meta_data_temp->name));
      printf("name          : %s\n", update_name_files);
      printf("check_sum     : %x\n", check_sum );
      printf("size          : %d\n", update_size_loop_bin);
      f_unlink(update_name_files);
      UpdateResponseMsg(OTA_STATE_START, update_size_loop_bin, Uart_SendByte);
      break;

    case OTA_STATE_DATA:
      data_temp = (uploadData_t*)datain.Data;
      printf("cmd     : %x\n", data_temp->cmd);
      printf("length  : %d\n", data_temp->length);
      printf("offset  : %d\n", data_temp->offset);

      if (update_count_size_loop == data_temp->offset)
      {
        printf("compe: %d - %d!\n", update_count_size_loop, data_temp->offset);
        update_check_sum = crc32(update_check_sum, data_temp->data, data_temp->length);
        SD_WriteFile((char*)update_name_files, data_temp->data, data_temp->length, data_temp->offset);
        printf("Kich thuoc file %s la %ld bytes\n", update_name_files, SD_getFileSize(update_name_files));
        update_count_size_loop += data_temp->length;
      }

      if ((update_count_size_loop >= update_size_loop_bin) && (data_temp->offset == update_size_loop_bin))
      {
        update_check_sum ^= 0xFFFFFFFFUL;
        printf("Check_Sum: %x\n", update_check_sum);
        if (update_check_sum == check_sum)
        {
          printf("Done file: %d!\n", update_count_size_loop);
        }
        UpdateResponseMsg(OTA_STATE_DATA, data_temp->offset, Uart_SendByte);
      }
      else
      {
        printf("Done: %d!\n", update_count_size_loop);
        UpdateResponseMsg(OTA_STATE_DATA, data_temp->offset, Uart_SendByte);
      }
      break;

    case OTA_STATE_END:
      printf("cmd     : %x\n", datain.Data[0]);
      if (update_count_size_loop >= update_size_loop_bin)
      {
        check_sum              = 0;
        update_size_loop_bin   = 0;
        update_count_size_loop = 0;
        UpdateResponseMsg(OTA_STATE_END, 1, Uart_SendByte);
      }
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

static void UpdateResponseMsg(uint8_t cmd, uint32_t offset, void(*Uart_SendByte)(char))
{
  uint8_t arr[20], length_arr;
  uploadResponse_t data, *data_temp;
  data.cmd = cmd;
  data.offset = offset;
  data_temp = &data;
  length_arr = CreateMessage(TYPE_MSG_UPDATE_FILE, sizeof(uploadResponse_t), (uint8_t*)data_temp, arr);
  for (int i = 0; i < length_arr; i++)
  {
    Uart_SendByte(arr[i]);
  }
}

static void DeltaResponseMsg(uint8_t cmd,  void(*Uart_SendByte)(char))
{
  uint8_t arr[2], arr_out[10], length_arr;
  arr[0] = cmd;
  length_arr = CreateMessage(TYPE_MSG_DELTA, 1, arr, arr_out);
  for (int i = 0; i < length_arr; i++)
  {
    Uart_SendByte(arr_out[i]);
  }
}

static void FlashResponseMsg(uint8_t cmd, void(*Uart_SendByte)(char))
{
  uint8_t arr[2], arr_out[10], length_arr;
  arr[0] = cmd;
  length_arr = CreateMessage(TYPE_MSG_UPDATE_FLASH, 1, arr, arr_out);
  for (int i = 0; i < length_arr; i++)
  {
    Uart_SendByte(arr_out[i]);
  }
}

static void AppModeResponseMsg(uint8_t  cmd, void(*Uart_SendByte)(char))
{
	uint8_t arr[2], arr_out[10], length_arr;
  arr[0] = cmd;
  length_arr = CreateMessage(TYPE_MSG_MODE_APP, 1, arr, arr_out);
  for (int i = 0; i < length_arr; i++)
  {
    Uart_SendByte(arr_out[i]);
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
  for (int i = 0; i < times; i++)
  {
    Fn_DELAY_ms(1000);
  }
}