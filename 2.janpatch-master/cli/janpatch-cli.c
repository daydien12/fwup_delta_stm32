

#include <stdio.h>
#include "janpatch.h"

#define FILE_OLD    "demo/blinky-k64f-old.bin"
#define FILE_PATCH  "demo/blinky-k64f.patch"
#define FILE_CREATE "./blinky-k64f-patched.bin"

#define SIZE_BUFFER 1024
static unsigned char source_buf[SIZE_BUFFER];
static unsigned char target_buf[SIZE_BUFFER];
static unsigned char patch_buf[SIZE_BUFFER];

typedef struct {
  uint8_t *file;
  size_t offset;
  size_t size;
} sfio_stream_t;

size_t sfio_fread(void* ptr, size_t size, size_t count, sfio_stream_t* stream)
{
  size_t bytesToRead = size * count;

  if (stream->offset + count > stream->size)
  {
    bytesToRead = stream->size - stream->offset;
  }
  fread(ptr, bytesToRead, 1, (FILE*)stream->file);
  return bytesToRead;
}

size_t sfio_fwrite(const void *ptr, size_t size, size_t count, sfio_stream_t *stream)
{
  size_t bytesToRead = size * count;
  if (stream->offset + count > stream->size) {
    count = stream->size - stream->offset;
  }
  fwrite(ptr, bytesToRead, 1, (FILE*)stream->file);
  return bytesToRead;
}

int sfio_fseek(sfio_stream_t *stream, long int offset, int origin) {
  if (offset > stream->size) {
    return -1;
  } else {
    stream->offset = offset;
  }
  return 0;
}


long int findSize(char file_name[])
{
  // opening the file in read mode
  FILE* fp = fopen(file_name, "r");

  // checking if the file exist or not
  if (fp == NULL) {
    printf("File Not Found!\n");
    return -1;
  }

  fseek(fp, 0L, SEEK_END);

  // calculating the size of the file
  long int res = ftell(fp);

  // closing the file
  fclose(fp);

  return res;
}


int main() {
  sfio_stream_t sources;
  sfio_stream_t patchs;
  sfio_stream_t targets;

  printf("%s \n", FILE_OLD);
  printf("%s \n", FILE_PATCH);
  printf("%s \n", FILE_CREATE);

  FILE *old = fopen(FILE_OLD, "rb");
  FILE *patch = fopen(FILE_PATCH, "rb");
  FILE *target =  fopen(FILE_CREATE, "wb");

  if (!old) {
    printf("Could not open '%s'\n", FILE_OLD);
    return 1;
  }

  if (!patch) {
    printf("Could not open '%s'\n", FILE_PATCH);
    return 1;
  }
  if (!target) {
    printf("Could not open '%s'\n", FILE_CREATE);
    return 1;
  }

  sources.file   = (uint8_t*)old;
  sources.offset = 0;
  sources.size   =  findSize(FILE_OLD);

  patchs.file   = (uint8_t*)patch;
  patchs.offset = 0;
  patchs.size   =  findSize(FILE_PATCH);

  targets.file   = (uint8_t*)target;
  targets.offset = 0;
  targets.size   =  0;

  janpatch_ctx ctx = {
    { source_buf, SIZE_BUFFER, "OLD" }, // source buffer
    { target_buf, SIZE_BUFFER,  "PATCH"}, // patch buffer
    { patch_buf, SIZE_BUFFER, "TARGET" }, // target buffer

    &sfio_fread,
    &sfio_fwrite,
    &sfio_fseek,
    NULL,
  };

  janpatch(ctx, &sources,  &patchs,  &targets);
}


