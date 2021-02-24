#include "GPIO.h"
#include "delay.h"
/*LED��ʼ��*/
void LED_GPIO_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_AHB2Periph_GPIOX,ENABLE);   //����GPIO��ʱ��
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;          //ѡģʽ�������
	GPIO_InitStructure.GPIO_Pin=GPIO_PINX;                  //�ĸ�����
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;         //���ŷ�Ӧ�ٶ�
	GPIO_Init(GPIOX, &GPIO_InitStructure);                  //��ɳ�ʼ�� 
  
//	GPIO_ResetBits(GPIOX, GPIO_PINX);   //�õ͵�ƽ 
}

/*OLED-ģ��iic��ʼ��*/
void OLED_GPIO_INIT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 		//ʹ��B�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_4;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				//�ٶ�50MHz
	
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  						//��ʼ��GPIOB4\5
	GPIO_SetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_5);	
}

/*SHT30-ģ��iic��ʼ��*/
void SHT30_GPIO_INIT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 		//ʹ��B�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				//�ٶ�50MHz
	
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  						//��ʼ��GPIOB6\7
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);	
}

/*����IO��ʼ��*/
void KEY_GPIO_INIT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 	//ʹ��A�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 			//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				//�ٶ�50MHz
	
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	 

}

/*DHT11-IO��ʼ��*/
void DHT11_GPIO_INIT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  							
	
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);	
}
/*����ɨ��*/
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY1==1||KEY2==1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY1==0)return KEY1_PRES;
		else if(KEY2==0)return KEY2_PRES;
	}else if(KEY1==0&&KEY2==0)key_up=1; 	    
 	return 0;// �ް�������
}



void EXTIX_Init(void)
{
	
	KEY_GPIO_INIT();
	
   	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��



   //GPIOA.4	  �ж����Լ��жϳ�ʼ������ �����ش��� PA0  WK_UP
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4); 
  	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	//GPIOA.5	  �ж����Լ��жϳ�ʼ������ �����ش��� PA0  WK_UP
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5); 
  	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 

  	NVIC_InitStructure.NVIC_IRQChannel =  EXTI9_5_IRQn;			//ʹ�ܰ���KEY1���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

 
}

