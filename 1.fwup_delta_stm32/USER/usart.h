#ifndef _UART_
#define _UART_
	
#ifdef __cplusplus
extern "C"{
#endif
	#include "stm32f10x.h" 
	extern uint8_t Flag2_Receive;
	extern uint8_t Array2_Receive[];
	extern uint8_t Count2_Data;
	
	void db_DEBUG_Init(unsigned int BaudRates);
	void db_DEBUG_Putchar(char *_varString);
	void db_DEBUG_SendChar(char _varChar);
	void db_DEBUG_Number(int _varNumber);
	int db_DEBUG_compare(char *string);
	int db_DEBUG_flag(void);
	void db_DEBUG_Putchar_RRX(void);
	
	void INIT_UART_2(unsigned int BaudRates);
	void UART2_SendChar(char data);
	void UART2_SendString(char *data);
#ifdef __cplusplus
}
#endif

#endif
