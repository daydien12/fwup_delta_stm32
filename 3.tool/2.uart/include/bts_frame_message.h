
#ifndef _FRAME_MESSAGE_H_
#define _FRAME_MESSAGE_H_


#include "main.h"


#define START_BYTE 0xA5

/*Start(1byte) + TypeMessage(1byte) + Length(2byte) = 4byte */
#define DEFAULT_BYTE                            4

#define DEFAULT_BYTE_CHECKSUM                   2																//  2byte checksum
#define DEFAULT_MAX_SIZE_DATA                   140  	                         

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
	TYPE_MSG_MODE_BOTLOADER 	  = 0x01,
	TYPE_MSG_UPDATE_FILE 	      = 0x02,
	TYPE_MSG_DELTA 	            = 0x03,
	TYPE_MSG_UPDATE_FLASH       = 0x04,
	TYPE_MSG_MODE_APP           = 0x05,
}typeMessageSensor_e;


class FrameMessage
{
    
public:
	int16_t CreateMessage(uint8_t type_msg, uint16_t length, uint8_t *datain, uint8_t *dataout);
    int16_t DetectMessage(uint8_t *dataint, messageFrameMsg_t *dataout);
private:
	uint16_t CheckSum(uint8_t *buf, uint8_t len);
};

#endif 
