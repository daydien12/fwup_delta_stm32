#include "lib_sd.h"
#include <stdio.h>

FRESULT res;
FATFS fs;
#define POLYNOMIAL 0x04C11DB7
uint32_t crc32(unsigned long crc, const unsigned char *data, size_t length);
/*******************************************************************************
    @T�n ch?c nang    : scan_files
    @M� t? ch?c nang  : T�m ki?m t?t c? c�c t?p trong thu m?c t?p
    @Input            : tham s? du?ng d?n: thu m?c g?c
    @Output           : Kh�ng c�
    @Return           : Tham s? FRESULT
    @Notes            : Kh�ng c�
  *****************************************************************************/
FRESULT SD_ScanFiles(char* path)        /* N�t b?t d?u du?c qu�t (c�n du?c s? d?ng nhu khu v?c l�m vi?c) */
{
  FRESULT res;
  FILINFO fno;
  DIR dir;
  int i;
  char *fn;         /* H�m n�y gi? s? cfg kh�ng ph?i Unicode. */
#if _USE_LFN
  static char lfn[_MAX_LFN + 1];
  fno.lfname = lfn;
  fno.lfsize = sizeof(lfn);
#endif


  res = f_opendir(&dir, path);                           /* M? thu m?c */
  if (res == FR_OK)
  {
    i = strlen(path);
    for (;;)
    {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0)
      {
        break;
      }
      if (fno.fname[0] == '.')
      {
        continue;
      }
#if _USE_LFN
      fn = *fno.lfname ? fno.lfname : fno.fname;
#else
      fn = fno.fname;
#endif
      if (fno.fattrib & AM_DIR)  /* N?u n� l� m?t thu m?c */
      {
        sprintf(&path[i], "/%s", fn);
        printf("scan file - %s\n\r", path);
        res = SD_ScanFiles(path);
        if (res != FR_OK)
        {
          break;
        }

        path[i] = 0;
      }
      else
      { /* N?u n� l� m?t t?p tin. */
        printf("scan file - %s/%s\n\r", path, fn);
      }
    }
  }
  else
  {
    printf("scan files error : %d\n\r", res);
  }

  return res;
}

/*******************************************************************************
    @T�n Ch?c nang    : SD_TotalSize
    @M� t? ch?c nang  : Kh�ng gian t?p chi?m d?ng
    @Input            : Kh�ng c�
    @Output           : Kh�ng c�
    @Return           : tham s? 1: Th�nh c�ng,0: th?t b?i
    @Notes            : Kh�ng c�
  *****************************************************************************/

uint32_t SD_TotalSize(char *path)
{
  FATFS *fs;
  DWORD fre_clust;

  res = f_getfree(path, &fre_clust, &fs);  /* Ph?i l� thu m?c g?c */
  if ( res == FR_OK )
  {
    printf("\n\rget %s drive space.\n\r", path);
    /* In dung lu?ng tr?ng theo don v? MB (gi? s? 512 byte / sector) */
    printf("%d MB total drive space.\r\n"
           "%d MB available.\r\n",
           (unsigned int)((fs->n_fatent - 2) * fs->csize ) / 2 / 1024 , (unsigned int)(fre_clust * fs->csize) / 2 / 1024 );

    return 1;
  }
  else
  {
    printf("\n\rGet total drive space faild!\n\r");
    return 0;
  }
}

DWORD SD_getFileSize(const char* filename)
{
	DWORD fileSize = 0;
  FIL file;
  if (f_open(&file, filename, FA_READ) == FR_OK)
  {
    // Get the size of the file
    fileSize = f_size(&file);
    f_close(&file); // Close the file when done

    // Now you have the fileSize in bytes, you can do whatever you want with it
  }
  return fileSize;
}

uint32_t SD_WriteFile(const char *file_name, const void *ptr, size_t size, size_t offset)
{
	FIL fdsts;
  UINT brs;
  f_open(&fdsts, file_name, FA_OPEN_ALWAYS | FA_WRITE);
  f_lseek(&fdsts, offset);
  f_write(&fdsts, ptr, size, &brs);
  f_close(&fdsts);
  return brs;
}
uint32_t SD_ReadFile(const char *file_name, void *ptr, size_t size, size_t offset)
{
	FIL fdsts;
  UINT brs;
  f_open(&fdsts, file_name, FA_OPEN_EXISTING | FA_READ);
  f_lseek(&fdsts, offset);
  f_read(&fdsts, ptr, size, &brs);
  f_close(&fdsts);
  return brs;
}
uint32_t SD_GetCheckSumFile(const char *file_name)
{
	FIL fdsts;
  UINT brs;
	uint8_t arr[10];
	uint32_t size_file_bin = 0, check_sum = 0xFFFFFFFFUL;
  f_open(&fdsts, file_name, FA_OPEN_EXISTING | FA_READ);
	size_file_bin = SD_getFileSize(file_name);
	for(int i=0; i < size_file_bin; i++)
	{
		f_read(&fdsts, arr, 1, &brs);
		check_sum = crc32(check_sum, arr, 1);
	}
	
	f_close(&fdsts);
	check_sum ^= 0xFFFFFFFFUL;
	return check_sum;
}

void SD_Mount(void)
{
	res = f_mount(0, &fs);
  if (res != FR_OK) 
	{
    printf("mount filesystem 0 failed : %d\n\r", res);
  }
}

uint32_t crc32(unsigned long crc, const unsigned char *data, size_t length)
{
  for (size_t i = 0; i < length; i++)
  {
    crc ^= data[i];
    for (int j = 0; j < 8; j++)
    {
      crc = (crc >> 1) ^ ((crc & 1) ? POLYNOMIAL : 0);
    }
  }
  return crc;
}