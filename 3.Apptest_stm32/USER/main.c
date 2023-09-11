#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "Delay.h"
#include "debug.h"
#include "bts_frame_message.h"
#include "bts_get_message.h"

#include "lib_sd.h"
#include "flash_if.h"
#include "i2c.h"
#include "array_test.h"


GPIO_InitTypeDef  GPIO_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_init;

void ADC_Config(void);
void config_Timer (void);
void Fn_GPIO_Init (void);
volatile long long giay =0;
uint32_t CountBlynk = 0;
uint8_t i = 0;
uint16_t lenght = 0;
uint32_t addrees_save = 0x0801fc00;

int main (void){
	uint32_t temp_data = 0xAABBCCDD;
	SystemCoreClockUpdate();
	Fn_DELAY_Init(72);
	Fn_GPIO_Init();
	MSD0_SPI_Configuration();
	SD_Mount();
//	config_Timer();
//	i2c_init();
//	ADC_Config();
	db_DEBUG_Init(9600);
	SD_TotalSize("ptit.bin");	
	SD_WriteFile("array_test.txt", gImage_A, 15000, 0);
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
		
		if (Is_Message(&lenght) != 0)
    {
      if (lenght > 0)
      {
				messageFrameMsg_t dataout;
				printf("\n-(Size    : %d)-\n", DetectMessage(Array2_Receive, &dataout));
				if(dataout.TypeMessage == TYPE_MSG_MODE_BOTLOADER)
				{
					db_DEBUG_Putchar("\nReset now\n");
					Fn_DELAY_ms(100);
					NVIC_SystemReset();
				}
      }
    }
		Fn_DELAY_ms(1);
	}
}

void ADC_Config(void)
{
	ADC_InitTypeDef ADC_init;
	RCC_ADCCLKConfig (RCC_PCLK2_Div8);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	ADC_init.ADC_Mode = ADC_Mode_Independent;
	ADC_init.ADC_ScanConvMode = DISABLE;
	ADC_init.ADC_ContinuousConvMode = ENABLE;
	ADC_init.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_init.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_init.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1,&ADC_init);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8,1,ADC_SampleTime_55Cycles5);
	ADC_Cmd(ADC1, ENABLE);
	ADC_TempSensorVrefintCmd(ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
}

void config_Timer(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_init.TIM_ClockDivision = 0;
	TIM_init.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_init.TIM_Period = 65535;//Tru di 1
	TIM_init.TIM_Prescaler = 71;//72/72000000 = 0.000001s = 1us
	TIM_init.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_init);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
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
