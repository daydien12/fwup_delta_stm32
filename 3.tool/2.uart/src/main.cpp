
#include "main.h"

#define FILE_OLD    "demo/blinky-k64f-old.bin"
#define FILE_PATCH  "demo/blinky-k64f.patch"
#define FILE_CREATE "demo/bien.bin"

//Files        file("/home/bien/Desktop/bien.txt", std::ios::in|std::ios::binary);
//#define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Desktop/code_ota/gpio_blynk.bin"
#define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Desktop/code_ota/blinky-k64f-new.bin"
//#define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Desktop/code_ota/file_bin_test/bin_10k.bin"
// #define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Desktop/code_ota/file_bin_test/bin_20k.bin"
// #define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Desktop/code_ota/file_bin_test/bin_30k.bin"
// #define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Desktop/code_ota/file_bin_test/bin_10516k.bin"
// #define NAME_BIN "/mnt/c/Users/nguye/OneDrive/Desktop/code_ota/file_bin_test/bin_51233k.bin"

HandleMsg    handlemsg;
SerialPort   serial("/dev/ttyUSB0", 9600, 1);
GetMessage   getmessage;
FrameMessage framemessage;


void CreateMessageUpdateDeviceTest(void);
int main()
{
  uint8_t array[200], state_update_file = OTA_STATE_START;
  uint16_t sizeof_message = 0;
  uint32_t offset = 0, size_file_bin = 0;
  uint8_t length_data = 0, flag_update = 1, size_temp = 0;
  uint64_t count_time = 0;
 // CreateMessageUpdateDeviceTest();

  //sleep(1);
  //handlemsg.Handle_DeltaUpdate("/dev/ttyUSB0", FILE_OLD, FILE_PATCH, FILE_CREATE);
 
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
          //printf("\n-(Size    : %d)-\n", length_data));
          if(dataout.TypeMessage == TYPE_MSG_UPDATE_FILE)
          {
            flag_update = 0;
            sleep(1);
            printf("Get: size_file_bin: %d - %d - %d\n", size_file_bin, offset, state_update_file);
            handlemsg.Handle_UpdateFile(NAME_BIN, "/dev/ttyUSB0",1, "ptit2.bin" , state_update_file, offset, size_file_bin);
            flag_update = 1;
            count_time = 0;
            if(dataout.Data[0] == OTA_STATE_START)
            {
                state_update_file = OTA_STATE_DATA;
            }
            else if((size_file_bin<= offset) && (dataout.Data[0] == OTA_STATE_DATA))
            {
              state_update_file = OTA_STATE_END;
            }
            else if(dataout.Data[0] == OTA_STATE_END)
            {
              state_update_file = 0;
              offset = 0;
              size_file_bin = 0;
              flag_update = 0;
              count_time = 0;
            }
          }
          else
          {
            printf("Start     : %x\n", dataout.Start);
            printf("TypeMSG   : %x\n", dataout.TypeMessage);
            printf("Length    : %d\n", dataout.Length);
            printf("Value     : ");
            for (int i = 0; i < dataout.Length - DEFAULT_BYTE_CHECKSUM; i++)
            {
              printf("%d ", dataout.Data[i]);
            }
            printf("\nCheck sum : %x\n", dataout.Crc);
          }

        }
      }
    }
    
    if(flag_update == 1)
    {
      if(count_time >= 3000)
      {
        size_temp = handlemsg.Handle_UpdateFile(NAME_BIN, "/dev/ttyUSB0",0, "ptit2.bin" , state_update_file, offset, size_file_bin);
        count_time = 0;
      }
      count_time ++;
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
	// 	if (arr2[i] <= 0x0f)
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
	// 	data_temp.data[i] = i+1;
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