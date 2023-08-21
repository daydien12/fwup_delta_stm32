#ifndef _HANDLE_MSG_H_
#define _HANDLE_MSG_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include "stm32f10x.h" 
#include "bts_frame_message.h"

extern uint8_t arr_msg_response[];

/*
 * OTA process state
 */
typedef enum
{
  OTA_STATE_START   = 1,
  OTA_STATE_HEADER  = 2,
  OTA_STATE_DATA    = 3,
  OTA_STATE_END     = 4,
}OtaState_e;

typedef struct
{
	uint8_t  cmd;
	uint32_t package_size;
  uint32_t package_crc;
}__attribute__((packed)) uploadMetaData_t;

typedef struct
{
	uint8_t  cmd;
	uint32_t length;
	uint8_t  data[128];
}__attribute__((packed)) uploadData_t;

typedef struct
{
	uint32_t size_file_bin;
	uint32_t count_size_file;
	uint32_t check_sum;
}uploadValueAll_t;

void Handle_GetMsg(const messageFrameMsg_t datain);
void Handle_ResponseMsg(void);

#ifdef __cplusplus
}
#endif

#endif
