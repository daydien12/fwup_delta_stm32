
#ifndef _HANDLE_MSG_H_
#define _HANDLE_MSG_H_

#include "main.h"

#define LENGTH_SEND 128

/*
   OTA process state
*/
typedef enum
{
  OTA_STATE_START   = 1,
  OTA_STATE_DATA    = 2,
  OTA_STATE_END     = 3,
  OTA_STATE_DELETE  = 4,
} OtaState_e;

typedef struct
{
  uint8_t  cmd;
  uint32_t package_size;
  uint32_t package_crc;
  char name[50];
} __attribute__((packed)) uploadMetaData_t;

typedef struct
{
  uint8_t  cmd;
  uint8_t  length;
  uint32_t offset;
  uint8_t  data[128];
} __attribute__((packed)) uploadData_t;

typedef struct
{
  uint8_t cmd;
  char name[50];
} __attribute__((packed)) uploadDeleteFile_t;

typedef struct
{
  char name_old[30];
	char name_patch[30];
	char name_create[30];
} __attribute__((packed)) deltaDiff_t;


class HandleMsg
{

public:
  uint32_t Handle_UpdateFile(const char *name_file_bin, const char *port, const uint8_t mode, const char *name_file, uint8_t &state,  uint32_t &offset, uint32_t &size_file_bin);
  uint32_t Handle_DeltaUpdate(const char *port, const char *name_old, const char *name_patch, const char *name_create);
};

#endif 
