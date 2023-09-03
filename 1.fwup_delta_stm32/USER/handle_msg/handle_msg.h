#ifndef _HANDLE_MSG_H_
#define _HANDLE_MSG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "delay.h"
#include "stm32f10x.h"
#include "bts_frame_message.h"

#define POLYNOMIAL 0x04C11DB7

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
  uint8_t  cmd;
  uint32_t offset;
} __attribute__((packed)) uploadResponse_t;

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


typedef struct
{
  uint8_t flag;
  uint8_t arr_msg_response[10];
  uint16_t count_time_send;
} responseValueAll_t;

void Handle_GetMsg(const messageFrameMsg_t datain, void(*Uart_SendByte)(char));
void Handle_ResponseMsg(void);
void Handle_InitValueAll(void);

#ifdef __cplusplus
}
#endif

#endif
