
#include "main.h"

#define FILE_OLD "demo/old.bin"
#define FILE_PATCH "demo/patch.patch"
#define FILE_CREATE "demo/create1.bin"

// #define STATE_RUN TYPE_MSG_MODE_BOTLOADER
 #define STATE_RUN TYPE_MSG_UPDATE_FILE
// #define STATE_RUN TYPE_MSG_DELTA
// #define STATE_RUN TYPE_MSG_UPDATE_FLASH
// #define STATE_RUN TYPE_MSG_MODE_APP
// #define STATE_RUN TYPE_MSG_MODE_NULL

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
// #define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/gpio_blynk.bin"

// #define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Desktop/code_ota/gpio_blynk.bin"
// #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/blinky-k64f-new.bin"
// #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_10k.bin"
// #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_20k.bin"
// #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_30k.bin"
// #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_10516k.bin"
// #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_51233.bin"
// #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_53561.bin"
// #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_200000.bin"
// #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_650000.bin"
 #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/bin_660000.bin"
// #define NAME_BIN "/mnt/c/Users/nhanma/Documents/github/fwup_delta_stm32/3.tool/file_bin_test/gpio_blynk.bin"
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
  uint8_t length_data = 0, flag_update_send = 1, flag_update_send_timeout = 0;
  uint64_t count_timeout = 0;

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
            if (dataout.Data[0] == 1)
            {
              printf("Bootloader Done\n");
              state_sys = TYPE_MSG_MODE_NULL;
              count_timeout = 0;
              flag_update_send = 0;
              flag_update_send_timeout = 0;
            }
            else
            {
              printf("Bootloader Error\n");
              count_timeout = 0;
              flag_update_send = 1;
              flag_update_send_timeout = 0;
            }
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
              state_sys = TYPE_MSG_MODE_APP;
              count_timeout = 0;
              flag_update_send = 1;
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
            if (dataout.Data[0] == 1)
            {
              printf("Mode App Done\n");
              state_sys = TYPE_MSG_MODE_NULL;
              count_timeout = 0;
              flag_update_send = 1;
              flag_update_send_timeout = 0;
            }
            else
            {
              printf("Mode App Error\n");
              count_timeout = 0;
              flag_update_send = 1;
              flag_update_send_timeout = 0;
            }
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
      if (flag_update_send == 1)
      {
        sleep(1);
        printf("Delta Send: %d\n", handlemsg.Handle_ModeBootloader("/dev/ttyUSB0"));
        count_timeout = 0;
        flag_update_send = 0;
        flag_update_send_timeout = 1;
      }

      if (flag_update_send_timeout == 1)
      {
        if (count_timeout >= 10)
        {
          flag_update_send = 1;
          count_timeout = 0;
        }
        count_timeout++;
      }
      break;
    case TYPE_MSG_UPDATE_FILE:
      if (flag_update_send == 1)
      {
        sleep(1);
        handlemsg.Handle_UpdateFileRun(NAME_BIN, "/dev/ttyUSB0", "ptit1.bin", state_update_file, offset, size_file_bin);
        // handlemsg.Handle_UpdateFileRun(NAME_BIN, "/dev/ttyUSB0", FILE_OLD, state_update_file, offset, size_file_bin);
        // handlemsg.Handle_UpdateFileRun(NAME_BIN, "/dev/ttyUSB0", FILE_PATCH, state_update_file, offset, size_file_bin);
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
        printf("Flash Send: %d\n", handlemsg.Handle_FlashUpdate("/dev/ttyUSB0", "ptit.bin", 0x08006000));
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
      if (flag_update_send == 1)
      {
        sleep(5);
        printf("Mode App Send: %d\n", handlemsg.Handle_ModeApp("/dev/ttyUSB0", 0x08004000));
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
        if (count_timeout >= 10)
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