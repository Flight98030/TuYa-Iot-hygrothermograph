#ifndef __SHT30_H
#define __SHT30_H 
#include "stm32f10x.h"  
#include "delay.h"

#include "sys.h"
#include "stdio.h"

#include "string.h"
//IO方向设置
/*群友*/
//#define SDA_IN()  {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=0<<7*2;}	//PB11输入模式
//#define SDA_OUT() {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=1<<7*2;} //PB11输出模式

 /*原子*/
//#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
//#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO操作函数	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //输入SDA 

//SHT30操作
#define SHT_Addr		0x44
#define SHT_Write		0
#define SHT_Read		1

//extern u8 humiture_buff1[20];
void SHT30_read_result(u8 addr);

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口
void SDA_IN(void);                  //设置SDA输入
void SDA_OUT(void);				    //设置SDA输出
void SHT_IIC_Start(void);				//发送IIC开始信号
void SHT_IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 SHT_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void SHT_Init(void);                 //SHT30初始化
u8 sht30_read_temp_humi(float *temperature,float *humidity);//SHT读数据

//void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
//u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  



 
#endif

