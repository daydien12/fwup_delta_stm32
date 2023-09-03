#include "delta.h"
#include "janpatch.h"

//FATFS fs;

static unsigned char source_buf[SIZE_BUFFER];
static unsigned char target_buf[SIZE_BUFFER];
static unsigned char patch_buf [SIZE_BUFFER];

size_t sfio_fread(void* ptr, size_t size, size_t count, sfio_stream_t* stream);
size_t sfio_fwrite(const void *ptr, size_t size, size_t count, sfio_stream_t *stream);
int sfio_fseek(sfio_stream_t *stream, long int offset, int origin);
long sfio_tell(sfio_stream_t *stream);

janpatch_ctx ctx =
{
  {source_buf, SIZE_BUFFER}, 
  {target_buf, SIZE_BUFFER}, 
  {patch_buf , SIZE_BUFFER}, 

  &sfio_fread,
  &sfio_fwrite,
  &sfio_fseek,
  &sfio_tell,
};

//void Delta_Mount(void)
//{
//	res = f_mount(0, &fs);
//  if (res != FR_OK) 
//	{
//    printf("mount filesystem 0 failed : %d\n\r", res);
//  }
//}

void Delta_Run(char *name_old_file, char *name_patch_file, char *name_new_file)
{
  sfio_stream_t sources;
  sfio_stream_t patchs;
  sfio_stream_t targets;

  sources.name_file = name_old_file;
  sources.offset    = 0;
  sources.size      = SD_getFileSize(name_old_file);

  patchs.name_file  = name_patch_file;
  patchs.offset     = 0;
  patchs.size       = SD_getFileSize(name_patch_file);;

  targets.name_file = name_new_file;
  targets.offset    = 0;
  targets.size      = (sources.size + patchs.size);

  janpatch(ctx, (FIL*)&sources,  (FIL*)&patchs,  (FIL*)&targets);
	printf("Kich thuoc file %s la %ld bytes\n", (char*)name_old_file, SD_getFileSize(name_old_file));
  printf("Kich thuoc file %s la %ld bytes\n", (char*)name_patch_file , SD_getFileSize(name_patch_file));
	printf("Kich thuoc file %s la %ld bytes\n", (char*)name_new_file, SD_getFileSize(name_new_file));
}

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

int sfio_fseek(sfio_stream_t *stream, long int offset, int origin)
{
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

long sfio_tell(sfio_stream_t *stream)
{
  long ret = -1;
  if (stream == NULL)
  {
    return ret;
  }
  return stream->offset;
}
