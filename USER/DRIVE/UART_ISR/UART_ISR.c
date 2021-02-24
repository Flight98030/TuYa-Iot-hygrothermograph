#include "UART_ISR.h"

//void USART3_IRQHandler(void)
//{
//	if(USART_GetITStatus(USART3,USART_IT_RXNE)){
//		USART_SendData(USART3,USART_ReceiveData(USART3));
//		while(!USART_GetFlagStatus(USART3,USART_FLAG_TC));
//	}
//}

//void USART2_IRQHandler(void)
//{
//	if(USART_GetITStatus(USART2,USART_IT_RXNE))
//	{
//		USART_SendData(USART2,USART_ReceiveData(USART2));
//		while(!USART_GetFlagStatus(USART2,USART_FLAG_TC));
//	}
//}

//void USART1_IRQHandler(void)
//{
//	if(USART_GetITStatus(USART1,USART_IT_RXNE))
//	{
//		USART_SendData(USART2,USART_ReceiveData(USART1));
//		while(!USART_GetFlagStatus(USART2,USART_FLAG_TC));
//	}
//}
#if 0
int fputc(int ch,FILE*f)
{
	USART_SendData(USART3,(uint8_t)ch);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);
	return ch;
}

int fgetc(FILE*f)
{
	while(USART_GetFlagStatus(USART3,USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(USART3);
}
#endif
