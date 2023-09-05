
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"                  // Device header
#include "bts_frame_message.h"
#include "bts_get_message.h"
#include "handle_msg.h"

#include "lib_sd.h"
#include "delta.h"
#include "flash_if.h"

uint16_t lenght = 0;
uint32_t addrees_save = FLASH_SAVE_MODE;

int main(void)
{
  SystemInit();
  db_DEBUG_Init(9600);
  INIT_UART_2(9600);
  Fn_DELAY_Init(72);
	MSD0_SPI_Configuration();
	SD_Mount();
	printf("hello\n");
	
//	printf("Kich thuoc file %s la %ld bytes\n", "ptit.bin", SD_getFileSize("ptit.bin"));
//	printf("check sum: %x \n", SD_GetCheckSumFile("ptit.bin"));
//	
//	flashSaveMode_t data, *temp_data;
//	data.mode = 0xAABBCCDD;
//	data.reserved_1 = 0xAABBCCDD;
//	data.reserved_2 = 0xAABBCCFF;
//	data.reserved_3 = 0;
//	temp_data = &data;
//	FLASH_If_Init();
//	FLASH_If_Erase(addrees_save);
//	FLASH_If_Write(&addrees_save, (uint32_t*)temp_data, sizeof(flashSaveMode_t)/4);
//	FLASH_If_Finish();
	
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
