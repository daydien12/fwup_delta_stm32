
#include "main.h"

#define FILE_OLD "demo/old.bin"
#define FILE_PATCH "demo/patch.patch"
#define FILE_CREATE "demo/create.bin"

#define PORT "/dev/ttyUSB0"
#define FLASH_ADDRESS 0x08006000

#define PATH_FILE_OLD "/mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/4.delta/jj/old.bin"
#define PATH_FILE_PATCH "/mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/4.delta/jj/patch.patch"

/*
 MODE_NULL       = 0x00;
 MODE_BOTLOADER  = 0x01;
 UPDATE_FILE     = 0x02;
 DELTA           = 0x03;
 UPDATE_FLASH    = 0x04;
 MODE_APP        = 0x05;
 */

/*
1-0 vào bootloader

2-0 upload file: patch;                     upload file, delta, flash, app
2-1 upload file: old patch;                 upload file, delta, flash, app
2-2 upload file: run;                       upload file,        flash, app
3-0 Delta,                                                delta, flash, app
4-0 Flash,                                                       flash, app
5-0 vào app
6-0 đổi file create -> old.   rename file,  upload file, delta, flash, app

*/
#define STATE_RUN 0x02
/*old, patch = 1, patch = 0, file = 2*/

#define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.Apptest_stm32/MDK/Objects/gpio_blynk.bin"

HandleMsg handlemsg;
SerialPort serial(PORT, 9600, 1);
GetMessage getmessage;
FrameMessage framemessage;

updateFile_t updatefile;
valueSystem_t value_system;
timeoutSendRepeat_t value_send;

uint8_t state_sys = STATE_RUN;
void CreateMessageUpdateDeviceTest(void);
void SendTimeout(void);

int main(int argc, char *argv[])
{
  state_sys = argv[1][0] - '0';

  value_send.flag_send = 1;
  value_send.count_timeout = 0;
  value_send.flag_send_timeout = 0;

  value_system.sizeof_message = 0;
  value_system.status_update_file = 0;

  updatefile.state_update_file = OTA_STATE_START;
  updatefile.mode_update_file = argv[2][0] - '0';

  while (1)
  {
    int32_t checkdata = serial.Available();
    if (checkdata > 0)
    {
      getmessage.GetMessages(serial.readbyte(), value_system.array);
      if (getmessage.IsMessage(value_system.sizeof_message) == 1)
      {
        if (value_system.sizeof_message > 0)
        {
          messageFrameMsg_t dataout;
          uint8_t length_data = framemessage.DetectMessage(value_system.array, &dataout);
          printf("\nlength_data: %d\n", length_data);
          switch (dataout.TypeMessage)
          {
          case TYPE_MSG_MODE_BOTLOADER:
            if (dataout.Data[0] == 1)
            {
              printf("Bootloader Done\n");
              state_sys = TYPE_MSG_MODE_NULL;
            }
            break;

          case TYPE_MSG_RENAME_FILE:
            if (dataout.Data[0] == 1)
            {
              printf("Rename file Done\n");
              state_sys = TYPE_MSG_UPDATE_FILE;
            }
            break;

          case TYPE_MSG_UPDATE_FILE:

            if (updatefile.mode_update_file == 0)
            {
              uint8_t state = handlemsg.Handle_UpdateFileState(dataout.Data, updatefile.state_update_file, updatefile.offset, updatefile.size_file_bin);
              printf("--------------Update Mode 0(Patch): %d\n", state);
              if ((state == 1))
              {
                updatefile.offset = 0;
                updatefile.size_file_bin = 0;
                state_sys = TYPE_MSG_DELTA;
              }
            }
            else if (updatefile.mode_update_file == 1)
            {
              uint8_t state = handlemsg.Handle_UpdateFileState(dataout.Data, updatefile.state_update_file, updatefile.offset, updatefile.size_file_bin);
              printf("--------------Update Mode 1(Old, Patch): %d\n", state);
              if ((value_system.status_update_file == 0) && (state == 1))
              {
                updatefile.offset = 0;
                updatefile.size_file_bin = 0;

                value_system.status_update_file = 1;
                updatefile.state_update_file = OTA_STATE_START;
              }
              else if ((value_system.status_update_file == 1) && (state == 1))
              {
                state_sys = TYPE_MSG_DELTA;
              }
            }
            else if (updatefile.mode_update_file == 2)
            {
              uint8_t state = handlemsg.Handle_UpdateFileState(dataout.Data, updatefile.state_update_file, updatefile.offset, updatefile.size_file_bin);
              printf("--------------Update Mode 2(run.bin): %d\n", state);
              if ((state == 1))
              {
                updatefile.offset = 0;
                updatefile.size_file_bin = 0;
                state_sys = TYPE_MSG_UPDATE_FLASH;
              }
            }
            break;

          case TYPE_MSG_DELTA:
            if (dataout.Data[0] == 1)
            {
              printf("Created Delta Done\n");
              state_sys = TYPE_MSG_UPDATE_FLASH;
            }
            break;

          case TYPE_MSG_UPDATE_FLASH:
            if (dataout.Data[0] == 1)
            {
              printf("Flash Update Done\n");
              state_sys = TYPE_MSG_MODE_APP;
            }
            break;

          case TYPE_MSG_MODE_APP:
            if (dataout.Data[0] == 1)
            {
              printf("Mode App Done\n");
              state_sys = TYPE_MSG_MODE_NULL;
            }
            break;

          case TYPE_MSG_MODE_NULL:
            break;
          }
          value_send.flag_send = 1;
          value_send.count_timeout = 0;
          value_send.flag_send_timeout = 0;
        }
      }
    }

    if (value_send.flag_send == 1)
    {
      switch (state_sys)
      {
      case TYPE_MSG_MODE_BOTLOADER:
        sleep(1);
        printf("Bootloader Send: %d\n", handlemsg.Handle_ModeBootloader(PORT));
        break;
      case TYPE_MSG_RENAME_FILE:
        printf("Rename Send: %d\n", handlemsg.Handle_RenameFile(PORT, FILE_CREATE, FILE_OLD));
        break;
      case TYPE_MSG_UPDATE_FILE:
        sleep(1);
        if (updatefile.mode_update_file == 1)
        {
          if (value_system.status_update_file == 0)
          {
            handlemsg.Handle_UpdateFileRun(PATH_FILE_OLD, PORT, FILE_OLD, updatefile.state_update_file, updatefile.offset, updatefile.size_file_bin);
          }
          else if (value_system.status_update_file == 1)
          {
            handlemsg.Handle_UpdateFileRun(PATH_FILE_PATCH, PORT, FILE_PATCH, updatefile.state_update_file, updatefile.offset, updatefile.size_file_bin);
          }
        }
        else if (updatefile.mode_update_file == 0)
        {
          handlemsg.Handle_UpdateFileRun(PATH_FILE_PATCH, PORT, FILE_PATCH, updatefile.state_update_file, updatefile.offset, updatefile.size_file_bin);
        }
        else if (updatefile.mode_update_file == 2)
        {
          handlemsg.Handle_UpdateFileRun(NAME_BIN, PORT, "run.bin", updatefile.state_update_file, updatefile.offset, updatefile.size_file_bin);
        }
        break;

      case TYPE_MSG_DELTA:
        sleep(1);
        printf("Delta Send: %d\n", handlemsg.Handle_DeltaUpdate(PORT, FILE_OLD, FILE_PATCH, FILE_CREATE));
        break;

      case TYPE_MSG_UPDATE_FLASH:
        sleep(1);
        if (updatefile.mode_update_file == 2)
        {
          printf("Flash Send: %d\n", handlemsg.Handle_FlashUpdate(PORT, "run.bin", FLASH_ADDRESS));
        }
        else
        {
          printf("Flash Send: %d\n", handlemsg.Handle_FlashUpdate(PORT, FILE_CREATE, FLASH_ADDRESS));
        }
        break;

      case TYPE_MSG_MODE_APP:
        sleep(5);
        printf("Mode App Send: %d\n", handlemsg.Handle_ModeApp(PORT, FLASH_ADDRESS));
        break;

      case TYPE_MSG_MODE_NULL:
        sleep(1);
        printf("Null Send: %d\n", handlemsg.Handle_Null());
        break;
      }
      value_send.flag_send = 0;
      value_send.count_timeout = 0;
      value_send.flag_send_timeout = 1;
    }
    SendTimeout();
    usleep(1000);
    getmessage.TimeOut();
  }
  return 0;
}

void SendTimeout(void)
{
  if (value_send.flag_send_timeout == 1)
  {
    if (value_send.count_timeout >= 1000)
    {
      value_send.flag_send = 1;
      value_send.count_timeout = 0;
    }
    value_send.count_timeout++;
  }
}