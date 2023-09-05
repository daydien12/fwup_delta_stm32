#include "ota_uart_fram.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "debug.h"
#include "flash_if.h"

ETX_OTA_VR_t OTA_Data;
static ETX_OTA_FRAME_DATA_	ota_frame = ETX_OTA_FRAME_START;
static ETX_OTA_STATE_ 			ota_state = ETX_OTA_STATE_START;

uint8_t ArrTemp[128] = {0};
uint8_t checkpoint = 0;
uint32_t datasize;
uint32_t SizeFileBin;
uint32_t Count_SizeFileBin = FLASH_APP_ADDR;
uint32_t tempss = 0;

static void ext_init_otadata(void);
static void ext_reset_index(void);
static void ext_end_frame(void);
static void ext_process_frame(void);


void etx_run(void)
{
	ext_process_frame();
}

static void ext_process_frame(void)
{
	if(OTA_Data.FlagFrame == 1)
	{
		OTA_Data.FlagFrame = 0;
		switch( ota_state )
    {
      case ETX_OTA_STATE_START:
      {
        ETX_OTA_COMMAND_t *cmd = (ETX_OTA_COMMAND_t*)OTA_Data.Buffer;
        if( cmd->packet_type == ETX_OTA_PACKET_TYPE_CMD )
        {
          if( cmd->cmd == ETX_OTA_CMD_START )
          {
						FLASH_If_Erase(FLASH_APP_ADDR);
            ota_state = ETX_OTA_STATE_HEADER;
						checkpoint = 1;
          }
        }
				else
				{
					Count_SizeFileBin = FLASH_APP_ADDR;
					tempss = 0;
					SizeFileBin = 0;
				}
      }
      break;

      case ETX_OTA_STATE_HEADER:
			{
				u8Tou32_u TempData;
				ETX_OTA_HEADER_t *header = (ETX_OTA_HEADER_t*)OTA_Data.Buffer;
				if( header->packet_type == ETX_OTA_PACKET_TYPE_HEADER )
				{
						ota_state = ETX_OTA_STATE_DATA;
						TempData.Data_In[0] = OTA_Data.Buffer[4];
						TempData.Data_In[1] = OTA_Data.Buffer[5];
						TempData.Data_In[2] = OTA_Data.Buffer[6];
						TempData.Data_In[3] = OTA_Data.Buffer[7];
						SizeFileBin = TempData.Data_Out;
						checkpoint = 3;
				}
				else
				{
					SizeFileBin = 0;
					ota_state = ETX_OTA_STATE_START;
				}
			}
      break;

      case ETX_OTA_STATE_DATA:
			{
				uint8_t i;
				u8Tou16_u datatemp;
				uint32_t *dd;
				checkpoint = 3;
				if(OTA_Data.Buffer[1]== ETX_OTA_PACKET_TYPE_DATA)
				{
					datatemp.Data_In[0] = OTA_Data.Buffer[2];
					datatemp.Data_In[1] = OTA_Data.Buffer[3];
					datasize = datatemp.Data_Out;
					
					for(i=0; i<datasize; i++)
					{
						ArrTemp[i] = OTA_Data.Buffer[i+4];
					}
					dd = (uint32_t*)ArrTemp;
					FLASH_If_Write(&Count_SizeFileBin, (uint32_t*)dd, (uint32_t)datasize/4);	
					tempss = Count_SizeFileBin - FLASH_APP_ADDR;
					if(tempss >= SizeFileBin)
					{
						ota_state = ETX_OTA_STATE_END;
						checkpoint = 4;
					}
				}
			
			}
      break;

      case ETX_OTA_STATE_END:
			{
				ETX_OTA_COMMAND_t *cmd = (ETX_OTA_COMMAND_t*)OTA_Data.Buffer;
        if( cmd->packet_type == ETX_OTA_PACKET_TYPE_CMD )
        {
          if( cmd->cmd == ETX_OTA_CMD_END )
          {
						checkpoint = 5;
            ota_state = ETX_OTA_STATE_START;
						FLASH_If_Finish();
          }
        }
				else
				{
					checkpoint = 6;
          ota_state = ETX_OTA_STATE_START;
					FLASH_If_Finish();
				}
			}
      break;
		}
	}
}

void etx_receive_chunk(void)
{
	FLASH_If_Init();
	OTA_Data.Buffer[0] =0;
	ota_state = ETX_OTA_STATE_START;
	ext_init_otadata();
} 

static void ext_reset_index(void)
{
	OTA_Data.Index = 0;
	OTA_Data.Length = 0;
	OTA_Data.FlagFrame = 0;
	ota_frame= ETX_OTA_FRAME_START;
}

static void ext_end_frame(void)
{
	ota_frame= ETX_OTA_FRAME_START;
}

static void ext_init_otadata(void)
{
	OTA_Data.Index = 0;
	OTA_Data.Length = 0;
	OTA_Data.FlagFrame = 0;
	memset( OTA_Data.Buffer, 0, ETX_OTA_PACKET_MAX_SIZE );
}


void USART1_IRQHandler()
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
	{
		uint8_t temp_char = USART_ReceiveData(USART1);
		switch(ota_frame)
		{
			case ETX_OTA_FRAME_START:
				if(temp_char != ETX_OTA_SOF)
				{
					ext_init_otadata();
				}
				else
				{
					ext_init_otadata();
					ota_frame = ETX_OTA_FRAME_TYPE;
					OTA_Data.Buffer[OTA_Data.Index] = temp_char;
					OTA_Data.Index++;
				}
			break;
			
			case ETX_OTA_FRAME_TYPE:
				ota_frame = ETX_OTA_FRAME_LENGTH;
				OTA_Data.Buffer[OTA_Data.Index] = temp_char;
				OTA_Data.Index++;
			break;
					
			case ETX_OTA_FRAME_LENGTH:
				if(OTA_Data.Index >= 4)
				{
					u8Tou16_u Tempdata;
					ota_frame = ETX_OTA_FRAME_DATA;
					Tempdata.Data_In[0] = OTA_Data.Buffer[2];
					Tempdata.Data_In[1] = OTA_Data.Buffer[3];
					OTA_Data.Length = Tempdata.Data_Out;
				}
				OTA_Data.Buffer[OTA_Data.Index] = temp_char;
				OTA_Data.Index++;
			break;
							
			case ETX_OTA_FRAME_DATA:
				
				OTA_Data.Buffer[OTA_Data.Index] = temp_char;
				OTA_Data.Index++;
				if((OTA_Data.Index - 4) >= (OTA_Data.Length))
				{
					ota_frame = ETX_OTA_FRAME_CRC;
				}
			break;
									
			case ETX_OTA_FRAME_CRC:
				OTA_Data.Buffer[OTA_Data.Index] = temp_char;
				OTA_Data.Index++;
				if((OTA_Data.Index - 4 - (OTA_Data.Length)) >= (4))
				{
					ota_frame = ETX_OTA_FRAME_END;
				}
			break;
											
			case ETX_OTA_FRAME_END:
				if(temp_char != ETX_OTA_EOF)
				{
					ext_reset_index();
				}
				else
				{
					OTA_Data.FlagFrame = 1;
					OTA_Data.Buffer[OTA_Data.Index] = temp_char;
					OTA_Data.Index++;
					OTA_Data.Buffer[OTA_Data.Index] = 0x00;
					ext_end_frame();
				}
			break;
		}
	}
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}
