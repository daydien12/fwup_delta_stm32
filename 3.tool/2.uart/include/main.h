/**
 * @file main.h
 * @author long (longzil193@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-02-19
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef _MAIN_H_
#define _MAIN_H_

#include <ctime>
#include <vector>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <string.h>
#include <fstream>

/**/
#include "bts_debug.h"
#include "handle_msg.h"
#include "bts_serialport.h"
#include "bts_get_message.h"
#include "bts_frame_message.h"
#include "file.h"

#define RETRUN_ERROR -1

typedef union
{
    float data_float;
    uint8_t bytes[4];
} data_format_float_bytes;

typedef union
{
    uint32_t data_int;
    uint8_t bytes[4];
} data_format_int_bytes;

typedef union
{
    uint16_t data_uint16;
    uint8_t bytes[2];
} data_format_uint8_16_t;

typedef struct
{
    uint8_t flag_send;
    uint32_t count_timeout;
    uint8_t flag_send_timeout;
} timeoutSendRepeat_t;

typedef struct
{
    uint8_t array[200];
    uint16_t sizeof_message;
    uint8_t status_update_file;
} valueSystem_t;

typedef struct
{
    uint32_t offset;
    uint32_t size_file_bin;
    uint8_t state_update_file;
    uint8_t mode_update_file;
} updateFile_t;

#endif /* MAIN */
