#include "stm32f10x.h"
#include "TIM.h"
#include "delay.h"
/*Ӳ����*/
#include "GPIO.h"
#include "oled.h"
// #include "bmp.h"
#include "UART.h"
//#include "DHT11.h"
#include "SHT30.h"
#include "sys.h"
#include "rtc.h"
//����Э���

//ͼ�ν���
#include "oled_gui.h"

//Ϳѻ�����豸
#include "wifi.h"

//C��
#include <string.h>

// extern int BatElect;
extern u8 GuiNum;
// extern u8 OledRef;
// extern u8 LedBit;
extern u8 KEY1_st;

// float temperature;  	    
// float humidity;
u8 t = 0;
u8 LED_time = 0;

// vu8 key = 0;
// u16 WifiState = 0;


int main()
{	

	/*LED��������ʼ��*/
	LED_GPIO_INIT();
//	KEY_GPIO_INIT();
	
	/*�ж�����*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	EXTIX_Init();
	
	/*OLED��ʼ��*/
	OLED_Init();
	delay_ms(1000);
	// delay_ms(1000);
	StartUp();
	delay_ms(1000);

	//DHT11_Init();

	/*SHT30��ʼ��*/
	SHT_Init();
	delay_ms(500);
	
	/*���ڳ�ʼ��*/
	Usart1_Init(115200);			//debug����
	Usart2_Init(9600);				//Ϳѻģ��

	/*RTC��ʼ��*/
	RTC_Init();
	// mcu_get_system_time();
	
	/*wifiЭ���ʼ��*/
	wifi_protocol_init();
	delay_ms(1000);

	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	
	for(LED_time = 0; LED_time < 6; LED_time++)
	{
		PBout(11) = (1- GPIO_ReadOutputDataBit(GPIOB , GPIO_Pin_11));		
		delay_ms(250);
	}
		
	ALL_LED_OFF;		
	OLED_Clear();

	while(1)
	{
		
		switch (GuiNum)
		{
			case GuiTempHum:
				OledGui_Temp_Hum();		//��ʪ�Ƚ���
				break;
			case GuiTime:
				OledGui_Time();			//ʱ�ӽ���
				break;
			case GuiWeather:
				OledGui_Weather();		//��������
				break;
		}		
	}

}


void USART1_IRQHandler(void)
{
	uint8_t d=0;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //�����ж�
	{
		//��ȡ���յ�������
		d = USART_ReceiveData(USART1);
		
		//���ص�PC
		USART_SendData(USART1,d);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
		
		if(d == '1')GPIO_ResetBits(GPIOB,GPIO_Pin_11);
		if(d == 'A')GPIO_SetBits(GPIOB,GPIO_Pin_11);
		
		
		//��ձ�־λ	
		//USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}

}

void USART2_IRQHandler(void)
{
	unsigned char Res = 0;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //�����ж�
	{
		//��ȡ���յ�������
		Res = USART_ReceiveData(USART2);
		
		uart_receive_input(Res);
		
		//��ձ�־λ	
		//USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}

}

void EXTI4_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY1==0)	 //����KEY1
	{
		LED2_ON;
		UsartPrintf(USART_DEBUG, "Doing Smart Config\r\n");
		GuiNum = GuiTempHum;
		mcu_set_wifi_mode(SMART_CONFIG);//��������
		delay_ms(1000);
		LED2_OFF;
	}		 
	EXTI_ClearITPendingBit(EXTI_Line4);  //���LINE4�ϵ��жϱ�־λ  
}


void EXTI9_5_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY2==0)	 //����KEY0
	{
		LED2_ON;
		UsartPrintf(USART_DEBUG, "Function Select\r\n");
		KEY1_st = 1;
		delay_ms(500);
		LED2_OFF;
	}		 
	EXTI_ClearITPendingBit(EXTI_Line5);  //���LINE4�ϵ��жϱ�־λ  
}

