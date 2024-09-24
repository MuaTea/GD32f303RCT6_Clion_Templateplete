#include "key_drv.h"
#include "systick.h"
#include "system.h"

void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//ʹ������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);

	//����GPIO
	//wk-up���ţ�PA0
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	//k1������PC9	 k2����: PC8
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;
	GPIO_Init(GPIOC,&GPIO_InitStruct);

}


