#ifndef __UART_H
#define __UART_H
#include "stm32f10x.h"

#define USART_DEBUG		USART1		//���Դ�ӡ��ʹ�õĴ�����
#define USART_TY		USART2		//Ϳѻģ����ʹ�õĴ�����

void Usart1_Init(unsigned int baud);

void Usart2_Init(unsigned int baud);

void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);

void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);

void USART2_SendByte(unsigned char data);

#endif
