#include "SHT30.h" 
#include "delay.h"
#include "GPIO.h"

//初始化IIC
//void IIC_Init(void)
//{					     
//	GPIO_InitTypeDef GPIO_InitStructure;
//	//RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟 
//	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
//	   
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
// 
//	IIC_SCL=1;
//	IIC_SDA=1;

//}

void SDA_IN(void)
{
	GPIO_InitTypeDef  gpio;    
	
	gpio.GPIO_Pin 	= GPIO_Pin_7;    
    gpio.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &gpio);
}
 
void SDA_OUT(void)
{
	GPIO_InitTypeDef   	gpio;    
	gpio.GPIO_Pin  	= 	GPIO_Pin_7;
	gpio.GPIO_Mode 	= 	GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = 	GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &gpio);

}

//产生IIC起始信号
void SHT_IIC_Start(void)
{
	SDA_OUT();     	//sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;		//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;		//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void SHT_IIC_Stop(void)
{
	SDA_OUT();		//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;		//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;		//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 SHT_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      			//SDA设置为输入  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);
	
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			SHT_IIC_Stop();
			return 1;
		}
	}
	
	IIC_SCL=0;				//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;			//拉低时钟开始数据传输
	
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;	
	SDA_IN();			//SDA设置为输入
	
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(2); 
    }					 
    
	if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    
	return receive;
}

/*-----------------------------------------
首先是写操作，写入一个16bit的命令。
1.I2C开始信号->7位I2C地址+0（写操作标志位）
 （前面介绍了，如果ADDR接低电平，那么这里就是0x88，如果接高电平就是0x8a）
2.命令MSB->命令LSB
 （这里的16位数据0x2C06是默认的操作，表示的意思是高速测量开启时钟拉伸
3.I2C停止信号
-----------------------------------------*/

/*SHT30初始化*/
void SHT_Init(void)
{
//	
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 		//使能B端口时钟
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	 
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 		//推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				//速度50MHz
//	
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  						//初始化GPIOB6\7
//	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);	
	
	SHT30_GPIO_INIT();	
    delay_ms(300); //250
   	
	SHT_IIC_Start();
	//IIC_Send_Byte(addr<<1 | write);//写7位I2C设备地址加0作为写取位,1为读取位
   	IIC_Send_Byte(0x88);    //SHT30地址0x44,后加一个0(写操作)，实际发0x88
   	SHT_IIC_Wait_Ack();		

   	IIC_Send_Byte(0x23);    //MSB  	//0x2c06读一次
   	SHT_IIC_Wait_Ack();				//0x2130每1ms自动循环读取
									//0x2236\0x2334\0x2737每2、4、10
   	IIC_Send_Byte(0x34);    //LSB
   	SHT_IIC_Wait_Ack();

    SHT_IIC_Stop();
   
   delay_ms(150); 
   
}


/*SHT30读数据*/
u8 sht30_read_temp_humi(float *temperature,float *humidity)
{
	u16 tem,hum;
	u16 buff[6];
	float Temperature=0;
	float Humidity=0;
	
	//这里和前面的一样，也是写一个命令给SHT30，这个命令是访问SHT30转换结果的寄存器的
   	SHT_IIC_Start();
  	IIC_Send_Byte(0x88);
  	SHT_IIC_Wait_Ack();
	
   	IIC_Send_Byte(0xe0);
   	SHT_IIC_Wait_Ack();
   	IIC_Send_Byte(0x00);
   	SHT_IIC_Wait_Ack();
	//下面是开始读取数据，其中的数组p存放结果，前三个存放温度值，后三个是湿度值，在前三个温度值里面，
	//p[0]是温度的高八位，p[1]是低八位，p[2]是CRC校验，有关CRC校验的知识我是百度上面看的，
	//要是各位不懂得话，可以不用crc校验，直接用p[0]、p[1]就可以转换出来温度的值。
   	SHT_IIC_Start();
   	IIC_Send_Byte(0x89);		//SHT30地址0x44,后加一个1(读操作)，实际发0x89
   	SHT_IIC_Wait_Ack();
   
   	//前五次读取都要发送ack信号，最后一次就不用发了。
   	buff[0] = IIC_Read_Byte(1);
   	buff[1] = IIC_Read_Byte(1);
   	buff[2] = IIC_Read_Byte(1);
   	buff[3] = IIC_Read_Byte(1);
   	buff[4] = IIC_Read_Byte(1);
   	buff[5] = IIC_Read_Byte(0);
   	SHT_IIC_Stop();

   	tem = ((buff[0]<<8) | buff[1]);//温度拼接
	hum = ((buff[3]<<8) | buff[4]);//湿度拼接

	/*转换实际温度*/
	Temperature= (175.0*(float)tem/65535.0-45.0) ;// T = -45 + 175 * tem / (2^16-1)
	Humidity= (100.0*(float)hum/65535.0);// RH = hum*100 / (2^16-1)
	
	if((Temperature>=-20)&&(Temperature<=125)&&(Humidity>=0)&&(Humidity<=100))//过滤错误数据
	{
		*temperature = Temperature;
		*humidity    = Humidity;
	}

	hum=0;
	tem=0;

   
}

