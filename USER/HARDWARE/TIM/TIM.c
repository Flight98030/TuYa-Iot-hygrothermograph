#include "TIM.h"

/*
**psc为预分频器,可对时钟进行1-65536分频
**cnt为16位计数器,计数达到自动重装载寄存器时产生更新事件
**arr为16位自动重装载寄存器
**则记一次数的时间为1/((TIMxCLK)/(psc+1))
**定时时间为1/((TIMxCLK)/(psc+1))*(arr+1) = (psc+1))*(arr+1)/TIMxCLK
**psc与arr都为可调范围0-65535，实现1-65536,所以输入时要注意
**因为AHB1预分频系数为2，所以TIM2-TIM7的时钟频率仍然为72M
*/

void TIM3_INIT(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);          //使能定时器时钟

	TIM_Cmd(TIM3, ENABLE);                                        //使能定时器	
	
	/*初始化定时器*/
	TIM_TimeBaseInitStrue.TIM_Period = arr ;                      //自动装载值       
	TIM_TimeBaseInitStrue.TIM_Prescaler = psc;                    //预分频系数 
	TIM_TimeBaseInitStrue.TIM_CounterMode = TIM_CounterMode_Up;   //计数模式 向上计数方式
	TIM_TimeBaseInitStrue.TIM_ClockDivision = TIM_CKD_DIV1;       //选有效值1，无影响
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStrue);               //完成初始化配置


	/*使能定时器中断*/
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);                   //使能更新中断
	
	/*开启定时器中断 初始化NVIC寄存器*/ 
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;             //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQ通道使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;  //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //响应优先级3
	NVIC_Init(&NVIC_InitStructure);

}


