#include "systick.h"
uint32_t nus;//Î¢Ãë
uint32_t nms;//ºÁÃë

void Systick_Init(uint8_t ahbclk)
{
	SysTick -> CTRL &= ~(0x01<<2);

	nus = ahbclk/8;
	nms = nus * 1000;
}

void delay_us(uint32_t n)
{
	uint32_t temp;
	SysTick -> LOAD = n*nus;
	SysTick -> VAL = 0x00;
	SysTick -> CTRL |= 0x01;
	do{
		temp = SysTick -> CTRL;
	}while((temp&0x01) &&   !(temp & (0x01<<16)));
	SysTick -> CTRL &= ~(0x01);
	SysTick -> VAL = 0x00;
}

void delay_ms(uint32_t n)
{
	uint32_t temp;
	SysTick -> LOAD = n*nms;
	SysTick -> VAL = 0x00;
	SysTick -> CTRL |= 0x01;
	do{
		temp = SysTick -> CTRL;
	}while((temp&0x01) &&   !(temp & (0x01<<16)));
	SysTick -> CTRL &= ~(0x01);
	SysTick -> VAL = 0x00;
}

void delay_nms(uint32_t n)
{
	uint8_t i;
	uint32_t remain,round;
	round = n/1000;
	remain = n%1000;
	for(i = 0;i < round;i++)
	{
		delay_ms(1000);
	}
	if(remain != 0)
	{
		delay_ms(remain);
	}
}

