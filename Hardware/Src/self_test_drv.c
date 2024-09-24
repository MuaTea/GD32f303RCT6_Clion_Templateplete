#include "key_drv.h"
#include "systick.h"
#include "usart_drv.h"
#include "tim_drv.h"
#include "system.h"
#include "self_test_drv.h"
#include "state_drv.h"

extern u16 second_counter;

void FlLight(u16 value)
{
	
	if (value != 0)
    {
    	
			u8 k1_pressed = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9);
			// 每秒执行一次
        	if (second_counter >= 30000)
        	{
            	second_counter = 0;
        	}
        
        	// 控制LED亮灭
        	if (second_counter <= 1000)
        	{           	
            	TIM_SetCompare1(TIM1, 100);
        	}
			else
        	{
            	TIM_SetCompare1(TIM1, 0);
        	}
		
	}  
   
}

//初始化蜂鸣器
void BuzzerInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}




