#include "stm32f10x.h"
#include "TIM.h"
#include "GPIO.h"
#include "delay.h"
#include "oled.h"
#include "bmp.h"
#include "UART.h"
#include "DHT11.h"
#include "sys.h"
//����Э���
#include "onenet.h"

//�����豸
#include "esp8266.h"

//C��
#include <string.h>


u8 DHT11_CHECH[2] = {66,88};
u8 t1;
u8 t2;
const char *topics[] = {"/mysmarthome/sub"};
unsigned short timeCount = 0;	//���ͼ������
unsigned char *dataPtr = NULL;


int main()
{	
	u8 t=0;			    
	u8 temperature;  	    
	u8 humidity;
	vu8 key=0;	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	LED_GPIO_INIT();
	OLED_Init();
	KEY_GPIO_INIT();
	DHT11_Init();
	delay_ms(500);
	
	
	OLED_Clear();
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




	
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	ESP8266_Init();					//��ʼ��ESP8266
	
	while(OneNet_DevLink())			//����OneNET
		delay_ms(500);
	
	LED2_ON;				//������ʾ����ɹ�
	delay_ms(250);
	LED2_OFF;
	
	OneNet_Subscribe(topics, 1);
	
	while(1)
	{

		ALL_LED_OFF;
		while(1)
		{	
			key=KEY_Scan(1);
			if(key)
			{						   
				switch(key)
				{				 
					case KEY2_PRES:	//����LED0��ת
						LED2_ON;
						delay_us(1000000);
						LED2_OFF;
						break;
					case KEY1_PRES:	//����LED1��ת	 
						LED1_ON;
						delay_us(1000000);
						LED1_OFF;
						break;
				}
			}else delay_ms(10);
			if(t%10==0)			//ÿ100ms��ȡһ��
		{									  
			DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ					    
			OLED_ShowNum(40,3,temperature,3,16);	//��ʾ�¶�   		   
			OLED_ShowNum(40,5,humidity,3,16);		//��ʾʪ��	
//			OLED_Refresh_Gram();		//������ʾ��OLED 	   
		}				   
	 	delay_ms(10);
		t++;
		
		}
		
		if(++timeCount >= 500)									//���ͼ��5s
		{
			UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
			
			OneNet_Publish("pcTopic", "MQTT Publish Test");
			
			timeCount = 0;
			ESP8266_Clear();
		}
		
		dataPtr = ESP8266_GetIPD(3);
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
		
		delay_ms(10);

	}
}
