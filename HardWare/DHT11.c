#include "DHT11.h"

//引脚初始化
void DHT11_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
  	GPIO_InitStructure.GPIO_Pin = DHT11_Pin;	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOA, &GPIO_InitStructure);		  

	GPIO_SetBits(GPIOA, DHT11_Pin);	 
}
//设置单总线输入模式
static void DHT11_Mode_IPU(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = DHT11_Pin;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 
	  GPIO_Init(GPIOA, &GPIO_InitStructure);	 
}
//设置单总线输出模式
static void DHT11_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Pin = DHT11_Pin;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);	 	 
}

void DHT11_Start(void)
{
	DHT11_Mode_Out_PP();		//输出模式
	DHT11_DATA_OUT(1);			//默认拉高
	DHT11_DATA_OUT(0);			//主机拉低 至少18ms
	Delay_ms(20);
	DHT11_DATA_OUT(1);			//主机拉高 20~40us
	Delay_us(30);
	DHT11_Mode_IPU();			//等待从机响应
//	while(DHT11_DATA_IN() == 0);
//	while(DHT11_DATA_IN() == 1);
	Delay_us(170);
}
//开始传输数据
uint8_t DHT11_ReceiveByte(void)
{
	uint8_t Data = 0x00, i ,Temp;
	
	for(i = 0; i < 8; i ++){
		while(DHT11_DATA_IN() == 0);	//等待从机拉低时间
		Delay_us(40);				//50us后,若总线为低,则Bit为0 反之为1
		Temp = 0x00;
		if(DHT11_DATA_IN() == 1){
			Temp = 0x01;
		}
		while(DHT11_DATA_IN() == 1);
		Data <<= 1;				//将Temp逐位移至Data中
		Data |= Temp;
	}
	return Data;
}

uint8_t DHT11_ReceiveData(DHT11_Data_TypeDef *DHT11_Data)
{
	DHT11_Start();					//起始条件
	
	if(DHT11_DATA_IN() == 0){
		DHT11_Data -> Humi_int = DHT11_ReceiveByte();
		DHT11_Data -> Humi_deci = DHT11_ReceiveByte();
		DHT11_Data -> Temp_int  = DHT11_ReceiveByte();
		DHT11_Data -> Temp_deci = DHT11_ReceiveByte();
		DHT11_Data -> Check_sum = DHT11_ReceiveByte();
		Delay_us(50);
		DHT11_Mode_Out_PP();
		DHT11_DATA_OUT(1);			//结束条件
		if(DHT11_Data -> Check_sum == DHT11_Data -> Humi_int + DHT11_Data -> Humi_deci + DHT11_Data -> Temp_int + DHT11_Data -> Temp_deci){
			return SUCCESS;		//验证成功标志
		}
		else {
			return ERROR;
		}
	}
	else	{
		return ERROR;
	}
}


