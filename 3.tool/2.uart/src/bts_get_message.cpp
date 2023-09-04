
#include "bts_get_message.h"

int16_t length_message = 0;

static void ClearState(void);

static uint16_t Bytes_To_Uint16(uint8_t data1, uint8_t data2)
{
  data_format_uint8_16_t input_convert;
  input_convert.bytes[0] = data1;
  input_convert.bytes[1] = data2;
  return input_convert.data_uint16;
}

/**
   @brief Get the Message:: Get Message objectL: parameters initialization

*/
GetMessage::GetMessage(void)
{
  count_element_arr = 0;
  flag_new_message = false;
  fsm_state = FSM_STATE_START;

  timeout_start = 0;
  timeout_end = 0;
  timeout_wait = false;
}

/**
 * @brief Checks if a message is available in the input buffer.
 *
 * This function checks if a message is available in the input buffer. If a message
 * is available, the function returns true and sets the output parameter `length`
 * to the length of the message. If no message is available, the function returns
 * false and does not modify the value of `length`.
 *
 * @param length A reference to a uint16_t value that will be set to the length of the
 *               message if one is available.
 *
 * @return true if a message is available, false otherwise.
 */
bool GetMessage::IsMessage(uint16_t &length)
{
  DebugLogger Debug;
  if (flag_new_message == true)
  {
    flag_new_message = false;
    length = length_message;
    // Debug.DebugStatus("GetMessage::IsMessage");
    return 1;
  }
  return 0;
}

/**
 * @brief Resets the message receive timeout.
 *
 * This function resets the message receive timeout, causing the `IsMessage` function to
 * wait for a new message until the timeout period elapses. This function is typically
 * called after a message has been successfully received to ensure that the next call to
 * `IsMessage` waits for a new message and does not immediately return false.
 */
void GetMessage::TimeOut(void)
{
  DebugLogger Debug;
  if (timeout_wait == true)
  {
    timeout_end = clock();
    if ((timeout_end - timeout_start) >= 40000)
    // if ((timeout_end - timeout_start) >= 100000)
    {
      length_message = 0;
      flag_new_message = false;
      GetMessage::ClearState();
      Debug.DebugError("GetMessage::TimeOut");
    }
  }
  else
  {
    GetMessage::ClearState();
  }
}

/**
 * @brief Processes incoming bytes of data and extracts complete messages.
 *
 * This function processes incoming bytes of data and extracts complete messages from them.
 * The function takes a single byte of data as input and writes any complete messages found
 * to the output array `arr_message`. The length of each message is determined by the value
 * of the first byte in the message. If the input byte does not result in a complete message,
 * the function returns without modifying the output array.
 *
 * @param datain A single byte of incoming data to be processed.
 * @param arr_message An output array to hold any complete messages found by the function.
 */

void GetMessage::GetMessages(uint8_t datain, uint8_t arr_message[])
{
  DebugLogger Debug;
  timeout_start = clock();
  timeout_wait = true;
  arr_message[count_element_arr] = datain;
  count_element_arr++;
#if ENABLE_DEBUG_GETMESSAGE_FSM
  Debug.DebugFSMMessage(fsm_state, count_element_arr, length_message, flag_new_message, datain);
#endif
  switch (fsm_state)
  {
  case FSM_STATE_START:

    if (count_element_arr == FSM_STATE_CHANGE_VALUE_WAIT)
    {
      if (arr_message[0] != 0xA5)
      {
        GetMessage::ClearState();
      }
      else
      {
        fsm_state = FSM_STATE_WAIT;
      }
    }
    break;

  case FSM_STATE_WAIT:
    if (count_element_arr == FSM_STATE_CHANGE_VALUE_END)
    {
      data_after_length = Bytes_To_Uint16(arr_message[2], arr_message[3]);
      fsm_state = FSM_STATE_END;
    }
    break;

  case FSM_STATE_END:
    if (count_element_arr == (data_after_length + 4))
    {
      flag_new_message = true;
      length_message = count_element_arr;
#if ENABLE_DEBUG_GETMESSAGE_FSM
      Debug.DebugFSMMessage(fsm_state, count_element_arr, length_message, flag_new_message, datain);
#endif
      GetMessage::ClearState();
    }
    break;
  }
}
/**
 * @brief Resets the internal state of the message processor.
 *
 * This function resets the internal state of the message processor, clearing any partially
 * received messages and resetting the receive timeout. This function is typically called
 * when the input stream is reset or when the message processor is otherwise put into an
 * unknown state.
 */
void GetMessage::ClearState(void)
{
  count_element_arr = 0;
  data_after_length = 0;
  timeout_start = 0;
  timeout_end = 0;
  timeout_wait = false;
  fsm_state = FSM_STATE_START;
}
