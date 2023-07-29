
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"                  // Device header
#include "delta.h"
#include "Delay.h"

#define FILE_OLD    "demo/blinky-k64f-old.bin"
#define FILE_PATCH  "demo/blinky-k64f.patch"
#define FILE_CREATE "demo/blinky5.bin"
#define FILE_FILE "bien1.txt"

uint8_t textFileBuffer[] = "Chinese and English test strings";
int main(void)
{
	char Number[10];
	size_t offset = 0;
  SystemInit();
	db_DEBUG_Init(9600);
	Fn_DELAY_Init(72);
  MSD0_SPI_Configuration();
	Delta_Mount();
	//Delta_Run(FILE_OLD, FILE_PATCH, FILE_CREATE);
	Fn_DELAY_ms(1000);
	SD_WriteFile(FILE_FILE, textFileBuffer, sizeof(textFileBuffer)-1, 0);

  while (1)
  {

  }
}
