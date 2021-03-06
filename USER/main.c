#include "stm32f10x.h"
#include "TIM.h"
#include "delay.h"
/*硬件库*/
#include "GPIO.h"
#include "oled.h"
// #include "bmp.h"
#include "UART.h"
//#include "DHT11.h"
#include "SHT30.h"
#include "sys.h"
#include "rtc.h"
//网络协议层

//图形界面
#include "oled_gui.h"

//涂鸦网络设备
#include "wifi.h"

//C库
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

	/*LED、按键初始化*/
	LED_GPIO_INIT();
//	KEY_GPIO_INIT();
	
	/*中断设置*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	EXTIX_Init();
	
	/*OLED初始化*/
	OLED_Init();
	delay_ms(1000);
	// delay_ms(1000);
	StartUp();
	delay_ms(1000);

	//DHT11_Init();

	/*SHT30初始化*/
	SHT_Init();
	delay_ms(500);
	
	/*串口初始化*/
	Usart1_Init(115200);			//debug串口
	Usart2_Init(9600);				//涂鸦模组

	/*RTC初始化*/
	RTC_Init();
	// mcu_get_system_time();
	
	/*wifi协议初始化*/
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
				OledGui_Temp_Hum();		//温湿度界面
				break;
			case GuiTime:
				OledGui_Time();			//时钟界面
				break;
			case GuiWeather:
				OledGui_Weather();		//天气界面
				break;
		}		
	}

}


void USART1_IRQHandler(void)
{
	uint8_t d=0;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收中断
	{
		//读取接收到的数据
		d = USART_ReceiveData(USART1);
		
		//返回到PC
		USART_SendData(USART1,d);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
		
		if(d == '1')GPIO_ResetBits(GPIOB,GPIO_Pin_11);
		if(d == 'A')GPIO_SetBits(GPIOB,GPIO_Pin_11);
		
		
		//清空标志位	
		//USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}

}

void USART2_IRQHandler(void)
{
	unsigned char Res = 0;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收中断
	{
		//读取接收到的数据
		Res = USART_ReceiveData(USART2);
		
		uart_receive_input(Res);
		
		//清空标志位	
		//USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}

}

void EXTI4_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY1==0)	 //按键KEY1
	{
		LED2_ON;
		UsartPrintf(USART_DEBUG, "Doing Smart Config\r\n");
		GuiNum = GuiTempHum;
		mcu_set_wifi_mode(SMART_CONFIG);//智能配网
		delay_ms(1000);
		LED2_OFF;
	}		 
	EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE4上的中断标志位  
}


void EXTI9_5_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY2==0)	 //按键KEY0
	{
		LED2_ON;
		UsartPrintf(USART_DEBUG, "Function Select\r\n");
		KEY1_st = 1;
		delay_ms(500);
		LED2_OFF;
	}		 
	EXTI_ClearITPendingBit(EXTI_Line5);  //清除LINE4上的中断标志位  
}

