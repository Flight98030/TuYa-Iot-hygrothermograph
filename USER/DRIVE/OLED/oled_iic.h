#ifndef __OLED_IIC_H_
#define __OLED_IIC_H_
#include "stm32f10x.h"
//-----------------OLED IIC端口定义----------------  					   
#define IIC_SCL_L			  GPIO_ResetBits(GPIOB,GPIO_Pin_8)	//SCL
#define IIC_SCL_H			 	GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define IIC_SDA_L 			GPIO_ResetBits(GPIOB,GPIO_Pin_9)	//SDA
#define IIC_SDA_H 			GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define READ_SDA				GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)
#define OLED_CMD  0																				//写命令
#define OLED_DATA 1																				//写数据
#define OLED_WR_Byte(dat,cmd)														\
{																												\
	if(cmd)		Write_IIC_Data(dat);												\
	else			Write_IIC_Command(dat);											\
}														

void IIC_Start(void);
void IIC_Stop(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
u8 IIC_Wait_Ack(void);
#endif
