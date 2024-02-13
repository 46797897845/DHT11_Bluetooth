#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define	DHT11_Pin				GPIO_Pin_7

#define	DHT11_DATA_OUT(x)		GPIO_WriteBit(GPIOA, DHT11_Pin, (BitAction)(x))	
#define  	DHT11_DATA_IN()	  	GPIO_ReadInputDataBit(GPIOA, DHT11_Pin)

typedef struct
{
	uint8_t  Humi_int;		//ʪ�ȵ���������
	uint8_t  Humi_deci;	 	//ʪ�ȵ�С������
	uint8_t  Temp_int;	 	//�¶ȵ���������
	uint8_t  Temp_deci;	 	//�¶ȵ�С������
	uint8_t  Check_sum;	 	//У���
		                 
}DHT11_Data_TypeDef;

void DHT11_Init(void);
uint8_t DHT11_ReceiveData(DHT11_Data_TypeDef *DHT11_Data);
#endif
