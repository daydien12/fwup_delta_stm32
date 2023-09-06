#include "bts_convert.h"

/**
    @brief This function takes an float value data and returns a pointer to an array of 4 bytes. It is used to 
    represent the given float in little-endian format, with the least significant byte first.
    @param data the float value to be converted to bytes.
    @returnA pointer to an array of 4 bytes that represent the given float in little-endian order.
*/
uint8_t *Bts_Convert_From_Float_To_Bytes(float data)
{
    static data_format_float_bytes input_convert;
    input_convert.data_float = data;
    return input_convert.bytes;
}

/**
    @brief: This function converts four bytes of data (data0, data1, data2, data3) into a single floating-point 
	number and returns it as a float. The order of the bytes is important and must match the type of encoding/decoding 
	used to create/send/receive them.
    @param data0: A byte of data, representing the most significant (left-most) 8 bits of the converted float.
    @param data1: A byte of data, representing the second most significant 8 bits of the converted float.
    @param data2: A byte of data, representing the second least significant 8 bits of the converted float.
    @param data3: A byte of data, representing the least significant (right-most) 8 bits of the converted float.
    @retval: A single float value that contains the converted data from the four input bytes..
*/
float Bts_Convert_From_Bytes_To_Float(uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3)
{
    data_format_float_bytes input_convert;
    input_convert.bytes[3] = data0;
    input_convert.bytes[2] = data1;
    input_convert.bytes[1] = data2;
    input_convert.bytes[0] = data3;
     return input_convert.data_float;
}

/**
    @brief This function takes an integer value data and returns a pointer to an array of 4 bytes. It is used to 
    represent the given integer in little-endian format, with the least significant byte first.
    @param data the integer value to be converted to bytes.
    @returnA pointer to an array of 4 bytes that represent the given integer in little-endian order.
*/
uint8_t *Bts_Convert_From_Int_To_Bytes(int data)
{
    static data_format_int_bytes input_convert;
    input_convert.data_int = data;
    return input_convert.bytes;
}

/**
    @brief: This function converts four bytes of data (data0, data1, data2, data3) into a single int number and 
	returns it as a int. The order of the bytes is important and must match the type of encoding/decoding used 
	to create/send/receive them.
    @param data0: A byte of data, representing the most significant (left-most) 8 bits of the converted float.
    @param data1: A byte of data, representing the second most significant 8 bits of the converted float.
    @param data2: A byte of data, representing the second least significant 8 bits of the converted float.
    @param data3: A byte of data, representing the least significant (right-most) 8 bits of the converted float.
    @retval: A single int value that contains the converted data from the four input bytes..
*/
uint32_t Bts_Convert_From_Bytes_To_Int(uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4)
{
    data_format_int_bytes input_convert;
    input_convert.bytes[3] = data1;
    input_convert.bytes[2] = data2;
    input_convert.bytes[1] = data3;
    input_convert.bytes[0] = data4;
    return input_convert.data_int;
}

/**
    @brief: This function takes an unsigned 16-bit integer as input and converts it into an array of two bytes which 
    can be used to transmit the information over a network or save it in memory.
    @param data: The unsigned 16-bit integer that needs to be converted to an array of bytes.
    @retval: A pointer to an array of 2 bytes. This array contains the two bytes corresponding to the input unsigned 16-bit integer.
*/
uint8_t *Bts_Convert_From_Uint16_To_Bytes(int data)
{
    static data_format_uint8_16_t input_convert;
    input_convert.data_uint16 = data;
    return input_convert.bytes;
}

/**
    @brief: The function takes in two uint8_t data as input, which represents the byte stream of the 2-byte unsigned integer. 
    The function then converts these bytes back to their corresponding unsigned integer representation.
    @param data0: A uint8_t data representing the low byte of the 2-byte stream.
    @param data1: A uint8_t data representing the high byte of the 2-byte stream.
    @retval:  A single unsigned 16-bit value that contains the converted data from the two input bytes..
*/
uint16_t Bts_Convert_From_Bytes_To_Uint16(uint8_t data1, uint8_t data2)
{
    data_format_uint8_16_t input_convert;
    input_convert.bytes[0] = data1;
    input_convert.bytes[1] = data2;

    return input_convert.data_uint16;
}
