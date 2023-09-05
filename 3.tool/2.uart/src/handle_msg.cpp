#include "handle_msg.h"

uint32_t UpdateFile_CreateFrameMetaData(const char *name_file_bin, const char *name_file, uint8_t *dataout, uint32_t &size_file);
uint32_t UpdateFile_CreateFrameData(const char *name_file_bin, const uint32_t length, const uint32_t offset, uint8_t *dataout);
uint32_t UpdateFile_CreateFrameEnd(uint8_t *dataout);

uint32_t HandleMsg::Handle_UpdateFileRun(const char *name_file_bin, const char *port, const char *name_file, uint8_t &state, uint32_t &offset, uint32_t &size_file_bin)
{
  uint8_t arr[150], size_arr = 0;
  uint32_t length_send = 0;

  SerialPort uart(port, 9600, 1);
  switch (state)
  {
  case OTA_STATE_START:
    offset = 0;
    size_arr = UpdateFile_CreateFrameMetaData(name_file_bin, name_file, arr, size_file_bin);
    uart.writebyte(arr, size_arr);
    break;

  case OTA_STATE_DATA:
    if ((size_file_bin - offset) > LENGTH_SEND)
    {
      size_arr = UpdateFile_CreateFrameData(name_file_bin, LENGTH_SEND, offset, arr);
    }
    else
    {
      size_arr = UpdateFile_CreateFrameData(name_file_bin, (size_file_bin - offset), offset, arr);
    }
    printf("Send: %d-%d\n", size_arr, offset);
    uart.writebyte(arr, size_arr);
    break;

  case OTA_STATE_END:
    size_arr = UpdateFile_CreateFrameEnd(arr);
    printf("Send: %d\n", size_arr);
    uart.writebyte(arr, size_arr);
    break;

  case OTA_STATE_NULL:
    printf("Null\n");
    break;

  default:
    break;
  }
  uart.~SerialPort();
  return 1;
}

uint32_t HandleMsg::Handle_UpdateFileState(const uint8_t *datain, uint8_t &state, uint32_t &offset, uint32_t &size_file_bin)
{
  uploadResponse_t *data_temp = (uploadResponse_t *)datain;
  printf("Get1: cmd: %d - offset: %d - size_file_bin: %d\n", data_temp->cmd, data_temp->offset, size_file_bin);
  switch (state)
  {
  case OTA_STATE_START:
    if ((data_temp->cmd == OTA_STATE_START) && (data_temp->offset == size_file_bin))
    {
      state = OTA_STATE_DATA;
    }
    break;

  case OTA_STATE_DATA:
    if ((data_temp->cmd == OTA_STATE_DATA) && (data_temp->offset == offset))
    {
      if ((size_file_bin - offset) > LENGTH_SEND)
      {
        offset += LENGTH_SEND;
      }
      else if ((size_file_bin - offset) < LENGTH_SEND)
      {
        if (data_temp->offset != size_file_bin)
        {
          offset += (size_file_bin - offset);
        }
        else
        {
          state = OTA_STATE_END;
        }
      }
    }
    break;

  case OTA_STATE_END:
    if ((data_temp->cmd == OTA_STATE_END) && (data_temp->offset == 1))
    {
      state = OTA_STATE_NULL;
    }
    else
    {
      state = OTA_STATE_START;
    }
    break;

  case OTA_STATE_NULL:
    printf("Null\n");
    return 1;
    break;

  default:
    break;
  }
  return 0;
}

uint32_t HandleMsg::Handle_DeltaUpdate(const char *port, const char *name_old, const char *name_patch, const char *name_create)
{
  SerialPort uart(port, 9600, 1);
  uint8_t sizearr = 0, data_out[150];
  uint8_t lenth_old = strlen(name_old);
  uint8_t lenth_patch = strlen(name_patch);
  uint8_t lenth_create = strlen(name_create);

  FrameMessage framemessage;
  deltaDiff_t *delta_data, delta_data_temp;
  memcpy(delta_data_temp.name_old, name_old, lenth_old);
  memcpy(delta_data_temp.name_patch, name_patch, lenth_patch);
  memcpy(delta_data_temp.name_create, name_create, lenth_create);

  delta_data_temp.name_old[lenth_old] = 0;
  delta_data_temp.name_patch[lenth_patch] = 0;
  delta_data_temp.name_create[lenth_create] = 0;

  delta_data = &delta_data_temp;
  sizearr = framemessage.CreateMessage(TYPE_MSG_DELTA, sizeof(deltaDiff_t), (uint8_t *)delta_data, data_out);
  uart.writebyte(data_out, sizearr);
  uart.~SerialPort();
  return sizearr;
}

uint32_t HandleMsg::Handle_FlashUpdate(const char *port, const char *name_file, const uint32_t flash_address)
{
  SerialPort uart(port, 9600, 1);
  uint8_t sizearr = 0, data_out[150];
  uint8_t length_name_file = strlen(name_file);

  FrameMessage framemessage;
  flashUpdate_t *flash_data, flash_data_temp;
  memcpy(flash_data_temp.name_file, name_file, length_name_file);

  flash_data_temp.name_file[length_name_file] = 0;
  flash_data_temp.flash_app_address = flash_address;

  flash_data = &flash_data_temp;
  sizearr = framemessage.CreateMessage(TYPE_MSG_UPDATE_FLASH, sizeof(flashUpdate_t), (uint8_t *)flash_data, data_out);
  uart.writebyte(data_out, sizearr);
  uart.~SerialPort();
  return sizearr;
}

uint32_t HandleMsg::Handle_ModeApp(const char *port, const uint32_t flash_address)
{
  SerialPort uart(port, 9600, 1);
  uint8_t sizearr = 0, data_out[150];

  FrameMessage framemessage;
  appModeUpdate_t *app_data, app_data_temp;
  app_data_temp.flash_app_address = flash_address;

  app_data = &app_data_temp;
  sizearr = framemessage.CreateMessage(TYPE_MSG_MODE_APP, sizeof(appModeUpdate_t), (uint8_t *)app_data, data_out);
  uart.writebyte(data_out, sizearr);
  uart.~SerialPort();
  return sizearr;
}

uint32_t HandleMsg::Handle_ModeBootloader(const char *port)
{
  char *name_file = "bootloadr\n";
  SerialPort uart("/dev/ttyUSB0", 9600, 1);
  uart.writebyte((uint8_t *)name_file, strlen(name_file));
  return strlen(name_file);
}


uint32_t HandleMsg::Handle_Null(void)
{
  SerialPort uart("/dev/ttyUSB0", 9600, 1);
  uart.writebyte((uint8_t *)"xin chao\n", strlen("xin chao\n"));
  return strlen("xin chao\n");
}

uint32_t UpdateFile_CreateFrameMetaData(const char *name_file_bin, const char *name_file, uint8_t *dataout, uint32_t &size_file)
{
  uint8_t sizearr = 0, length_name = strlen(name_file) + 1;
  FrameMessage framemessage;
  Files file(name_file_bin, std::ios::in | std::ios::binary);
  uploadMetaData_t *meta_data, meta_data_temp;
  size_file = file.Files_GetSizeFile();
  meta_data_temp.cmd = OTA_STATE_START;
  meta_data_temp.package_crc = file.Files_CalculatorChecksum();
  meta_data_temp.package_size = size_file;
  printf("size %d\n", file.Files_GetSizeFile());
  memcpy(meta_data_temp.name, name_file, length_name);
  meta_data = &meta_data_temp;
  sizearr = framemessage.CreateMessage(TYPE_MSG_UPDATE_FILE, (9 + length_name), (uint8_t *)meta_data, dataout);
  file.Files_Clear();
  return sizearr;
}

uint32_t UpdateFile_CreateFrameData(const char *name_file_bin, const uint32_t length, const uint32_t offset, uint8_t *dataout)
{
  uint8_t lengths = 0;
  FrameMessage framemessage;
  Files file(name_file_bin, std::ios::in | std::ios::binary);
  uploadData_t *data, data_temp;
  data_temp.cmd = OTA_STATE_DATA;
  data_temp.length = length;
  data_temp.offset = offset;
  file.Files_GetArrFile(length, offset, data_temp.data);
  data = &data_temp;
  lengths = framemessage.CreateMessage(TYPE_MSG_UPDATE_FILE, sizeof(uploadData_t) - (128 - length), (uint8_t *)data, dataout);
  file.Files_Clear();
  return lengths;
}

uint32_t UpdateFile_CreateFrameEnd(uint8_t *dataout)
{
  uint8_t sizearr = 0;
  uint8_t arr[2] = {OTA_STATE_END, 0};
  FrameMessage framemessage;
  sizearr = framemessage.CreateMessage(TYPE_MSG_UPDATE_FILE, 1, arr, dataout);
  return sizearr;
}