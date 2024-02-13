#include <stm32f10x.h>
#include "OLED.h"
#include "DHT11.h"
#include "Serial.h"
#include "Delay.h"
#include "Timer.h"

DHT11_Data_TypeDef DHT11_Data;

int main(void)
{ 
	OLED_Init();
	Serial_Init();
	DHT11_Init();
	Timer_Init();
	
	OLED_ShowString(1,1,"Temperature:");	
	OLED_ShowString(3,1,"Humidity:");
	
	while(1)
	{	
		if(DHT11_ReceiveData(&DHT11_Data) == SUCCESS){
			OLED_ShowNum(2, 1, DHT11_Data.Temp_int, 2);
			OLED_ShowString(2, 3, ".");
			OLED_ShowNum(2, 4, DHT11_Data.Temp_deci, 1);
			OLED_ShowChar(2, 5, '`');
			OLED_ShowChar(2, 6, 'C');
			
			OLED_ShowNum(4, 1, DHT11_Data.Humi_int, 2);
			OLED_ShowString(4, 3, ".");
			OLED_ShowNum(4, 4, DHT11_Data.Humi_deci, 2);
			OLED_ShowChar(4, 5, '\%');		
		}
	}
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET){
			Serial_Printf("Temperatuer:%d.%d`C\n", DHT11_Data.Temp_int, DHT11_Data.Temp_deci);
			Serial_Printf("Humidity:%d.%d%%\n", DHT11_Data.Humi_int, DHT11_Data.Humi_deci);
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
