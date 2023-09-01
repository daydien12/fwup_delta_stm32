
#ifndef _BTS_DEBUG_H_
#define _BTS_DEBUG_H_

#include "main.h"
/*DEBUG for file bts_get_message*/
#define ENABLE_DEBUG_GETMESSAGE_FSM 0

/*DEBUG for file bts_frame_message*/
#define ENABLE_DEBUG_FRAME_MESSAGE_ERROR 0
#define ENABLE_DEBUG_FRAME_MESSAGE_STATUS 1

/*DEBUG for file bts_system*/
#define ENABLE_DEBUG_SYSTEM_ERROR 1
#define ENABLE_DEBUG_SYSTEM_STATUS 1

/*DEBUG for file bts_system*/
#define ENABLE_DEBUG_MODBUS_ERROR 1
#define ENABLE_DEBUG_MODBUS_STATUS 1

#define DEBUG 1
#define MQTT_DEBUG 0



class DebugLogger
{
public:
    void DebugFrameMessage(uint8_t *dataint);
    void DebugFSMMessage(uint16_t state, uint16_t count_element_arr, int16_t length_message, uint8_t Flags, uint8_t data);
    void DebugError(const char *str);
    void DebugStatus(const char *str);
    void DebugArrSensor(float *arr, uint8_t sizearr);
    void DebugArrDevice(uint8_t *arr, uint8_t sizearr);
    void DebugLOG(int header, std::vector<std::string> content);
public:
};

#endif 
