#ifndef __LIB_SD_H
#define __LIB_SD_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_conf.h"
#include "usart.h"
#include "ff.h"
#include "SPI_MSD1_Driver.h"
#include "SPI_MSD0_Driver.h"
#include <string.h>

extern FRESULT res;
/* Exported Functions --------------------------------------------------------*/

/* N�t b?t d?u du?c qu�t (c�n du?c s? d?ng nhu khu v?c l�m vi?c) */
FRESULT  SD_ScanFiles (char* path);         	
uint32_t SD_TotalSize(char *path);
DWORD SD_getFileSize(const char* filename);

uint32_t SD_WriteFile(const char *file_name, const void *ptr, size_t size, size_t offset);

#endif/* __LID_SD_H */
