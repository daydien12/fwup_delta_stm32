
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"                  // Device header
#include "delta.h"
#include "bts_frame_message.h"
#include "bts_get_message.h"
#include "handle_msg.h"

#include "lib_sd.h"
#include "delta.h"


#define FILE_FILE   "bien123456789.txt"

#define AS32_SETUP_M0_PIN GPIO_Pin_8
#define AS32_SETUP_M1_PIN GPIO_Pin_9

static void CreateMessageUpdateDeviceTest(void);
uint16_t lenght = 0;

int main(void)
{
  SystemInit();
  db_DEBUG_Init(9600);
  INIT_UART_2(9600);
  Fn_DELAY_Init(72);
	MSD0_SPI_Configuration();
	SD_Mount();
	f_unlink("bien.bin");
	printf("hello\n");
  CreateMessageUpdateDeviceTest();
	
  while (1)
  {
    if (Is_Message(&lenght) != 0)
    {
      if (lenght > 0)
      {
				messageFrameMsg_t dataout;
				printf("\n-(Size    : %d)-\n", DetectMessage(Array2_Receive, &dataout));
				Handle_GetMsg(dataout, db_DEBUG_SendChar);			
      }
    }
		Fn_DELAY_ms(1);
  }
}

/**
   @brief Create a Message Update Device Test object
   Test object is used to create a update device message.
*/
static void CreateMessageUpdateDeviceTest(void)
{
	uint8_t arr2[200];
	uploadMetaData_t *meta_data, meta_data_temp;
	uploadData_t *data, data_temp;
	uploadDeleteFile_t *deletes, delete_temp;
	
	printf("\n---------------(Create Message Metadata Test)---------------\n");
	meta_data_temp.cmd          = OTA_STATE_START;
	meta_data_temp.package_crc  = 0xAABBCCDD;
	meta_data_temp.package_size = 1024;
	memcpy(meta_data_temp.name, "gpio.bin", sizeof("gpio.bin"));
	meta_data = &meta_data_temp;
	
	for(int i=0; i<CreateMessage(TYPE_MSG_UPDATE_FILE,9+sizeof("gpio.bin"),(uint8_t*)meta_data, arr2); i++)
	{
		db_DEBUG_SendChar(arr2[i]);
		if (arr2[i] <= 0x0f)
    {
      printf("0%x ", arr2[i]);
    }
    else
    {
      printf("%x ", arr2[i]);
    }
	}
	
//	printf("\n---------------(Create Message data Test)---------------\n");
//	data_temp.cmd = OTA_STATE_DATA;
//	data_temp.length  = 128;
//	data_temp.offset  = 0;
//	for(int i=0; i<128; i++)
//	{
//		data_temp.data[i] = i+1;
//	}
//	data = &data_temp;

//	for(int i=0; i<CreateMessage(TYPE_MSG_UPDATE_FILE,sizeof(uploadData_t), (uint8_t*)data, arr2); i++)
//	{
//		if (arr2[i] <= 0x0f)
//    {
//      printf("0%x ", arr2[i]);
//    }
//    else
//    {
//      printf("%x ", arr2[i]);
//    }
//	}
//	
//	printf("\n---------------(Delete Message data Test)---------------\n");
//	delete_temp.cmd = OTA_STATE_DELETE;
//	memcpy(delete_temp.name, "gpio.bin", sizeof("gpio.bin"));
//	deletes = &delete_temp;

//	for(int i=0; i<CreateMessage(TYPE_MSG_UPDATE_FILE,1+sizeof("gpio.bin"), (uint8_t*)deletes, arr2); i++)
//	{
//		if (arr2[i] <= 0x0f)
//    {
//      printf("0%x ", arr2[i]);
//    }
//    else
//    {
//      printf("%x ", arr2[i]);
//    }
//	}
//	
//	printf("\n---------------(Detect Message Test)---------------");
//	//DebugMessage(arr2);
//	printf("---------------(Done)---------------\n");
}

  //  GPIO_InitTypeDef gpio_struct;
  //  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  //  gpio_struct.GPIO_Pin = AS32_SETUP_M0_PIN | AS32_SETUP_M1_PIN;
  //  gpio_struct.GPIO_Mode = GPIO_Mode_Out_PP;
  //  gpio_struct.GPIO_Speed = GPIO_Speed_50MHz;
  //  GPIO_Init(GPIOB, &gpio_struct);
  //  GPIO_ResetBits(GPIOB, AS32_SETUP_M0_PIN | AS32_SETUP_M1_PIN);

  //  MSD0_SPI_Configuration();
  //  ETX_ReceiveChunk();
  //  Delta_Run(FILE_OLD, FILE_PATCH, FILE_CREATE);