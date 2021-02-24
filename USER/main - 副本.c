#include "stm32f10x.h"
#include "TIM.h"
#include "GPIO.h"
#include "delay.h"
#include "oled.h"
#include "bmp.h"
#include "UART.h"
#include "DHT11.h"
#include "sys.h"
//网络协议层
#include "onenet.h"

//网络设备
#include "esp8266.h"

//C库
#include <string.h>


u8 DHT11_CHECH[2] = {66,88};
u8 t1;
u8 t2;
const char *topics[] = {"/mysmarthome/sub"};
unsigned short timeCount = 0;	//发送间隔变量
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
	OLED_ShowCHinese(0,0,0);//温
	OLED_ShowCHinese(18,0,1);//湿
	OLED_ShowCHinese(36,0,2);//度
	OLED_ShowCHinese(54,0,3);//检
	OLED_ShowCHinese(72,0,4);//测
	
	OLED_ShowCHinese(0,3,0);//温
	OLED_ShowCHinese(18,3,2);//度
	OLED_ShowString(36,3,":",16);
	OLED_ShowCHinese(70,3,5);//℃
	
	OLED_ShowCHinese(0,5,1);//湿
	OLED_ShowCHinese(18,5,2);//度
 	OLED_ShowString(36,5,":    %",16); 




	
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	ESP8266_Init();					//初始化ESP8266
	
	while(OneNet_DevLink())			//接入OneNET
		delay_ms(500);
	
	LED2_ON;				//鸣叫提示接入成功
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
					case KEY2_PRES:	//控制LED0翻转
						LED2_ON;
						delay_us(1000000);
						LED2_OFF;
						break;
					case KEY1_PRES:	//控制LED1翻转	 
						LED1_ON;
						delay_us(1000000);
						LED1_OFF;
						break;
				}
			}else delay_ms(10);
			if(t%10==0)			//每100ms读取一次
		{									  
			DHT11_Read_Data(&temperature,&humidity);	//读取温湿度值					    
			OLED_ShowNum(40,3,temperature,3,16);	//显示温度   		   
			OLED_ShowNum(40,5,humidity,3,16);		//显示湿度	
//			OLED_Refresh_Gram();		//更新显示到OLED 	   
		}				   
	 	delay_ms(10);
		t++;
		
		}
		
		if(++timeCount >= 500)									//发送间隔5s
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
