#include "stm32f10x.h"
#include "TIM.h"
#include "delay.h"
/*Ӳ����*/
#include "GPIO.h"
#include "oled.h"
#include "bmp.h"
#include "UART.h"
//#include "DHT11.h"
#include "SHT30.h"
#include "sys.h"
//����Э���


//Ϳѻ�����豸
#include "wifi.h"


//C��
#include <string.h>


//u8 DHT11_CHECH[2] = {66,88};
//u8 t1;
//u8 t2;
u8 traget = 0;       //��ʪ�ȱ�־ֵ 

//const char *devSubTopic[] = {"/mysmarthome/sub"};
//const char devPubTopic[] = "/mysmarthome/pub";

char  PUB_BUF[256];	
//int flag = 1;


int main()
{	
	u8 t = 0;
	u8 LED_time = 0;
	
	float temperature;  	    
	float humidity;
	
	u16 temp;  	    
	u16 humi;
	
	vu8 key = 0;
	u16 WifiState = 0;
//	unsigned short timeCount = 0;									// ���ͼ������
//	unsigned char *dataPtr = NULL;
	
//	u8 buffer[5]; 	
	
	/*LED��������ʼ��*/
	LED_GPIO_INIT();
//	KEY_GPIO_INIT();
	
	/*�ж�����*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	EXTIX_Init();
	
	/*OLED��ʼ��*/
	OLED_Init();
	delay_ms(1000);
	delay_ms(1000);
	OLED_Clear();
	OLED_ShowString(0,0,"Waiting",16);
	OLED_ShowString(0,3,"Hardware",16);
	OLED_ShowString(64,3,"...",16);
	delay_ms(1000);
	delay_ms(1000);

	
	//DHT11_Init();
	
	/*SHT30��ʼ��*/
	SHT_Init();
	delay_ms(500);
	
	/*���ڳ�ʼ��*/
	Usart1_Init(115200);			//debug����
	Usart2_Init(9600);				//Ϳѻģ��
	

	delay_ms(1000);
	
	/*wifiЭ���ʼ��*/
	wifi_protocol_init();
	
    /*��ʾ*/	
	OLED_Clear();
	OLED_ShowString(0,0,"WiFi",16);
	OLED_ShowString(0,3,"Connecting...",16);
	
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
	
		/*wifi�������ݴ�����*/
		wifi_uart_service();
		
		/*��ȡWiFi״̬*/			
		WifiState = mcu_get_wifi_work_state();
	
			
			if(t%10==0 && WifiState>=WIFI_CONNECTED)			//ÿ100ms��ȡһ��
				{									  
//					DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ	
					sht30_read_temp_humi(&temperature,&humidity);
					
					/*��ʾ��ʪ��*/	
//					OLED_Clear();
					OLED_ShowCHinese(0,0,0);//��
					OLED_ShowCHinese(18,0,1);//ʪ
					OLED_ShowCHinese(36,0,2);//��
					OLED_ShowCHinese(54,0,3);//��
					OLED_ShowCHinese(72,0,4);//��
	
					OLED_ShowCHinese(0,3,0);//��
					OLED_ShowCHinese(18,3,2);//��
					OLED_ShowString(36,3,":",16);
					OLED_ShowCHinese(70,3,5);//��
	
					OLED_ShowCHinese(0,5,1);//ʪ
					OLED_ShowCHinese(18,5,2);//��
					OLED_ShowString(36,5,":    %",16); 
			
					humi = (u16)humidity;
					temp = (u16)temperature;
			
					UsartPrintf(USART_DEBUG,"Tem:%d\r\n",temp);
					UsartPrintf(USART_DEBUG,"Hum:%d\r\n",humi);
				
					OLED_ShowNum(40,3,temperature,3,16);	//��ʾ�¶�   		   
					OLED_ShowNum(40,5,humidity,3,16);		//��ʾʪ��	
			
					temperature = temperature * 10;
				
					mcu_dp_value_update(DPID_TEMP_CURRENT,temperature); //VALUE�������ϱ�;
					mcu_dp_value_update(DPID_HUMIDITY_VALUE,humidity); //VALUE�������ϱ�;
				
//					OLED_Refresh_Gram();		//������ʾ��OLED 	


				}

					/*WiFi����״̬*/
					switch(WifiState)
						{
							case SMART_CONFIG_STATE:
								//Smart ����״̬LED ������led ��˸���û����
								UsartPrintf(USART_DEBUG, "State: Smart Config\r\n");
								OLED_Clear();
								OLED_ShowString(0,0,"WiFi",16);
								OLED_ShowString(0,3,"Connecting...",16);

									for(LED_time = 0; LED_time < 6; LED_time++)
										{
											PBout(11) = (1- GPIO_ReadOutputDataBit(GPIOB , GPIO_Pin_11));
											delay_ms(100);
										}
							break;
						
							case AP_STATE:
								//AP ����״̬LED ������led ��˸���û����
								UsartPrintf(USART_DEBUG, "State: AP Config\r\n");
									for(LED_time = 0; LED_time < 6; LED_time++)
										{	
											PBout(11) = (1- GPIO_ReadOutputDataBit(GPIOB , GPIO_Pin_11));							
											delay_ms(600);
										}	
							break;
						
							case WIFI_NOT_CONNECTED:
								//WIFI ������ɣ���������·������LED ����
								UsartPrintf(USART_DEBUG, "Connecting Route\r\n");
								LED1_OFF;
							break;
				
							case WIFI_CONNECTED:
								//·�������ӳɹ�LED ����
								UsartPrintf(USART_DEBUG, "Connecting Route OK!\r\n");
								LED1_ON;
							break;
				
							case WIFI_CONN_CLOUD:
								LED1_ON;
							//wifi �������ƶ�LED ����
							UsartPrintf(USART_DEBUG, "Connecting Cloud\r\n");
							default:break;
							}

					
			
			delay_ms(100);
			t++;
		


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
		mcu_set_wifi_mode(SMART_CONFIG);//��������
		delay_ms(1000);
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
		UsartPrintf(USART_DEBUG, "Doing WiFi Test\r\n");
		//mcu_start_wifitest();
		mcu_reset_wifi();
		delay_ms(1000);
		delay_ms(1000);
		LED2_OFF;
	}		 
	EXTI_ClearITPendingBit(EXTI_Line5);  //���LINE4�ϵ��жϱ�־λ  
}
