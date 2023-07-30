

#ifndef _DELTA_
#define _DELTA_


#ifdef __cplusplus
extern "C"{
#endif
#define SIZE_BUFFER 4096
#include <stdio.h>
#include "usart.h"
#include "lib_sd.h"
typedef struct
{
  char* name_file;
  size_t offset;
  size_t size;
} sfio_stream_t;

extern FATFS fs;

void Delta_Mount(void);
void Delta_Run(char *name_old_file, char *name_patch_file, char *name_new_file);

#ifdef __cplusplus
}
#endif

#endif


