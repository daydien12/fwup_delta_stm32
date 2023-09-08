#ifndef __LIB_SD_H
#define __LIB_SD_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_conf.h"
#include "debug.h"
#include "ff.h"
#include "SPI_MSD1_Driver.h"
#include "SPI_MSD0_Driver.h"
#include <string.h>

extern FRESULT res;
/* Exported Functions --------------------------------------------------------*/

/* Nút b?t d?u du?c quét (còn du?c s? d?ng nhu khu v?c làm vi?c) */
FRESULT  SD_ScanFiles (char* path);         	
uint32_t SD_TotalSize(char *path);
DWORD SD_getFileSize(const char* filename);

uint32_t SD_WriteFile(const char *file_name, const void *ptr, size_t size, size_t offset);
uint32_t SD_ReadFile(const char *file_name, void *ptr, size_t size, size_t offset);
uint32_t SD_GetCheckSumFile(const char *file_name);
void SD_Mount(void);
#endif/* __LID_SD_H */
