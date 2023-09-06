#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "Delay.h"
#include "debug.h"
#include "bts_frame_message.h"
#include "bts_get_message.h"



GPIO_InitTypeDef  GPIO_InitStructure;

void Fn_GPIO_Init (void);
uint32_t CountBlynk = 0;
uint8_t i = 0;
uint16_t lenght = 0;

int main (void){
	SystemCoreClockUpdate();
	Fn_DELAY_Init(72);
	Fn_GPIO_Init();
	db_DEBUG_Init(9600);
	while(1)
	{
		if(db_DEBUG_flag()==1)
		{
			db_DEBUG_Putchar("\n<-->\n");
			db_DEBUG_Putchar_RRX();
		}
		if(CountBlynk >= 1000)
		{
			db_DEBUG_Putchar("\n1\n");
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, i);
			i =! i;
			CountBlynk = 0;
		}
		CountBlynk++;
//		if(db_DEBUG_compare("bootloader") == 1)
//		{
//			db_DEBUG_Putchar("\nReset now\n");
//			Fn_DELAY_ms(100);
//			NVIC_SystemReset();
//		}
		
//		if (Is_Message(&lenght) != 0)
//    {
//      if (lenght > 0)
//      {
//				messageFrameMsg_t dataout;
//				printf("\n-(Size    : %d)-\n", DetectMessage(Array2_Receive, &dataout));
//				if(dataout.TypeMessage == TYPE_MSG_MODE_BOTLOADER)
//				{
//					db_DEBUG_Putchar("\nReset now\n");
//					Fn_DELAY_ms(100);
//					NVIC_SystemReset();
//				}
//      }
//    }
		Fn_DELAY_ms(1);
	}
}


void Fn_GPIO_Init (void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


