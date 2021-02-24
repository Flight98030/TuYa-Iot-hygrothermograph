#include "GPIO.h"
#include "delay.h"
/*LED初始化*/
void LED_GPIO_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_AHB2Periph_GPIOX,ENABLE);   //启动GPIO的时钟
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;          //选模式推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_PINX;                  //哪个引脚
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;         //引脚反应速度
	GPIO_Init(GPIOX, &GPIO_InitStructure);                  //完成初始化 
  
//	GPIO_ResetBits(GPIOX, GPIO_PINX);   //置低电平 
}

/*OLED-模拟iic初始化*/
void OLED_GPIO_INIT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 		//使能B端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_4;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				//速度50MHz
	
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  						//初始化GPIOB4\5
	GPIO_SetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_5);	
}

/*SHT30-模拟iic初始化*/
void SHT30_GPIO_INIT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 		//使能B端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				//速度50MHz
	
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  						//初始化GPIOB6\7
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);	
}

/*按键IO初始化*/
void KEY_GPIO_INIT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 	//使能A端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 			//下拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				//速度50MHz
	
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	 

}

/*DHT11-IO初始化*/
void DHT11_GPIO_INIT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  							
	
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);	
}
/*按键扫描*/
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY1==1||KEY2==1))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY1==0)return KEY1_PRES;
		else if(KEY2==0)return KEY2_PRES;
	}else if(KEY1==0&&KEY2==0)key_up=1; 	    
 	return 0;// 无按键按下
}



void EXTIX_Init(void)
{
	
	KEY_GPIO_INIT();
	
   	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟



   //GPIOA.4	  中断线以及中断初始化配置 上升沿触发 PA0  WK_UP
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4); 
  	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	//GPIOA.5	  中断线以及中断初始化配置 上升沿触发 PA0  WK_UP
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5); 
  	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键WK_UP所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 

  	NVIC_InitStructure.NVIC_IRQChannel =  EXTI9_5_IRQn;			//使能按键KEY1所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

 
}

