
#ifndef _GETMESSAGE_H_
#define _GETMESSAGE_H_

#include "main.h"

#define FSM_STATE 5 
#define  Array_Out_Size 50

typedef enum
{
	FSM_STATE_START 	= 0,
	FSM_STATE_WAIT 		= 1,
	FSM_STATE_END 		= 2,
} fsmListState_e;

typedef enum
{
	FSM_STATE_CHANGE_VALUE_START_FRAME	= 0,
	FSM_STATE_CHANGE_VALUE_WAIT 		= 2,
	FSM_STATE_CHANGE_VALUE_END 			= 4,
} fsmValueNextStep_e;

class GetMessage
{

public:
    GetMessage(void);
	void GetMessages(uint8_t datain, uint8_t arr_message[]);
	bool IsMessage(uint16_t &length);
	void TimeOut(void);
private:
	bool 			flag_new_message;
	uint16_t		count_element_arr;
	uint16_t		data_after_length;
	fsmListState_e	fsm_state;

	clock_t timeout_start;
	clock_t timeout_end;
	bool timeout_wait;
	void ClearState(void);
};

#endif 
