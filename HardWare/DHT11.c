#include "DHT11.h"

//���ų�ʼ��
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
//���õ���������ģʽ
static void DHT11_Mode_IPU(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = DHT11_Pin;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 
	  GPIO_Init(GPIOA, &GPIO_InitStructure);	 
}
//���õ��������ģʽ
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
	DHT11_Mode_Out_PP();		//���ģʽ
	DHT11_DATA_OUT(1);			//Ĭ������
	DHT11_DATA_OUT(0);			//�������� ����18ms
	Delay_ms(20);
	DHT11_DATA_OUT(1);			//�������� 20~40us
	Delay_us(30);
	DHT11_Mode_IPU();			//�ȴ��ӻ���Ӧ
//	while(DHT11_DATA_IN() == 0);
//	while(DHT11_DATA_IN() == 1);
	Delay_us(170);
}
//��ʼ��������
uint8_t DHT11_ReceiveByte(void)
{
	uint8_t Data = 0x00, i ,Temp;
	
	for(i = 0; i < 8; i ++){
		while(DHT11_DATA_IN() == 0);	//�ȴ��ӻ�����ʱ��
		Delay_us(40);				//50us��,������Ϊ��,��BitΪ0 ��֮Ϊ1
		Temp = 0x00;
		if(DHT11_DATA_IN() == 1){
			Temp = 0x01;
		}
		while(DHT11_DATA_IN() == 1);
		Data <<= 1;				//��Temp��λ����Data��
		Data |= Temp;
	}
	return Data;
}

uint8_t DHT11_ReceiveData(DHT11_Data_TypeDef *DHT11_Data)
{
	DHT11_Start();					//��ʼ����
	
	if(DHT11_DATA_IN() == 0){
		DHT11_Data -> Humi_int = DHT11_ReceiveByte();
		DHT11_Data -> Humi_deci = DHT11_ReceiveByte();
		DHT11_Data -> Temp_int  = DHT11_ReceiveByte();
		DHT11_Data -> Temp_deci = DHT11_ReceiveByte();
		DHT11_Data -> Check_sum = DHT11_ReceiveByte();
		Delay_us(50);
		DHT11_Mode_Out_PP();
		DHT11_DATA_OUT(1);			//��������
		if(DHT11_Data -> Check_sum == DHT11_Data -> Humi_int + DHT11_Data -> Humi_deci + DHT11_Data -> Temp_int + DHT11_Data -> Temp_deci){
			return SUCCESS;		//��֤�ɹ���־
		}
		else {
			return ERROR;
		}
	}
	else	{
		return ERROR;
	}
}


