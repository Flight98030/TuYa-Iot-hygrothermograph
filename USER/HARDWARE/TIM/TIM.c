#include "TIM.h"

/*
**pscΪԤ��Ƶ��,�ɶ�ʱ�ӽ���1-65536��Ƶ
**cntΪ16λ������,�����ﵽ�Զ���װ�ؼĴ���ʱ���������¼�
**arrΪ16λ�Զ���װ�ؼĴ���
**���һ������ʱ��Ϊ1/((TIMxCLK)/(psc+1))
**��ʱʱ��Ϊ1/((TIMxCLK)/(psc+1))*(arr+1) = (psc+1))*(arr+1)/TIMxCLK
**psc��arr��Ϊ�ɵ���Χ0-65535��ʵ��1-65536,��������ʱҪע��
**��ΪAHB1Ԥ��Ƶϵ��Ϊ2������TIM2-TIM7��ʱ��Ƶ����ȻΪ72M
*/

void TIM3_INIT(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);          //ʹ�ܶ�ʱ��ʱ��

	TIM_Cmd(TIM3, ENABLE);                                        //ʹ�ܶ�ʱ��	
	
	/*��ʼ����ʱ��*/
	TIM_TimeBaseInitStrue.TIM_Period = arr ;                      //�Զ�װ��ֵ       
	TIM_TimeBaseInitStrue.TIM_Prescaler = psc;                    //Ԥ��Ƶϵ�� 
	TIM_TimeBaseInitStrue.TIM_CounterMode = TIM_CounterMode_Up;   //����ģʽ ���ϼ�����ʽ
	TIM_TimeBaseInitStrue.TIM_ClockDivision = TIM_CKD_DIV1;       //ѡ��Чֵ1����Ӱ��
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStrue);               //��ɳ�ʼ������


	/*ʹ�ܶ�ʱ���ж�*/
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);                   //ʹ�ܸ����ж�
	
	/*������ʱ���ж� ��ʼ��NVIC�Ĵ���*/ 
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;             //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQͨ��ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;  //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //��Ӧ���ȼ�3
	NVIC_Init(&NVIC_InitStructure);

}


