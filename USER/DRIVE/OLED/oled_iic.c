#include "oled_iic.h"
#include "delay.h"

static void SDA_OUT(void)
{
	GPIO_InitTypeDef   	gpio;    
	gpio.GPIO_Pin  	= 	GPIO_Pin_9;
	gpio.GPIO_Mode 	= 	GPIO_Mode_Out_OD;
	gpio.GPIO_Speed = 	GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &gpio);
}

static void SDA_IN(void)
{
    GPIO_InitTypeDef  gpio;    
		gpio.GPIO_Pin 	= GPIO_Pin_9;    
    gpio.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &gpio);
}

void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA_H;	  	  
	IIC_SCL_H;
	delay_us(4);
 	IIC_SDA_L;		 //START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_L;		 //ǯסI2C���ߣ�׼�����ͻ��������
}

void IIC_Stop(void)
{
	SDA_OUT();		//sda�����
	IIC_SCL_L;
	IIC_SDA_L;		//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL_H; 
	IIC_SDA_H;		//����I2C���߽����ź�
	delay_us(4);	
}

u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      		//SDA����Ϊ����  
	IIC_SDA_H;	delay_us(1);	   
	IIC_SCL_H;	delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_L;			//ʱ�����0 	   
	return 0;  
}

void Write_IIC_Byte(unsigned char IIC_Byte)
{
		u8 t;   
		SDA_OUT(); 	    
    IIC_SCL_L;			//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
			if(IIC_Byte & 0x80)	IIC_SDA_H;
      else 								IIC_SDA_L;
			IIC_Byte<<=1; 	  
			delay_us(2);   //��TEA5767��������ʱ���Ǳ����
			IIC_SCL_H;
			delay_us(2); 
			IIC_SCL_L;	
			delay_us(2);
    }	 
}

void Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
	 IIC_Wait_Ack();	
   Write_IIC_Byte(0x00);						//write command
	 IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Command); 
	 IIC_Wait_Ack();	
   IIC_Stop();
}

void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);			      //D/C#=0; R/W#=0
	 IIC_Wait_Ack();	
   Write_IIC_Byte(0x40);			      //write data
	 IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Data);
	 IIC_Wait_Ack();	
   IIC_Stop();
}
