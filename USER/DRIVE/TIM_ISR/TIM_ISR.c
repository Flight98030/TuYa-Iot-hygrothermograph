#include "TIM_ISR.h"
#include "GPIO.h"

int16_t tim = 0;
int16_t flag = 0;
void TIM3_IRQHandler(void)
{ 
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)    //����Ǹ����ж�
	{
		tim ++;
		if(tim >= 1000)
		{
			//flag = -flag;
			//if(flag == 1)	ALL_LED_ON;
			//else					ALL_LED_OFF;
			flag ++;
			tim = 0;
		}
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);      //�����־λ����,���TIM3�ĸ����жϵı�־λ
	}
}
