
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"                  // Device header
#include "delta.h"
#include "delay.h"
#include "save_file.h"

#define FILE_OLD    "demo/blinky-k64f-old.bin"
#define FILE_PATCH  "demo/blinky-k64f.patch"
#define FILE_CREATE "demo/blinky5.bin"
#define FILE_FILE "bien123456789.txt"

uint8_t textFileBuffer1[] = " Chinese and English test strings";
uint8_t textFileBuffer2[] = "1Chinese and English test strings\n";
uint8_t textFileBuffer3[] = "2Chinese and English";

int main(void)
{
  SystemInit();
	db_DEBUG_Init(9600);
	Fn_DELAY_Init(72);
  MSD0_SPI_Configuration();
	
	ETX_ReceiveChunk();
	//Delta_Run(FILE_OLD, FILE_PATCH, FILE_CREATE);

  while (1)
  {
		ETX_Run();
  }
}
