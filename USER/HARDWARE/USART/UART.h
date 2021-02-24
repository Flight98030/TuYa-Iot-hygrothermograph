#ifndef __UART_H
#define __UART_H
#include "stm32f10x.h"

#define USART_DEBUG		USART1		//调试打印所使用的串口组
#define USART_TY		USART2		//涂鸦模组所使用的串口组

void Usart1_Init(unsigned int baud);

void Usart2_Init(unsigned int baud);

void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);

void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);

void USART2_SendByte(unsigned char data);

#endif
