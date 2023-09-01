#include "handle_msg.h"

uint32_t UpdateFile_CreateFrameMetaData(const char *name_file_bin, const char *name_file, uint8_t *dataout, uint32_t &size_file);
uint32_t UpdateFile_CreateFrameData(const char *name_file_bin, const uint32_t length, const uint32_t offset, uint8_t *dataout);

uint32_t HandleMsg::Handle_UpdateFile(const char *name_file_bin, const char *port, const uint8_t mode, const char *name_file, uint8_t &state,  uint32_t &offset, uint32_t &size_file_bin)
{
  uint8_t arr[150], size_arr = 0;
  uint32_t length_send = 0;

  SerialPort   uart(port, 9600, 1);
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
      if(mode == 1)
      {
        offset += size_arr - 12;
      }
     break;

    case OTA_STATE_END:
      
     break;
  

    default:
      break;
  }
  uart.~SerialPort();
  return 1;
}

uint32_t HandleMsg::Handle_DeltaUpdate(const char *port, const char *name_old, const char *name_patch, const char *name_create)
{
  SerialPort   uart(port, 9600, 1);
  uint8_t sizearr = 0, data_out[150];
  uint8_t lenth_old = strlen(name_old);
  uint8_t lenth_patch = strlen(name_patch);
  uint8_t lenth_create = strlen(name_create);

  FrameMessage framemessage;
  deltaDiff_t *delta_data, delta_data_temp;
  memcpy(delta_data_temp.name_old, name_old, lenth_old);
  memcpy(delta_data_temp.name_patch, name_patch, lenth_patch);
  memcpy(delta_data_temp.name_create, name_create, lenth_create);

  delta_data_temp.name_old[lenth_old+1] = 0;
  delta_data_temp.name_patch[lenth_patch] = 0;
  delta_data_temp.name_create[lenth_create] = 0;

  delta_data = &delta_data_temp;
  sizearr = framemessage.CreateMessage(TYPE_MSG_DELTA,sizeof(deltaDiff_t), (uint8_t*)delta_data, data_out);
  uart.writebyte(data_out, sizearr);
  uart.~SerialPort();
  return sizearr;
}

uint32_t UpdateFile_CreateFrameMetaData(const char *name_file_bin, const char *name_file, uint8_t *dataout, uint32_t &size_file)
{
  uint8_t sizearr = 0, length_name = strlen(name_file) + 1;
  FrameMessage framemessage;
  Files file(name_file_bin, std::ios::in | std::ios::binary);
  uploadMetaData_t *meta_data, meta_data_temp;
  size_file = file.Files_GetSizeFile();
  meta_data_temp.cmd          = OTA_STATE_START;
  meta_data_temp.package_crc  = file.Files_CalculatorChecksum();
  meta_data_temp.package_size = size_file;
  printf("size %d\n", file.Files_GetSizeFile());
  memcpy(meta_data_temp.name, name_file, length_name);
  meta_data = &meta_data_temp;
  sizearr = framemessage.CreateMessage(TYPE_MSG_UPDATE_FILE, (9 + length_name), (uint8_t*)meta_data, dataout);
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
  data_temp.length  = length;
  data_temp.offset  = offset;
  file.Files_GetArrFile(length, offset, data_temp.data);
  data = &data_temp;
  lengths = framemessage.CreateMessage(TYPE_MSG_UPDATE_FILE, sizeof(uploadData_t) - (128 - length), (uint8_t*)data, dataout);
  file.Files_Clear();
  return lengths;
}