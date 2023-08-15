#ifndef _GETMESSAGE_H
#define _GETMESSAGE_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include "stm32f10x.h" 
#include "bts_convert.h"
#include "usart.h"

#define FSM_STATE e 
#define  Array_Out_Size 50

#define TRUE 1
#define FALSE 0
#define COUNTER_UPDATE_DATA 		5000 	
#define COUNTER_SMOKE_WARNING 		20 	
#define COUNTER_TIMEOUT 	20 	

#define DEFAULT_BYTE                            6 


typedef enum
{
	FSM_STATE_START 	= 0,
	FSM_STATE_WAIT 		= 1,
	FSM_STATE_END 		= 2,
} fsmListState_e;

typedef enum
{
	FSM_STATE_CHANGE_VALUE_START_FRAME	= 0,
	FSM_STATE_CHANGE_VALUE_WAIT 		= 4,
	FSM_STATE_CHANGE_VALUE_END 			= 6,
} fsmValueNextStep_e;

extern int32_t timeout_wait;

uint16_t Is_Message(uint16_t *lenght);
void BTS_Get_Message(uint8_t datain, uint8_t arr_message[]);
void Time_Out_Get_Message(void);
#ifdef __cplusplus
}
#endif

#endif
