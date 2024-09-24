#include "tim_drv.h"
#include "system.h"
#include "state_drv.h"

u16 idle_timer;
u16 second_counter = 0;
extern u16 blink_timer;
extern u16 breath_timer;
extern u16 alarm_timer;
extern u32 key_timer;


//定时器初始化
void TIM_MyConfig(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	//使能外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	//初始化配置定时器
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);

	//配置定时器中断类型
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_ClearFlag(TIM6,TIM_FLAG_Update);
	
	//初始化配置NVIC
	NVIC_InitStruct.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);

	//使能定时器
	TIM_Cmd(TIM6, ENABLE);
}

// 中断服务函数
void TIM6_IRQHandler(void)
{
    // 检查TIM6是否发生了更新中断
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
    {
        // 增加空闲计时器
        idle_timer++;
		blink_timer++;
		alarm_timer++;
		breath_timer++;
		second_counter++;
		key_timer++;
        // 如果空闲计时器达到最大值，则重置为0
        if (idle_timer >= 65535)
        {
            idle_timer = 0;
        }
		if (blink_timer >= 65535)
        {
            blink_timer = 0;
        }
		if (alarm_timer >=  65535)
        {
            alarm_timer = 0;
        }
		if (breath_timer >= 65535)
        {
            breath_timer = 0;
        }
		if (second_counter >= 65535)
        {
            second_counter = 0;
        }
		if (key_timer >= 0xFFFF)
        {
            key_timer = 0;
        } 

     
		LED_Control(720);
    }


    // 清除TIM6的更新中断标志
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    
}


