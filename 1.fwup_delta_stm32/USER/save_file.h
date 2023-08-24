

#ifndef _SAVEFILE_
#define _SAVEFILE_


#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f10x.h"

#define ETX_OTA_DATA_MAX_SIZE   (1024)  //Maximum data Size
#define ETX_OTA_DATA_OVERHEAD   (9)  //data overhead
#define ETX_OTA_PACKET_MAX_SIZE (ETX_OTA_DATA_MAX_SIZE + ETX_OTA_DATA_OVERHEAD)

#define ETX_OTA_SOF              0xAA    // Start of Frame
#define ETX_OTA_EOF              0xBB    // End of Frame
#define ETX_OTA_ACK              0x00    // ACK
#define ETX_OTA_NACK             0x01    // NACK

/*
   Exception codes
*/
typedef enum
{
  ETX_OTA_EX_OK       = 0,    // Success
  ETX_OTA_EX_ERR      = 1,    // Failure
} etx_OtaException_e;

/*
   OTA process state
*/
typedef enum
{
  ETX_OTA_STATE_START   = 1,
  ETX_OTA_STATE_HEADER  = 2,
  ETX_OTA_STATE_DATA    = 3,
  ETX_OTA_STATE_END     = 4,
} etx_OtaState_e;


/*
   OTA FSM get Frame data
*/
typedef enum
{
  ETX_OTA_FRAME_START   = 0,
  ETX_OTA_FRAME_TYPE    = 1,
  ETX_OTA_FRAME_LENGTH  = 2,
  ETX_OTA_FRAME_DATA    = 3,
  ETX_OTA_FRAME_CRC     = 4,
  ETX_OTA_FRAME_END     = 5,
} etx_OtaFrameData_e;


/*
   Packet type
*/
typedef enum
{
  ETX_OTA_PACKET_TYPE_CMD       = 0,    // Command
  ETX_OTA_PACKET_TYPE_DATA      = 1,    // Data
  ETX_OTA_PACKET_TYPE_HEADER    = 2,    // Header
  ETX_OTA_PACKET_TYPE_RESPONSE  = 3,    // Response
} etx_OtaPacketType_e;


/*
   OTA Commands
*/
typedef enum
{
  ETX_OTA_CMD_START = 0,    // OTA Start command
  ETX_OTA_CMD_END   = 1,    // OTA End command
  ETX_OTA_CMD_ABORT = 2,    // OTA Abort command
} etx_OtaCommands_e;


/*
   OTA meta info
*/
typedef struct
{
  uint32_t package_size;
  uint32_t package_crc;
  uint32_t reserved1;
  uint32_t reserved2;
} __attribute__((packed)) meta_info;


/*
   OTA Command format

   ________________________________________
   |     | Packet |     |     |     |     |
   | SOF | Type   | Len | CMD | CRC | EOF |
   |_____|________|_____|_____|_____|_____|
     1B      1B     2B    1B     4B    1B
*/
typedef struct
{
  uint8_t   sof;
  uint8_t   packet_type;
  uint16_t  data_len;
  uint8_t   cmd;
  uint32_t  crc;
  uint8_t   eof;
} __attribute__((packed)) ETX_OTA_COMMAND_t;


/*
   OTA Header format

   ___________________________________________
   |     | Packet |     | Header |     |     |
   | SOF | Type   | Len |  Data  | CRC | EOF |
   |_____|________|_____|________|_____|_____|
     1B      1B     2B     16B     4B    1B
*/
typedef struct
{
  uint8_t     sof;
  uint8_t     packet_type;
  uint16_t    data_len;
  meta_info   meta_data;
  uint32_t    crc;
  uint8_t     eof;
} __attribute__((packed)) ETX_OTA_HEADER_t;

/*
   OTA Data format

   __________________________________________
   |     | Packet |     |        |     |     |
   | SOF | Type   | Len |  Data  | CRC | EOF |
   |_____|________|_____|________|_____|_____|
     1B      1B     2B    nBytes   4B    1B
*/
typedef struct
{
  uint8_t     sof;
  uint8_t     packet_type;
  uint16_t    data_len;
  uint8_t     *data;
} __attribute__((packed)) ETX_OTA_DATA_t;

/*
   OTA Response format

   __________________________________________
   |     | Packet |     |        |     |     |
   | SOF | Type   | Len | Status | CRC | EOF |
   |_____|________|_____|________|_____|_____|
     1B      1B     2B      1B     4B    1B
*/
typedef struct
{
  uint8_t   sof;
  uint8_t   packet_type;
  uint16_t  data_len;
  uint8_t   status;
  uint32_t  crc;
  uint8_t   eof;
} __attribute__((packed)) ETX_OTA_RESP_t;

typedef struct
{
  uint8_t Buffer[ETX_OTA_PACKET_MAX_SIZE];
  uint16_t Index;
  uint16_t Length;
  uint8_t FlagFrame;
} ETX_OTA_VR_t;

typedef union
{
  uint8_t Data_In[2];
  uint16_t Data_Out;
} u8Tou16_u;

typedef union
{
  uint8_t Data_In[4];
  uint32_t Data_Out;
} u8Tou32_u;

extern ETX_OTA_VR_t ota_data;

void ETX_ReceiveChunk(void);
void ETX_Run(void);

#ifdef __cplusplus
}
#endif

#endif


