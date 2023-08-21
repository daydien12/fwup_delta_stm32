
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"                  // Device header
#include "delta.h"
#include "delay.h"
#include "save_file.h"
#include "bts_frame_message.h"
#include "bts_get_message.h"
#include "handle_msg.h"

#define FILE_OLD    "demo/blinky-k64f-old.bin"
#define FILE_PATCH  "demo/blinky-k64f.patch"
#define FILE_CREATE "demo/blinky5.bin"
#define FILE_FILE   "bien123456789.txt"

#define AS32_SETUP_M0_PIN GPIO_Pin_8
#define AS32_SETUP_M1_PIN GPIO_Pin_9

static void CreateMessageUpdateDeviceTest(void);
uint16_t lenght = 0;
uint8_t size_array_data = 0;
messageFrameMsg_t frame_message;

int main(void)
{
  SystemInit();
  db_DEBUG_Init(9600);
  INIT_UART_2(9600);
  Fn_DELAY_Init(72);

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

  CreateMessageUpdateDeviceTest();
  while (1)
  {
		
    if (Is_Message(&lenght) != 0)
    {
      if (lenght > 0)
      {
				messageFrameMsg_t dataout;
				printf("\n-(Size    : %d)-\n", DetectMessage(Array2_Receive, &dataout));	
				Handle_GetMsg(dataout);			
      }
    }
    
		Fn_DELAY_ms(1);
    //ETX_Run();
  }
}

/**
   @brief Create a Message Update Device Test object
   Test object is used to create a update device message.
*/
static void CreateMessageUpdateDeviceTest(void)
{
	uint8_t arr1[100]={8,9,1,2,3,4,5,6,7}, arr2[200], *arr3;
	uploadMetaData_t *meta_data, meta_data_temp;
	uint16_t sizedata = 0;
	printf("sizeof: %d \n", sizeof(uploadData_t));
	printf("\n---------------(Create Message Test)---------------\n");
	
	meta_data_temp.cmd = OTA_STATE_START;
	meta_data_temp.package_crc = 0xAABBCCDD;
	meta_data_temp.package_size = 20000;
	meta_data = &meta_data_temp;
	arr3 = (uint8_t*)meta_data;

	memcpy(arr1, arr3, 9);
	for(int i=0; i<CreateMessage(TYPE_MSG_UPDATE_FILE,9,arr1, arr2); i++)
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
	
	printf("\n---------------(Detect Message Test)---------------");
	DebugMessage(arr2);
	printf("---------------(Done)---------------\n");
}