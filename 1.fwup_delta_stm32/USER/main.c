
/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include "usart.h"
#include "ff.h"
#include "lib_sd.h"
#include "SPI_MSD0_Driver.h"
#include <string.h>
#include "janpatch.h"

FATFS fs;             /* Khu vực làm việc (hệ thống tệp) cho ổ đĩa logic */

//#define FILE_OLD    "demo/blinky-k64f-old.bin"
//#define FILE_PATCH  "demo/blinky-k64f.patch"
//#define FILE_CREATE "demo/bien1.bin"

#define FILE_OLD    "demo/gpio_new.bin"
#define FILE_PATCH  "demo/gpio.patch"
#define FILE_CREATE "demo/gpio.bin"


static unsigned char source_buf[1024];
static unsigned char target_buf[1024];
static unsigned char patch_buf [1024];

uint16_t counts = 0;

typedef struct
{
  char* name_file;
  size_t offset;
  size_t size;
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
	
	printf("Kich thuoc file %s la %ld bytes\n", (char*)FILE_CREATE, SD_getFileSize(FILE_OLD));
  printf("Kich thuoc file %s la %ld bytes\n", (char*)FILE_PATCH , SD_getFileSize(FILE_PATCH));
	
  sources.name_file = FILE_OLD;
  sources.offset    = 0;
  sources.size      = SD_getFileSize(FILE_OLD);

  patchs.name_file = FILE_PATCH;
  patchs.offset    = 0;
  patchs.size      = SD_getFileSize(FILE_PATCH);;

  targets.name_file = FILE_CREATE;
  targets.offset    = 0;
  targets.size      = 100000;

  printf("Run\n");
  janpatch(ctx, (FIL*)&sources,  (FIL*)&patchs,  (FIL*)&targets);
  printf("Done: \n");
	printf("Kich thuoc file %s la %ld bytes\n", (char*)FILE_CREATE, SD_getFileSize(FILE_CREATE));
  while (1)
  {

  }
}

