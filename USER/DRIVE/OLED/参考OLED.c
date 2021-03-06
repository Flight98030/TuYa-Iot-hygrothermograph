#include "i2c.h"
#include "OLED_I2C.h"

#include "codetab.h"



const unsigned char maohao[]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x0F,0x1F,0x1F,0x1F,0x1F,0x0F,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x07,0x0F,0x0F,0x0F,0x0F,0x07,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"未命名文件",0*/
/* (16 X 48 )*/
};

const unsigned char num24x48[][16*9]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0xF8,0x3C,0x0E,
0x06,0x03,0x01,0x01,0x01,0x01,0x02,0x06,0x1C,0xF8,0xF0,0x80,0x00,0x00,0x00,0x00,
0x00,0x00,0xFC,0xFF,0xFF,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x01,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,0x3F,0xFF,0xFF,0xC0,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0xFF,0x3F,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0x07,0x1F,0x3C,0x70,0x40,0xC0,0x80,0x80,0x80,0x80,0x40,0x60,
0x38,0x1F,0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"0",0*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x08,
0x08,0x08,0x08,0xFC,0xFE,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0xC0,0xFF,0xFF,0xFF,0xC0,0x80,
0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"1",1*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xF8,0xFC,0x04,0x02,
0x02,0x01,0x01,0x01,0x01,0x01,0x03,0x06,0x1E,0xFC,0xF8,0xE0,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x03,0x07,0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xE0,
0xF8,0x7F,0x1F,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,
0xE0,0x70,0x38,0x1C,0x0E,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0xE0,0xF8,0xEC,0xE6,0xE3,0xE1,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,
0xE0,0xF0,0xFC,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"2",2*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xF8,0xFC,0xC6,
0x02,0x01,0x01,0x01,0x01,0x01,0x03,0x06,0x1E,0xFC,0xF8,0xE0,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x80,0x80,0x80,0x80,0xC0,0x40,0x60,
0x38,0x1F,0x0F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x03,0x0E,0xFC,0xF8,0xE0,0x00,0x00,0x00,
0x00,0x00,0x00,0x0F,0x1F,0x3F,0x67,0x40,0x80,0x80,0x80,0x80,0x80,0x80,0xC0,0x40,
0x60,0x38,0x1F,0x0F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"3",3*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xC0,0x70,0x18,0xFE,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x70,0x18,0x0E,0x03,0x01,0x00,0x00,0xFF,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x70,0x78,0x6E,0x63,0x60,0x60,
0x60,0x60,0x60,0x60,0x60,0x60,0xFF,0xFF,0xFF,0x60,0x60,0x60,0x60,0x60,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0xFF,0xFF,
0xFF,0xC0,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"4",4*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x07,0x07,
0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xFF,0xC0,0xE0,0x30,0x30,0x18,0x18,0x18,0x18,0x18,0x18,0x38,
0x70,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC1,0xC1,0x80,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFE,0x00,0x00,0x00,
0x00,0x00,0x00,0x07,0x1F,0x3F,0x63,0x40,0xC0,0x80,0x80,0x80,0x80,0x80,0xC0,0x40,
0x70,0x3C,0x1F,0x0F,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"5",5*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xF0,0x18,0x0C,
0x06,0x02,0x01,0x01,0x01,0x01,0x01,0x3F,0x3E,0x3E,0x38,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0xF8,0xFF,0xFF,0x01,0xC0,0xC0,0x60,0x60,0x30,0x30,0x30,0x30,0x30,0x70,
0xE0,0xE0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0x03,0x01,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFC,0x00,0x00,0x00,
0x00,0x00,0x00,0x03,0x0F,0x1F,0x38,0x60,0x40,0xC0,0x80,0x80,0x80,0x80,0x80,0x40,
0x60,0x38,0x1F,0x0F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"6",6*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x3F,0x0F,0x0F,0x07,
0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0xC7,0x77,0x1F,0x0F,0x03,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xF0,0x1C,0x07,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xE0,0xFC,0xFF,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"7",7*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xF8,0xFC,0x0E,0x06,
0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x06,0x1E,0xFC,0xF8,0xE0,0x00,0x00,0x00,
0x00,0x00,0x00,0x03,0x0F,0x1F,0x3E,0xFC,0xF8,0xF0,0xE0,0xE0,0xC0,0xC0,0xC0,0xE0,
0x30,0x3C,0x1F,0x0F,0x03,0x00,0x00,0x00,0x00,0x00,0xE0,0xF8,0xFC,0x1E,0x07,0x01,
0x00,0x00,0x00,0x01,0x01,0x03,0x07,0x07,0x1F,0x3E,0xFC,0xF8,0xE0,0x00,0x00,0x00,
0x00,0x00,0x07,0x0F,0x1F,0x38,0x60,0x40,0xC0,0x80,0x80,0x80,0x80,0x80,0x80,0x40,
0x60,0x38,0x3F,0x0F,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"8",8*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xF0,0xF8,0x3C,0x0E,0x02,
0x03,0x01,0x01,0x01,0x01,0x01,0x02,0x02,0x0C,0x38,0xF0,0xC0,0x00,0x00,0x00,0x00,
0x00,0x00,0x3F,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x80,0xE0,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x07,0x0E,
0x0E,0x0C,0x0C,0x0C,0x0C,0x06,0x06,0x03,0x01,0xE0,0xFF,0xFF,0x0F,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x38,0x7C,0x7C,0xF8,0x80,0x80,0x80,0x80,0xC0,0x40,0x60,0x38,
0x1E,0x0F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"9",9*/

};
/////////////////////////////////////////////////////////////////////////
// 汉字字模表                                                          //
// 汉字库: 宋体16.dot,横向取模左高位,数据排列:从左到右从上到下         //
/////////////////////////////////////////////////////////////////////////
typFNT_GB16 const GBHZ_16[] =          // 数据表
{
"贵", 0x01,0x00,0x01,0x08,0x3F,0xFC,0x21,0x08,
      0x3F,0xF8,0x01,0x00,0xFF,0xFE,0x00,0x10,
      0x1F,0xF8,0x10,0x10,0x11,0x10,0x11,0x10,
      0x11,0x10,0x02,0xC0,0x0C,0x30,0x30,0x08,

"州", 0x10,0x04,0x10,0x84,0x10,0x84,0x10,0x84,
      0x54,0xA4,0x52,0x94,0x52,0x94,0x90,0x84,
      0x10,0x84,0x10,0x84,0x10,0x84,0x10,0x84,
      0x20,0x84,0x20,0x84,0x40,0x04,0x00,0x04,

"航", 0x08,0x40,0x10,0x30,0x3E,0x14,0x23,0xFE,
      0x32,0x00,0x2A,0x08,0x22,0x7C,0xFE,0x48,
      0x22,0x48,0x32,0x48,0x2A,0x48,0x22,0x48,
      0x22,0x4A,0x22,0x4A,0x4A,0x86,0x85,0x00,

"天", 0x00,0x08,0x7F,0xFC,0x01,0x00,0x01,0x00,
      0x01,0x00,0x01,0x04,0xFF,0xFE,0x01,0x00,
      0x02,0x80,0x02,0x80,0x04,0x40,0x04,0x40,
      0x08,0x20,0x10,0x10,0x20,0x0E,0xC0,0x04,

"职", 0x04,0x00,0xFE,0x04,0x24,0xFE,0x24,0x84,
      0x3C,0x84,0x24,0x84,0x24,0x84,0x3C,0x84,
      0x24,0xFC,0x24,0x84,0x27,0x48,0xFC,0x48,
      0x44,0x84,0x04,0x86,0x05,0x02,0x04,0x00,

"业", 0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x40,
      0x44,0x44,0x24,0x44,0x24,0x48,0x14,0x48,
      0x14,0x50,0x14,0x50,0x14,0x60,0x04,0x40,
      0x04,0x40,0x04,0x44,0xFF,0xFE,0x00,0x00,

"技", 0x10,0x40,0x10,0x40,0x10,0x48,0x13,0xFC,
      0xFC,0x40,0x10,0x40,0x10,0x40,0x13,0xF8,
      0x1A,0x08,0x31,0x10,0xD1,0x10,0x10,0xA0,
      0x10,0x40,0x10,0xB0,0x51,0x0E,0x26,0x04,

"术", 0x01,0x00,0x01,0x40,0x01,0x30,0x01,0x10,
      0x01,0x04,0xFF,0xFE,0x01,0x00,0x03,0x80,
      0x05,0x40,0x09,0x20,0x11,0x10,0x21,0x0E,
      0xC1,0x04,0x01,0x00,0x01,0x00,0x01,0x00,

"学", 0x22,0x08,0x11,0x08,0x11,0x10,0x00,0x20,
      0x7F,0xFE,0x40,0x02,0x80,0x04,0x1F,0xE0,
      0x00,0x40,0x01,0x84,0xFF,0xFE,0x01,0x00,
      0x01,0x00,0x01,0x00,0x05,0x00,0x02,0x00,

"院", 0x00,0x80,0x78,0x40,0x4F,0xFE,0x54,0x02,
      0x58,0x14,0x63,0xF8,0x50,0x00,0x48,0x08,
      0x4F,0xFC,0x48,0xA0,0x68,0xA0,0x50,0xA0,
      0x41,0x22,0x41,0x22,0x42,0x1E,0x4C,0x00
};

// 汉字表：
// 贵州航天职业技术学院




void OLED_WriteByte(u8 Addr,u8 Data)
{
	IIC_Start();
	
	IIC_Send_Byte(OLED_ADDRESS);
	IIC_Wait_Ack();
	
	IIC_Send_Byte(Addr);
	IIC_Wait_Ack();
	
	IIC_Send_Byte(Data);
	IIC_Wait_Ack();
	
	IIC_Stop();
}
void WriteCmd(u8 I2C_Command)//写命令
{
	OLED_WriteByte(0x00, I2C_Command);
}

void WriteDat(u8 I2C_Data)//写数据
{
	OLED_WriteByte(0x40, I2C_Data);
}

void OLED_Init(void)
{
	delay_ms(100); //这里的延时很重要
	
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //亮度调节 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}

void OLED_SetPos(u8 x, u8 y) //设置起始点坐标
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

void OLED_Fill(u8 fill_Data)//全屏填充
{
	u8 m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				WriteDat(fill_Data);
			}
	}
}

void OLED_CLS(void)//清屏
{
	OLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : 将OLED从休眠中唤醒
//--------------------------------------------------------------
void OLED_ON(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X14);  //开启电荷泵
	WriteCmd(0XAF);  //OLED唤醒
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : 让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X10);  //关闭电荷泵
	WriteCmd(0XAE);  //OLED休眠
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(u8 x, u8 y, u8 ch[], u8 TextSize)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); ch[] -- 要显示的字符串; TextSize -- 字符大小(1:6*8 ; 2:8*16)
// Description    : 显示codetab.h中的ASCII字符,有6*8和8*16可选择
//--------------------------------------------------------------
void OLED_ShowStr(u8 x, u8 y, u8 *ch, u8 TextSize)
{
	u8 c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(*ch != '\0')
			{
				c = *ch - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(*ch != '\0')
			{
				c = *ch - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				ch++;
			}
		}break;
	}
}
//void OLED_Show_24X54(u8 x,u8 y,u8*dat)
//{
//	u16 j=0;
//	u8 tmp=0;
//	while(*dat!='\0'){
//		j=*dat;
//		OLED_SetPos(x,y);
//		for(tmp=0;tmp<24;tmp++){
//			WriteDat(Show24x54[j*7]);

//		}
//		
//	
//	}
//	
//}	


//--------------------------------------------------------------
// Prototype      : void OLED_ShowCN(u8 x, u8 y, u8 N)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); N:汉字在codetab.h中的索引
// Description    : 显示codetab.h中的汉字,16*16点阵
//--------------------------------------------------------------
void OLED_ShowCN(u8 x, u8 y, u8 N)
{
	u8 wm=0;
	unsigned int  adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x161[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x161[adder]);
		adder += 1;
	}
}

void OLED_ShowST16_1(u8 n)
{
	u8 wm=0;
	
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(   GBST_16[n].Msk[wm]    );

	}
}
void OLED_ShowST16_2(u8 n)
{
	u8 wm=0;
	
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(   GBST_16[n].Msk[16+wm]    );

	}
}

u8 getIndex_From_GBST_16(signed char hz[])
{
	u8 i;
	for(i=0;i<HZMAX;i++)
	{
		if((GBST_16[i].Index[0] == hz[0])&&(GBST_16[i].Index[1] == hz[1]))return i;
	}
	return 255;
	//

}

void OLED_ShowString16(u8 x, u8 y, u8 str[])
{
	u8 hzn,i,j;
	for(i=0;str[i]!='\0';i++)
	{
		if(str[i]<127)//如果是ASCII字符，就按ASCII显示
		{
			OLED_SetPos(x,y);
			for(j=0;j<8;j++)
			//WriteDat(F8X16[0+j]);
			WriteDat(F8X16[(str[i]-32)*16+j]);
			OLED_SetPos(x,y+1);
			for(j=0;j<8;j++)
			//WriteDat(F8X16[0+8+j]);			
			WriteDat(F8X16[(str[i]-32)*16+8+j]);	
			x=x+8;
		}	
		else//如果是汉字，就在字库中找下标并显示
		{
			hzn=getIndex_From_GBST_16((signed char)str+i);//找下标
			OLED_SetPos(x,y);//上半部分
			OLED_ShowST16_1(hzn);
			
			OLED_SetPos(x, y+1);//下半部分
			OLED_ShowST16_2(hzn);	
		
			x=x+16;
			//======这个不可随便乱动
			i++;
		}
	}

}

//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]);
// Calls          : 
// Parameters     : x0,y0 -- 起始点坐标(x0:0~127, y0:0~7); x1,y1 -- 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
// Description    : 显示BMP位图
//--------------------------------------------------------------
void OLED_DrawBMP(u8 x0,u8 y0,u8 x1,u8 y1,u8 *BMP)
{
	unsigned int j=0;
	u8 x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			WriteDat(BMP[j++]);
		}
	}
}
