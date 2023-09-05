#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "Delay.h"
#include "debug.h"
GPIO_InitTypeDef  GPIO_InitStructure;
void Fn_GPIO_Init (void);
uint32_t CountBlynk = 0;
uint8_t i = 0;

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
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, i);
			i =! i;
			CountBlynk = 0;
		}
		CountBlynk++;
//		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0))
//		{
//			NVIC_SystemReset();
//		}
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


