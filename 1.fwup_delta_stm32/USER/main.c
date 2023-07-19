
/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include "usart.h"
#include "ff.h"
#include "lib_sd.h"
#include "SPI_MSD0_Driver.h"
#include <string.h>
#include "janpatch.h"
FATFS fs;             /* Khu vực làm việc (hệ thống tệp) cho ổ đĩa logic */
FIL fsrc, fdst;      /* file objects */
UINT br, bk;

uint8_t textFileBuffer[] = "\r\nChinese and English test strings Nguyen Qunag Bien 1234\r\n";

#define FILE_OLD    "demo/blinky-k64f-old.bin"
#define FILE_PATCH  "demo/blinky-k64f.patch"
#define FILE_CREATE "demo/bien1.bin"

static unsigned char source_buf[1024];
static unsigned char target_buf[1024];
static unsigned char patch_buf [1024];

uint16_t counts = 0;

typedef struct 
{
	char* name_file;
  size_t offset;
  size_t size;
	int currentPosition;
} sfio_stream_t;


size_t sfio_fread(void* ptr, size_t size, size_t count, sfio_stream_t* stream)
{
	FIL fsrcs;
	UINT brs;
  size_t bytesToRead;
	f_open(&fsrcs, stream->name_file,  FA_OPEN_EXISTING | FA_READ);
	
	bytesToRead = size * count;

  if (stream->offset + count > stream->size)
  {
    bytesToRead = stream->size - stream->offset;
  }
	
	f_lseek(&fsrcs, stream->offset);
	f_read(&fsrcs, ptr, bytesToRead, &brs);
	f_close(&fsrcs);
  return bytesToRead;
}

size_t sfio_fwrite(const void *ptr, size_t size, size_t count, sfio_stream_t *stream)
{
	FIL fdsts;
	UINT brs;
  size_t bytesToRead;
	
	f_open(&fdsts, stream->name_file, FA_OPEN_ALWAYS | FA_WRITE);
	bytesToRead = size * count;

  if (stream->offset + count > stream->size) 
	{
    bytesToRead = stream->size - stream->offset;
  }

	f_lseek(&fdsts, stream->offset);
	f_write(&fdsts, ptr, bytesToRead, &brs);
	f_close(&fdsts);
  return bytesToRead;
}


int sfio_fseek(sfio_stream_t *stream, long int offset, int origin) {
	
  if (offset > stream->size) 
	{
    return -1;
  } 
	else 
	{
    stream->offset = offset;
  }
  return 0;
}

janpatch_ctx ctx = {
	{source_buf, 1024}, // source buffer
	{target_buf, 1024}, // patch buffer
	{patch_buf , 1024}, // target buffer

	&sfio_fread,
	&sfio_fwrite,
	&sfio_fseek,
	NULL,
};

int main(void)
{
	sfio_stream_t sources;
  sfio_stream_t patchs;
  sfio_stream_t targets;
	
	FIL source, patch, target;
	
  SystemInit();                   
  db_DEBUG_Init(9600);          
  MSD0_SPI_Configuration();         
	
  res = f_mount(0, &fs);       
  if (res != FR_OK) {
    printf("mount filesystem 0 failed : %d\n\r", res);
  }
	
	sources.name_file = FILE_OLD;
  sources.offset    = 0;
  sources.size      = 47352;
	sources.currentPosition = 0;
	
  patchs.name_file = FILE_PATCH;
  patchs.offset    = 0;
  patchs.size      = 8580;
	patchs.currentPosition = 0;
	
  targets.name_file = FILE_CREATE;
  targets.offset    = 0;
  targets.size      = 100000;
	targets.currentPosition = 0;
	printf("Run\n");
  janpatch(ctx, (FIL*)&sources,  (FIL*)&patchs,  (FIL*)&targets);
	//printf("Kich thuoc file %s la %d bytes\n", (char*)FILE_CREATE, SD_getFileSize(FILE_CREATE)); 
	printf("Done: \n");
  while(1)
	{

  }
}
