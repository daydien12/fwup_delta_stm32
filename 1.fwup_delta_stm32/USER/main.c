
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"                  // Device header
#include "delta.h"
#include "delay.h"
#include "save_file.h"
#include "bts_frame_message.h"
#include "bts_get_message.h"

#define FILE_OLD    "demo/blinky-k64f-old.bin"
#define FILE_PATCH  "demo/blinky-k64f.patch"
#define FILE_CREATE "demo/blinky5.bin"
#define FILE_FILE "bien123456789.txt"

#define AS32_SETUP_M0_PIN GPIO_Pin_8
#define AS32_SETUP_M1_PIN GPIO_Pin_9
static void CreateMessageUpdateDeviceTest(void);
int i=0;
int main(void)
{
  SystemInit();
	db_DEBUG_Init(9600);
	INIT_UART_2(9600);
	Fn_DELAY_Init(72);
	
	GPIO_InitTypeDef gpio_struct;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  gpio_struct.GPIO_Pin = AS32_SETUP_M0_PIN | AS32_SETUP_M1_PIN;
  gpio_struct.GPIO_Mode = GPIO_Mode_Out_PP;
  gpio_struct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &gpio_struct);
  GPIO_ResetBits(GPIOB, AS32_SETUP_M0_PIN | AS32_SETUP_M1_PIN);
	
  //MSD0_SPI_Configuration();
	
	//ETX_ReceiveChunk();
	//Delta_Run(FILE_OLD, FILE_PATCH, FILE_CREATE);
	
  while (1)
  {
		CreateMessageUpdateDeviceTest();
		for(i=0; i<10; i++)
		{
		Fn_DELAY_ms(1000);
		}
		//ETX_Run();
  }
}

/**
 * @brief Create a Message Update Device Test object
 * Test object is used to create a update device message.
 */
static void CreateMessageUpdateDeviceTest(void)
{
	uint8_t array_data_in[DEFAULT_MAX_NUMBER_DEVICE], array_data_out[50];
	uint8_t size_array = 0, count = 0;
	for(count = 0; count < DEFAULT_MAX_NUMBER_DEVICE; count++)
	{
		array_data_in[count] = count;
	}
	printf("\n---------------(Create Message Update Device Test)---------------");
	size_array = BTS_Frame_Creat_Device(array_data_in, array_data_out);
	printf("\n---------------(Size : %d)---------------\n", size_array);
	for(count = 0; count < size_array; count++)
	{
		db_DEBUG_SendChar(array_data_out[count]);
		if(array_data_out[count] <= 0x0f)
		{
			printf("0%x ", array_data_out[count]);
		}
		else
		{
			printf("%x ", array_data_out[count]);
		}
		
	}
	
	//DebugMessage(array_data_out);
	printf("---------------(Done)---------------\n");
}