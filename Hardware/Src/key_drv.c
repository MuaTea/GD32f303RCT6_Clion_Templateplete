#include "key_drv.h"
#include "systick.h"
#include "system.h"

void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//使能外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);

	//配置GPIO
	//wk-up引脚：PA0
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;//下拉输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	//k1按键：PC9	 k2按键: PC8
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;
	GPIO_Init(GPIOC,&GPIO_InitStruct);

}


