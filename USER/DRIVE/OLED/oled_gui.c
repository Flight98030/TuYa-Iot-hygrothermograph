#include "oled_gui.h"
#include "delay.h"
#include "wifi.h"
#include "mcu_api.h"
/*硬件库*/
#include "GPIO.h"
#include "oled.h"
#include "bmp.h"
#include "UART.h"
#include "rtc.h"
//#include "DHT11.h"
#include "SHT30.h"
#include "sys.h"

extern u8 KEY1_st;
extern u8 LED_time;

float temperature;  	    
float humidity;

u16 temp;  	    
u16 humi;

u8 GuiNum = GuiTempHum;
u8 tt = 0;

//页面切换
static void GuiSwitch(void)
{
	//检测按键是否按下
	if(KEY1_st && (mcu_get_wifi_work_state())==WIFI_CONN_CLOUD)
	{
		KEY1_st = 0;
		GuiNum++;
	}
	if(GuiNum > 2)
	{
		GuiNum = 1;
	}

}

/*WiFi工作状态*/
void WiFi_work_state_show(void)
{
	switch(mcu_get_wifi_work_state())
	{
		case SMART_CONFIG_STATE:
			//Smart 配置状态LED 快闪，led 闪烁请用户完成
			UsartPrintf(USART_DEBUG, "State: Smart Config\r\n");
			if(GuiNum == GuiTempHum)
			{
				OLED_ShowString(0,6,"                ",16);
				OLED_ShowString(15,7,"Smart Config",12);
			}
			for(LED_time = 0; LED_time < 10; LED_time++)
				{
					PBout(11) = (1- GPIO_ReadOutputDataBit(GPIOB , GPIO_Pin_11));
					delay_ms(100);
				}
		break;
	
		case AP_STATE:
			//AP 配置状态LED 慢闪，led 闪烁请用户完成
			//不会触发AP模式
		break;
	
		case WIFI_NOT_CONNECTED:
			//WIFI 配置完成，正在连接路由器，LED 常暗
			UsartPrintf(USART_DEBUG, "Connecting Route\r\n");
			OLED_ShowString(0,6,"                 ",16);
			OLED_ShowString(15,7,"Connecting...",12);
			OLED_ShowString(0,6,"                 ",16);
			LED1_OFF;
			delay_ms(400);
		break;

		case WIFI_CONNECTED:
			//路由器连接成功LED 常亮
			UsartPrintf(USART_DEBUG, "Connecting Route OK!\r\n");
			OLED_ShowString(0,6,"                 ",16);
			OLED_ShowString(35,7,"Online",12);
			OLED_ShowString(0,6,"                 ",16);
			LED1_ON;
		break;

		case WIFI_CONN_CLOUD:
			LED1_ON;
			//wifi 已连上云端LED 常亮
			UsartPrintf(USART_DEBUG, "Connecting Cloud\r\n");
			if(GuiNum == GuiTempHum)
			{
				// OLED_ShowString(0,7,"                   ",12);
				// OLED_ShowString(35,7,"Online",12);
			}
		default:break;
	}

}



//开机
void StartUp(void)
{
	OLED_Clear();
	OLED_ShowCHinese(32,3,8);//正
	OLED_ShowCHinese(48,3,9);//在
	OLED_ShowCHinese(64,3,10);//开
	OLED_ShowCHinese(80,3,11);//机
	OLED_ShowString(48,5,".",16);
	delay_ms(500);
	OLED_ShowString(64,5,".",16);
	delay_ms(500);
	OLED_ShowString(79,5,".",16);
	delay_ms(500);


}

//所有显示界面都要完成的功能
//time :LED闪烁的时间，数值越大闪烁越慢
static void All_Functin(u16 time)
{
	static u16 t = 0;
	static u8 min = 0;
	t++;
	/*wifi串口数据处理服务*/
	wifi_uart_service();

	if(t%10==0)
	{
			
		WiFi_work_state_show();
		// mcu_get_green_time();
		
		/*读取温湿度*/
		sht30_read_temp_humi(&temperature,&humidity);

		humi = (u16)humidity;
		temp = (u16)temperature;
				
		UsartPrintf(USART_DEBUG,"Tem:%d\r\n",temp);
		UsartPrintf(USART_DEBUG,"Hum:%d\r\n",humi);

		UsartPrintf(USART_DEBUG,"Day: %d/%d/%d\r\n",calendar.w_year,calendar.w_month,calendar.w_date);
		UsartPrintf(USART_DEBUG,"Time: %d:%d:%d\r\n",calendar.hour,calendar.min,calendar.sec);
	
		/*上报温湿度信息*/						
		mcu_dp_value_update(DPID_TEMP_CURRENT,(temperature*10)); //VALUE型数据上报;
		mcu_dp_value_update(DPID_HUMIDITY_VALUE,humidity); //VALUE型数据上报;
		// t = 0;
	}
	delay_ms(200);//延时
	
	
	if (min != calendar.min)
	{
		min = calendar.min;
		mcu_get_system_time();		//每分钟更新一次时间
	}
	
	
}



//温湿度界面
void OledGui_Temp_Hum(void)
{
	/*显示温湿度*/
	OLED_Clear();	
	OLED_ShowCHinese(20,0,0);//温
	OLED_ShowCHinese(38,0,1);//湿
	OLED_ShowCHinese(56,0,2);//度
	OLED_ShowCHinese(74,0,3);//检
	OLED_ShowCHinese(92,0,4);//测

	OLED_ShowCHinese(0,2,0);//温
	OLED_ShowCHinese(18,2,2);//度
	OLED_ShowString(36,2,":",16);
	OLED_ShowCHinese(90,2,5);//℃

	OLED_ShowCHinese(0,4,1);//湿
	OLED_ShowCHinese(18,4,2);//度
	OLED_ShowString(36,4,":",16); 
	OLED_ShowString(92,4,"%",16);

	sht30_read_temp_humi(&temperature,&humidity);

	while(1)
	{
		// AllForGui(1000);
		All_Functin(1000);
		GuiSwitch();

		if(GuiNum != GuiTempHum)
			break;		

		/* 显示温湿度 */
		humi = (u16)humidity;
		temp = (u16)temperature;
	
		OLED_ShowNum(46,2,temperature,2,16);	//显示温度
		OLED_ShowString(64,2,".",16);
		OLED_ShowNumber0(70,2,((temperature-temp)*100),2,16);	//显示小数   

		OLED_ShowNum(46,4,humidity,2,16);		//显示湿度	
		OLED_ShowString(64,4,".",16);
		OLED_ShowNumber0(70,4,((humidity-humi)*100),2,16);	//显示小数  

		if(mcu_get_wifi_work_state()==WIFI_CONN_CLOUD)
		{
			// OLED_ShowString(0,6,"                   ",16);
			OLED_ShowNum(22, 6, calendar.hour, 2, 16);
			OLED_ShowString(38,6,":",16);
			OLED_ShowNumber0(48, 6, calendar.min, 2, 16);
			OLED_ShowString(66,6,":",16);
			OLED_ShowNumber0(75, 6, calendar.sec, 2, 16);
		}

		delay_ms(100);
				
	}
	
}

//时间显示界面
void OledGui_Time(void)
{
	OLED_Clear();
	// OLED_ShowCHinese(0,0,21);//当
	// OLED_ShowCHinese(16,0,22);//前
	// OLED_ShowCHinese(32,0,6);//时
	// OLED_ShowCHinese(48,0,7);//间
	OLED_ShowCHinese(32,0,21);//当
	OLED_ShowCHinese(48,0,22);//前
	OLED_ShowCHinese(64,0,6);//时
	OLED_ShowCHinese(80,0,7);//间

	OLED_ShowCHinese(80,6,19);//星
	OLED_ShowCHinese(96,6,20);//期

	// OLED_ShowString(72,0,"(UTC+8)",16);

	OLED_ShowString(0,6,"    /  /",16);

	OLED_ShowString(30,2,":",24);
	OLED_ShowString(78,2,":",24);
	mcu_get_system_time();		//每分钟更新一次时间

	while(1){
		// AllForGui(600);
		All_Functin(600);
		GuiSwitch();
		if(GuiNum != GuiTime)
			break;	

		OLED_ShowNum(0, 6, calendar.w_year, 4, 16);
		OLED_ShowNum(40, 6, calendar.w_month, 2, 16);
		OLED_ShowNum(64, 6, calendar.w_date, 2, 16);
		
		OLED_ShowNum(2, 2, calendar.hour, 2, 24);
		OLED_ShowNumber0(48, 2, calendar.min, 2, 24);
		OLED_ShowNumber0(96, 2, calendar.sec, 2, 24);

		/* 显示星期几 */
		switch(RTC_Get_Week(calendar.w_year, \
				calendar.w_month, calendar.w_date))
		{
			case 0:
				OLED_ShowCHinese(112,6,21);//日
				break;
			case 1:
				OLED_ShowCHinese(112,6,12);//一
				break;
			case 2:
				OLED_ShowCHinese(112,6,13);//二
				break;
			case 3:
				OLED_ShowCHinese(112,6,14);//三
				break;
			case 4:
				OLED_ShowCHinese(112,6,15);//四
				break;
			case 5:
				OLED_ShowCHinese(112,6,16);//五
				break;
			case 6:
				OLED_ShowCHinese(112,6,17);//六
				break;
			
		}
	}	
}

int Wea_Temp = 0;				//温度
int Wea_Humidity = 0;			//湿度
char Wea_WindSpeed[8] = {0};	//风速
char Wea_Condition[8] = {0};	//天气
u8 *Wea_bmp = sunny;

//cloudy,thunder,sunny,snows,rain
//天气显示界面
void OledGui_Weather(void)
{
	OLED_Clear();
	// OLED_ShowString(64, 2, (u8 *)"Temp:", 16, OLED_CHR_NORMAL);
	// OLED_ShowString(64, 4, (u8 *)"Humi:", 16, OLED_CHR_NORMAL);
	// OLED_ShowString(64, 6, (u8 *)"Wine:", 16, OLED_CHR_NORMAL);
	OLED_ShowString(65,2,"Temp:",16);
	OLED_ShowString(65,4,"Humi:",16);
	OLED_ShowString(65,6,"Wine:",16);
	
	while(1){
		
		All_Functin(800);
		GuiSwitch();
		if(GuiNum != GuiWeather)
			break;	
		// OLED_ShowString(64, 0, (u8 *)Wea_Condition, 16, OLED_CHR_NORMAL);
		// OLED_DrawBMP(0, 0, 64, 64, Wea_bmp);
		// OLED_ShowNum(104, 2, Wea_Temp, 2, 16, OLED_CHR_NORMAL);	 			//温度
		// OLED_ShowNumber(112, 4, Wea_Humidity,  2, 16, OLED_CHR_NORMAL);	 	//湿度
		// OLED_ShowString(104, 6, (u8 *)Wea_WindSpeed, 16, OLED_CHR_NORMAL);		//风速
		
		// OLED_ShowString(64, 0, Wea_Condition, 16);
		// OLED_DrawBMP(0, 0, 64, 64, Wea_bmp);	
		// OLED_ShowNum(104, 2, Wea_Temp, 2, 16);			//温度
		// OLED_ShowNum(112, 4, Wea_Humidity, 2, 16);		//湿度
		// OLED_ShowString(104, 6, Wea_WindSpeed, 16);		//风速
		
	}
		
}