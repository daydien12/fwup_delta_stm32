#include "bts_get_message.h"

int16_t 		length_message = 0;
uint8_t			flag_new_message;
uint16_t		count_element_arr;
uint16_t		data_after_length;

int32_t timeout_start;
int32_t timeout_wait;
fsmListState_e	fsm_state;
static void ClearState(void);

/**
 * @brief : Flag of the new message
 * 
 * @param lenght : Length of the new message
 * @return uint16_t : Return 1 if received successfully message, else return 0
 */
uint16_t Is_Message(uint16_t *lenght)
{
	if(flag_new_message == TRUE)
	{
		timeout_wait = FALSE;
		flag_new_message = FALSE;
		*lenght = length_message;
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * @brief Get the Message:: Time Out object
 * time to receive the new message 
 * used to avoid the case where the message is long or the message is shorter than the message length
 */
void Time_Out_Get_Message(void)
{
	if(timeout_wait == TRUE)
	{  
		if(timeout_start >= COUNTER_TIMEOUT)
		{		
			length_message = 0;
			ClearState();
		}
		timeout_start++;
	}
	else
	{
		ClearState();
	}
}

/**
 * @brief Get the message from message buffer received from serial port
 * @param datain : One byte data receive
 * @param arr_message : Array data out of message
 */
void BTS_Get_Message(uint8_t datain, uint8_t arr_message[])
{
	timeout_start = 0;
	timeout_wait = TRUE;
	
	switch (fsm_state)
	{
		case FSM_STATE_START:
			arr_message[count_element_arr] = datain;
			count_element_arr++;
		
			if (count_element_arr == FSM_STATE_CHANGE_VALUE_WAIT)
			{
				if(Bts_Convert_From_Bytes_To_Uint16(arr_message[0], arr_message[1]) != 0xAA55)
				{
					ClearState();
				}
				else
				{
					fsm_state = FSM_STATE_WAIT;
				}
			}
		break;
		
		case FSM_STATE_WAIT:
			arr_message[count_element_arr] = datain;
			count_element_arr++;
			if (count_element_arr == FSM_STATE_CHANGE_VALUE_END)
			{
				data_after_length = Bts_Convert_From_Bytes_To_Uint16(arr_message[4], arr_message[5]);
				fsm_state = FSM_STATE_END;
			}
		break;
		
		case FSM_STATE_END:
			arr_message[count_element_arr] = datain;
			count_element_arr++;
			if (count_element_arr == (data_after_length + DEFAULT_BYTE))
			{
				flag_new_message = TRUE;
				length_message = count_element_arr;
				ClearState();
			}
		break;
	}
}

/**
 * @brief Used to reset elements when get message successfully or timeout.
 * 
 */
void ClearState(void)
{
	count_element_arr = 0;
	data_after_length = 0;
    timeout_start = 0;
	timeout_wait = FALSE;
    fsm_state = FSM_STATE_START;
}
