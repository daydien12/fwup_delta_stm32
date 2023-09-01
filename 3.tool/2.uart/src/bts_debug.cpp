#include "bts_debug.h"

/**
 * @brief Converts two bytes into a 16-bit unsigned integer.
 *
 * This function takes in two uint8_t arguments, data1 and data2, which represent the
 * two bytes that need to be combined into a single 16-bit unsigned integer. The function
 * shifts data2 8 bits to the left and then ORs the result with data1 to combine the two
 * bytes into a single 16-bit value. This value is then returned.
 *
 * @param data1 The least significant byte of the 16-bit value.
 * @param data2 The most significant byte of the 16-bit value.
 * @return The combined 16-bit unsigned integer value.
 */
static uint16_t Bytes_To_Uint16(uint8_t data1, uint8_t data2)
{
  data_format_uint8_16_t input_convert;
  input_convert.bytes[0] = data1;
  input_convert.bytes[1] = data2;

  return input_convert.data_uint16;
}

/**
 * @brief Converts four bytes into a 32-bit floating point value.
 *
 * This function takes in four uint8_t arguments, data0, data1, data2, and data3, which
 * represent the four bytes that need to be combined into a single 32-bit floating point
 * value. The function first creates a uint32_t variable by shifting and ORing the input
 * bytes into the appropriate positions, then casts this variable as a float and returns
 * the result.
 *
 * @param data0 The first (least significant) byte of the 32-bit float.
 * @param data1 The second byte of the 32-bit float.
 * @param data2 The third byte of the 32-bit float.
 * @param data3 The fourth (most significant) byte of the 32-bit float.
 * @return The 32-bit floating point value.
 */
static float Bytes_To_Float(uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3)
{
  data_format_float_bytes input_convert;
  input_convert.bytes[3] = data0;
  input_convert.bytes[2] = data1;
  input_convert.bytes[1] = data2;
  input_convert.bytes[0] = data3;
  return input_convert.data_float;
}

/**
 * @brief Logs an error message to the debug output.
 *
 * This function writes the given error message to the debug output, along with a
 * timestamp and any other relevant information. The message is prefixed with the
 * string "[ERROR]" to make it clear that it is an error message.
 *
 * @param str The error message to log.
 */
void DebugLogger::DebugError(const char *str)
{
  printf("!!!!!!!!!!!(Error: %s)!!!!!!!!!!!\n", str);
}

/**
 * @brief Logs a status message to the debug output.
 *
 * This function writes the given status message to the debug output, along with a
 * timestamp and any other relevant information. The message is prefixed with the
 * string "[STATUS]" to make it clear that it is a status message.
 *
 * @param str The status message to log.
 */
void DebugLogger::DebugStatus(const char *str)
{
  printf("|||||||||||||(Status: %s)|||||||||||||\n", str);
}

/**
 * @brief Logs a frame message to the debug output.
 *
 * This function writes the given frame message data to the debug output, along with a
 * timestamp and any other relevant information. The message is formatted as a series
 * of comma-separated integers, with each integer representing one byte of the input
 * message data.
 *
 * @param dataint Pointer to an array of bytes representing the frame message data.
 */
// void DebugLogger::DebugFrameMessage(uint8_t *dataint)
// {
//   messageFrameMsg_t dataout;
//   FrameMessage framemessage;
//   int16_t size_data = framemessage.DetectMessage(dataint, &dataout);
//   printf("\n--------------------(Size: %d)--------------------\n", size_data);
//   printf("Start       : %x\n", dataout.Start);
//   printf("TypeMessage : %x - %s\n", dataout.TypeMessage, TypeMessageName[dataout.TypeMessage]);
//   if ((dataout.TypeMessage == TYPE_MESSAGE_UPDATE_SENSOR) )
//   {
//     uint16_t count_arr = 0;
//     for (int i = 0; i < ((size_data - 8) / 4); i++)
//     {
//       printf("data[%d]     : %f \n", i, Bytes_To_Float(dataout.Data[count_arr], dataout.Data[count_arr + 1], dataout.Data[count_arr + 2], dataout.Data[count_arr + 3]));
//       count_arr += 4;
//     }
//   }
//   else if (dataout.TypeMessage == TYPE_MESSAGE_UPDATE_DEVICE)
//   {
//     for (int i = 0; i < ((size_data - 8)); i++)
//     {
//       printf("data[%d]     : %d \n", i, (dataout.Data[i]));
//     }
//   }
//   else
//   {
//     printf("Name DV     : %d\n", dataout.Data[0]);
//     printf("Valuce      : %d\n", dataout.Data[1]);
//   }
//   printf("Check sum   : %x\n", dataout.Crc);
// }

/**
 * @brief Logs a FSM message to the debug output.
 *
 * This function writes a message describing the current state of the FSM, along with
 * any relevant information such as the number of elements in an array, the length of
 * a message, or any flags associated with the current state. The message is formatted
 * as a series of comma-separated values, with each value representing one piece of
 * information.
 *
 * @param state The current state of the FSM.
 * @param count_element_arr The number of elements in an array associated with the current state.
 * @param length_message The length of a message associated with the current state.
 * @param Flags Any flags associated with the current state.
 * @param data Any additional data associated with the current state.
 */
void DebugLogger::DebugFSMMessage(uint16_t state, uint16_t count_element_arr, int16_t length_message, uint8_t Flags, uint8_t data)
{
  switch (state)
  {
    case FSM_STATE_START:
      printf("state: FSM_STATE_START, element: %d, length_message: %d, Flags: %d, ", count_element_arr, length_message, Flags);
      break;
    case FSM_STATE_WAIT:
      printf("state: FSM_STATE_WAIT, element: %d, length_message: %d, Flags: %d, ", count_element_arr, length_message, Flags);
      break;
    case FSM_STATE_END:
      printf("state: FSM_STATE_END, element: %d, length_message: %d, Flags: %d, ", count_element_arr, length_message, Flags);
      break;
    default:
      break;
  }

  if (data <= 0x0f)
  {
    printf("data: 0%x\n", data);
  }
  else
  {
    printf("data: %x\n", data);
  }
}

/**
 * @brief Logs an array of sensor data to the debug output.
 *
 * This function writes the given array of sensor data to the debug output, along with
 * a timestamp and any other relevant information. The message is formatted as a series
 * of comma-separated floating-point values, with each value representing one element
 * of the input array.
 *
 * @param arr Pointer to an array of floating-point values representing the sensor data.
 * @param sizearr The number of elements in the input array.
 */
void DebugLogger::DebugArrSensor(float *arr, uint8_t sizearr)
{
  for (int i = 0; i < sizearr; i++)
  {
    printf("%f ", arr[i]);
  }
}

/**
 * @brief Logs an array of device data to the debug output.
 *
 * This function writes the given array of device data to the debug output, along with
 * a timestamp and any other relevant information. The message is formatted as a series
 * of comma-separated integers, with each integer representing one element of the input
 * array.
 *
 * @param arr Pointer to an array of integers representing the device data.
 * @param sizearr The number of elements in the input array.
 */
void DebugLogger::DebugArrDevice(uint8_t *arr, uint8_t sizearr)
{
  for (int i = 0; i < sizearr; i++)
  {
    printf("%d ", arr[i]);
  }
}

