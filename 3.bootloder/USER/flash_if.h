#ifndef __FALSH_IF_H
#define __FALSH_IF_H
#include <stdint.h>
#include "stm32f10x.h"

//��ǰAPP�ļ���СΪ��0xFC000
#define STM32_FLASH_BASE            ((uint32_t)0x08000000)//STM32 FLASH����ʼ��ַ
#define APPLICATION_START_ADDRESS   ((uint32_t)0x08002000)//APP�ļ�����ʼ��ַ
#define APPLICATION_END_ADDRESS     ((uint32_t)0x0801FFFF)//APP�ļ��Ľ�����ַ
#define APPLICATION_SIZE   (APPLICATION_END_ADDRESS - APPLICATION_START_ADDRESS + 1)//APP�ļ��Ĵ�С

#define FLASH_APP_ADDR		0x08006000

#define STM32RBT6_SECTOR_SIZE   1024
#define PAGE_SIZE   STM32RBT6_SECTOR_SIZE
extern void FLASH_If_Init(void);
extern void FLASH_If_Finish(void);
extern uint32_t FLASH_If_Erase(uint32_t StartSector);
extern uint32_t FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data ,uint32_t DataLength);
extern int  writeSector(uint32_t Address,void * values, uint16_t size);

#endif
