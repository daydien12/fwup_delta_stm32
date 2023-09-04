
#include "main.h"

#define FILE_OLD "demo/old.bin"
#define FILE_PATCH "demo/patch.patch"
#define FILE_CREATE "demo/create.bin"
   

//#define STATE_RUN TYPE_MSG_MODE_BOTLOADER
//#define STATE_RUN TYPE_MSG_UPDATE_FILE
//#define STATE_RUN TYPE_MSG_DELTA
//#define STATE_RUN TYPE_MSG_UPDATE_FLASH
//#define STATE_RUN TYPE_MSG_MODE_APP
#define STATE_RUN  TYPE_MSG_MODE_NULL


// Files        file("/home/bien/Desktop/bien.txt", std::ios::in|std::ios::binary);
// #define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Desktop/code_ota/gpio_blynk.bin"
// #define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.tool/blinky-k64f-new.bin"
// #define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_10k.bin"
// #define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_20k.bin"
// #define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_30k.bin"
// #define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_10516k.bin"
// #define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_51233.bin"
// #define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_53561.bin"
// #define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_200000.bin"
// #define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.tool/demo/blinky-k64f-old.bin"
// #define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.tool/demo/blinky-k64f.patch"

// #define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Desktop/code_ota/gpio_blynk.bin"
//#define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/blinky-k64f-new.bin"
// #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_10k.bin"
// #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_20k.bin"
// #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_30k.bin"
// #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_10516k.bin"
// #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_51233.bin"
// #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_53561.bin"
// #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_200000.bin"
 #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/gpio_blynk.bin"

// #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/demo/blinky-k64f-old.bin"
// #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/demo/blinky-k64f.patch"

HandleMsg handlemsg;
SerialPort serial("/dev/ttyUSB0", 9600, 1);
GetMessage getmessage;
FrameMessage framemessage;

uint8_t state_sys = STATE_RUN;
void CreateMessageUpdateDeviceTest(void);
int main()
{
  uint8_t array[200], state_update_file = OTA_STATE_START;
  uint16_t sizeof_message = 0;
  uint32_t offset = 0, size_file_bin = 0;
  uint8_t length_data = 0, flag_update_send = 1, size_temp = 0, flag_update_send_timeout = 0;
  uint64_t count_timeout = 0;
  // CreateMessageUpdateDeviceTest();

  while (1)
  {
    int32_t checkdata = serial.Available();
    if (checkdata > 0)
    {
      getmessage.GetMessages(serial.readbyte(), array);
      if (getmessage.IsMessage(sizeof_message) == 1)
      {
        if (sizeof_message > 0)
        {
          messageFrameMsg_t dataout;
          length_data = framemessage.DetectMessage(array, &dataout);
          printf("\nlength_data: %d\n", length_data);

          switch (dataout.TypeMessage)
          {
          case TYPE_MSG_MODE_BOTLOADER:

            break;

          case TYPE_MSG_UPDATE_FILE:
            handlemsg.Handle_UpdateFileState(dataout.Data, state_update_file, offset, size_file_bin);
            count_timeout = 0;
            flag_update_send = 1;
            flag_update_send_timeout = 0;
            break;

          case TYPE_MSG_DELTA:
            if (dataout.Data[0] == 1)
            {
              printf("Created Delta Done\n");
              state_sys = TYPE_MSG_MODE_NULL;
              count_timeout = 0;
              flag_update_send = 0;
              flag_update_send_timeout = 0;
            }
            else
            {
              printf("Created Delta Error\n");
              count_timeout = 0;
              flag_update_send = 1;
              flag_update_send_timeout = 0;
            }
            break;

          case TYPE_MSG_UPDATE_FLASH:
            if (dataout.Data[0] == 1)
            {
              printf("Flash Update Done\n");
              state_sys = TYPE_MSG_MODE_NULL;
              count_timeout = 0;
              flag_update_send = 0;
              flag_update_send_timeout = 0;
            }
            else
            {
              printf("Flash Update Error\n");
              count_timeout = 0;
              flag_update_send = 1;
              flag_update_send_timeout = 0;
            }
            break;

          case TYPE_MSG_MODE_APP:
            break;

          case TYPE_MSG_MODE_NULL:
            break;
          }
        }
      }
    }

    switch (state_sys)
    {
    case TYPE_MSG_MODE_BOTLOADER:

      break;
    case TYPE_MSG_UPDATE_FILE:
      if (flag_update_send == 1)
      {
        sleep(1);
        handlemsg.Handle_UpdateFileRun(NAME_BIN, "/dev/ttyUSB0", "ptit.bin", state_update_file, offset, size_file_bin);
        // handlemsg.Handle_UpdateFileRun(NAME_BIN, "/dev/ttyUSB0", "/demo/old.bin", state_update_file, offset, size_file_bin);
        // handlemsg.Handle_UpdateFileRun(NAME_BIN, "/dev/ttyUSB0", "/demo/patch.patch", state_update_file, offset, size_file_bin);
        count_timeout = 0;
        flag_update_send = 0;
        flag_update_send_timeout = 1;
      }

      if (flag_update_send_timeout == 1)
      {
        if (count_timeout >= 2000)
        {
          flag_update_send = 1;
          count_timeout = 0;
        }
        count_timeout++;
      }
      break;

    case TYPE_MSG_DELTA:

      if (flag_update_send == 1)
      {
        sleep(1);
        printf("Delta Send: %d\n", handlemsg.Handle_DeltaUpdate("/dev/ttyUSB0", FILE_OLD, FILE_PATCH, FILE_CREATE));
        count_timeout = 0;
        flag_update_send = 0;
        flag_update_send_timeout = 1;
      }

      if (flag_update_send_timeout == 1)
      {
        if (count_timeout >= 2000)
        {
          flag_update_send = 1;
          count_timeout = 0;
        }
        count_timeout++;
      }
      break;

    case TYPE_MSG_UPDATE_FLASH:
      if (flag_update_send == 1)
      {
        sleep(1);
        printf("Delta Send: %d\n", handlemsg.Handle_FlashUpdate("/dev/ttyUSB0", "ptit.bin", 0x08006000));
        count_timeout = 0;
        flag_update_send = 0;
        flag_update_send_timeout = 1;
      }

      if (flag_update_send_timeout == 1)
      {
        if (count_timeout >= 3000)
        {
          flag_update_send = 1;
          count_timeout = 0;
        }
        count_timeout++;
      }
      break;

    case TYPE_MSG_MODE_APP:
      break;

    case TYPE_MSG_MODE_NULL:
      if (flag_update_send == 1)
      {
        sleep(1);
        printf("Delta Send: %d\n", handlemsg.Handle_Null());
        count_timeout = 0;
        flag_update_send = 0;
        flag_update_send_timeout = 1;
      }

      if (flag_update_send_timeout == 1)
      {
        if (count_timeout >= 3000)
        {
          flag_update_send = 1;
          count_timeout = 0;
        }
        count_timeout++;
      }
      break;
    }

    usleep(1000);
    getmessage.TimeOut();
  }

  return 0;
}

void CreateMessageUpdateDeviceTest(void)
{
  // uint8_t arr2[200], sizearr = 0;
  // uint8_t length_send = 0;
  // uint32_t size_file_bin = 3120;
  // uint32_t offset_bin = 0;

  // uploadDeleteFile_t *deletes, delete_temp;

  // printf("Size File: %d\n", size_file_bin);

  // printf("\n---------------(Create Message Metadata Test)---------------\n");

  // sizearr = handlemsg.UpdateFile_CreateFrameMetaData(NAME_BIN, 0xAABBCCDD, "biennq.bin", arr2);

  // for(int i=0; i<sizearr; i++)
  // {
  //  if (arr2[i] <= 0x0f)
  //   {
  //     printf("0%x ", arr2[i]);
  //   }
  //   else
  //   {
  //     printf("%x ", arr2[i]);
  //   }
  // }
  // serial.writebyte(arr2, sizearr);

  // length_send = (size_file_bin/128)+1;

  // sleep(5);

  // for(int i=0; i<length_send; i++)
  // {
  //   uint32_t sizedata = 0;
  //   if((size_file_bin-offset_bin)>128)
  //   {
  //     sizedata = handlemsg.UpdateFile_CreateFrameData(NAME_BIN,128,offset_bin,arr2);
  //     printf("sizearr: %d\n", sizedata);
  //   }
  //   else
  //   {
  //     sizedata = handlemsg.UpdateFile_CreateFrameData(NAME_BIN,(size_file_bin-offset_bin),offset_bin,arr2);
  //     printf("sizearr: %d\n", sizedata);
  //   }
  //   serial.writebyte(arr2, sizedata);
  //   offset_bin += sizedata-12;
  //   sleep(1);
  // }

  /*
  printf("\n---------------(Create Message data Test)---------------\n");
  data_temp.cmd = OTA_STATE_DATA;
  data_temp.length  = 128;
  data_temp.offset  = 0;
  // for(int i=0; i<128; i++)
  // {
  //  data_temp.data[i] = i+1;
  // }
  file.Files_GetArrFile(128, 0, data_temp.data);
  data = &data_temp;
  sizearr = framemessage.CreateMessage(TYPE_MSG_UPDATE_FILE,sizeof(uploadData_t), (uint8_t*)data, arr2);
  printf("sizearr: %d\n", sizearr);
  for(int i=0; i<sizearr; i++)
  {
  if (arr2[i] <= 0x0f)
  {
  printf("0%x ", arr2[i]);
  }
  else
  {
  printf("%x ", arr2[i]);
  }
  }
  */

  printf("\n---------------(Done)---------------\n");
}