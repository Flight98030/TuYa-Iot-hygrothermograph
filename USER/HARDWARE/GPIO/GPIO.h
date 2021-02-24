#ifndef __GPIO_H
#define __GPIO_H
#include "stm32f10x.h"

/*宏定义*/
#define GPIOX 					GPIOB                                     
#define RCC_AHB2Periph_GPIOX    RCC_APB2Periph_GPIOB 
#define GPIO_PINX 				GPIO_Pin_11 | GPIO_Pin_10

#define LED1_OFF 				GPIO_SetBits(GPIOX, GPIO_Pin_11)
#define LED2_OFF 				GPIO_SetBits(GPIOX, GPIO_Pin_10)

#define LED1_ON 				GPIO_ResetBits(GPIOX, GPIO_Pin_11)
#define LED2_ON 				GPIO_ResetBits(GPIOX, GPIO_Pin_10)

#define	ALL_LED_OFF			    GPIO_SetBits(GPIOX, GPIO_PINX)
#define ALL_LED_ON				GPIO_ResetBits(GPIOX, GPIO_PINX)

#define KEY1										GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define KEY2										GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)


#define KEY1_PRES 	1	//KEY0按下
#define KEY2_PRES	2	//KEY1按下

void LED_GPIO_INIT(void);
void OLED_GPIO_INIT(void);
void SHT30_GPIO_INIT(void);
void KEY_GPIO_INIT(void);
u8 KEY_Scan(u8);  	//按键扫描函数	
void DHT11_GPIO_INIT(void);
void EXTIX_Init(void);
#endif
