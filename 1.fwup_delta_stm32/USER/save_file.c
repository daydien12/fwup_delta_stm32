
#include "save_file.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "usart.h"
#include "lib_sd.h"
#include "delta.h"
#define FILE_FILE "blynk.bin"

ETX_OTA_VR_t 								ota_data;
static etx_OtaState_e       ota_state = ETX_OTA_STATE_START;
static etx_OtaFrameData_e   ota_frame = ETX_OTA_FRAME_START;

uint8_t  arr_temp[1024] = {0};
uint8_t  check_point = 0, check_point_uart = 0;
uint32_t data_size;
uint32_t size_file_bin, count_size_file = 0;

static void EXT_EndFrame(void);
static void EXT_ResetIndex(void);
static void EXT_InitOtaData(void);
static void EXT_ProcessFrame(void);

void ETX_ReceiveChunk(void)
{
	ota_data.Buffer[0] = 0;
  ota_state = ETX_OTA_STATE_START;
  EXT_InitOtaData();
}

void ETX_Run(void)
{
	EXT_ProcessFrame();
}

static void EXT_EndFrame(void)
{
	ota_frame = ETX_OTA_FRAME_START;
}

static void EXT_ResetIndex(void)
{
	ota_data.Index = 0;
  ota_data.Length = 0;
  ota_data.FlagFrame = 0;
  ota_frame = ETX_OTA_FRAME_START;
}

static void EXT_InitOtaData(void)
{
	ota_data.Index = 0;
  ota_data.Length = 0;
  ota_data.FlagFrame = 0;
  memset(ota_data.Buffer, 0, ETX_OTA_PACKET_MAX_SIZE);
}

static void EXT_ProcessFrame(void)
{
	if (ota_data.FlagFrame == 1)
  {
    ota_data.FlagFrame = 0;
    switch (ota_state )
    {
      case ETX_OTA_STATE_START:
        {
          ETX_OTA_COMMAND_t *cmd = (ETX_OTA_COMMAND_t*)ota_data.Buffer;
          if (cmd->packet_type == ETX_OTA_PACKET_TYPE_CMD )
          {
            if (cmd->cmd == ETX_OTA_CMD_START )
            {
							Delta_Mount();
              ota_state = ETX_OTA_STATE_HEADER;
							check_point = 1;
            }
          }
          else
          {
            size_file_bin = 0;
						count_size_file = 0;
          }
        }
        break;

      case ETX_OTA_STATE_HEADER:
        {
          u8Tou32_u TempData;
          ETX_OTA_HEADER_t *header = (ETX_OTA_HEADER_t*)ota_data.Buffer;
					
          if ( header->packet_type == ETX_OTA_PACKET_TYPE_HEADER )
          {
						check_point = 2;
            ota_state = ETX_OTA_STATE_DATA;
            TempData.Data_In[0] = ota_data.Buffer[4];
            TempData.Data_In[1] = ota_data.Buffer[5];
            TempData.Data_In[2] = ota_data.Buffer[6];
            TempData.Data_In[3] = ota_data.Buffer[7];
            size_file_bin = TempData.Data_Out;
          }
          else
          {
            size_file_bin = 0;
            ota_state = ETX_OTA_STATE_START;
          }
        }
        break;

      case ETX_OTA_STATE_DATA:
        {
          uint16_t i;
          u8Tou16_u datatemp;
					uint8_t *dd;
          if (ota_data.Buffer[1] == ETX_OTA_PACKET_TYPE_DATA)
          {
						check_point = 3;
            datatemp.Data_In[0] = ota_data.Buffer[2];
            datatemp.Data_In[1] = ota_data.Buffer[3];
            data_size = datatemp.Data_Out;
						
						for (i = 0; i < data_size; i++)
            {
              arr_temp[i] = ota_data.Buffer[i + 4];
            }
						dd = (uint8_t*)arr_temp;
						SD_WriteFile(FILE_FILE, dd, data_size, count_size_file);
						count_size_file += data_size;
            if (count_size_file >= size_file_bin)
            {
              ota_state = ETX_OTA_STATE_END;
            }
          }
        }
        break;

      case ETX_OTA_STATE_END:
        {
					check_point = 4;
          ETX_OTA_COMMAND_t *cmd = (ETX_OTA_COMMAND_t*)ota_data.Buffer;
          if ( cmd->packet_type == ETX_OTA_PACKET_TYPE_CMD )
          {
            if ( cmd->cmd == ETX_OTA_CMD_END )
            {
              ota_state = ETX_OTA_STATE_START;
							size_file_bin = 0;
							count_size_file = 0;
            }
          }
          else
          {
            ota_state = ETX_OTA_STATE_START;
						size_file_bin = 0;
						count_size_file = 0;
          }
        }
        break;
    }
  }
}

void USART1_IRQHandler()
{
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    uint8_t temp_char = USART_ReceiveData(USART1);
    switch (ota_frame)
    {
      case ETX_OTA_FRAME_START:
        if (temp_char != ETX_OTA_SOF)
        {
          EXT_InitOtaData();
        }
        else
        {
          EXT_InitOtaData();
          ota_frame = ETX_OTA_FRAME_TYPE;
          ota_data.Buffer[ota_data.Index] = temp_char;
          ota_data.Index++;
					check_point_uart = 1;
        }
        break;

      case ETX_OTA_FRAME_TYPE:
        ota_frame = ETX_OTA_FRAME_LENGTH;
        ota_data.Buffer[ota_data.Index] = temp_char;
        ota_data.Index++;
				check_point_uart = 2;
        break;

      case ETX_OTA_FRAME_LENGTH:
        if (ota_data.Index >= 4)
        {
          u8Tou16_u temp_data;
          ota_frame = ETX_OTA_FRAME_DATA;
          temp_data.Data_In[0] = ota_data.Buffer[2];
          temp_data.Data_In[1] = ota_data.Buffer[3];
          ota_data.Length = temp_data.Data_Out;
					check_point_uart = 3;
        }
        ota_data.Buffer[ota_data.Index] = temp_char;
        ota_data.Index++;
        break;

      case ETX_OTA_FRAME_DATA:
        ota_data.Buffer[ota_data.Index] = temp_char;
        ota_data.Index++;
        if ((ota_data.Index - 4) >= (ota_data.Length))
        {
          ota_frame = ETX_OTA_FRAME_CRC;
					check_point_uart = 4;
        }
        break;

      case ETX_OTA_FRAME_CRC:
        ota_data.Buffer[ota_data.Index] = temp_char;
        ota_data.Index++;
        if ((ota_data.Index - 4 - (ota_data.Length)) >= (4))
        {
          ota_frame = ETX_OTA_FRAME_END;
					check_point_uart = 5;
        }
        break;

      case ETX_OTA_FRAME_END:
        if (temp_char != ETX_OTA_EOF)
        {
          EXT_ResetIndex();
        }
        else
        {
          ota_data.FlagFrame = 1;
          ota_data.Buffer[ota_data.Index] = temp_char;
          ota_data.Index++;
          ota_data.Buffer[ota_data.Index] = 0x00;
          EXT_EndFrame();
					check_point_uart = 6;
        }
        break;
    }
  }
  USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}
