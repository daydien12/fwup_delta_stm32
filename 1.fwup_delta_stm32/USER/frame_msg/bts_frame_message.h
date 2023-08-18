
#ifndef _FRAME_MESSAGE_H
#define _FRAME_MESSAGE_H

#ifdef __cplusplus
    extern "C" {
#endif

#include <stdint.h>
#include "stm32f10x.h" 
#include "bts_convert.h"

#define RETRUN_ERROR -1

#define START_BYTE 0xA5

/*Start(1byte) + TypeMessage(1byte) + Length(2byte) = 4byte */
#define DEFAULT_BYTE                            4

#define DEFAULT_BYTE_CHECKSUM                   2																//  2byte checksum
#define DEFAULT_MAX_SIZE_DATA                   130  	                          //  support max 10 device

/**
 * @brief structure of frame message
 * 
 */
typedef struct
{
	uint8_t   Start;
	uint8_t   TypeMessage;
	uint16_t  Length;
	uint8_t   Data[DEFAULT_MAX_SIZE_DATA];
	uint16_t  Crc;
}messageFrameMsg_t;

typedef enum
{
	TYPE_MESSAGE_UPDATE_SENSOR 	    = 0x0001,
	TYPE_MESSAGE_UPDATE_DEVICE 	    = 0x0002,
	TYPE_MESSAGE_CONTROL_DEVICE 	  = 0x0003,
}typeMessageSensor_e;

int16_t DetectMessage(uint8_t *dataint, messageFrameMsg_t *dataout);
int16_t CreateMessage(uint8_t type_msg, uint16_t Length, uint8_t *datain, uint8_t *dataout);

int16_t DebugMessage(uint8_t *dataint);
#ifdef __cplusplus
}
#endif

#endif 

