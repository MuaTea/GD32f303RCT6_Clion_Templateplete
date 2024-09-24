#include "tim_drv.h"
#include "system.h"
#include "state_drv.h"

u16 idle_timer;
u16 second_counter = 0;
extern u16 blink_timer;
extern u16 breath_timer;
extern u16 alarm_timer;
extern u32 key_timer;


//��ʱ����ʼ��
void TIM_MyConfig(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	//ʹ������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	//��ʼ�����ö�ʱ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);

	//���ö�ʱ���ж�����
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_ClearFlag(TIM6,TIM_FLAG_Update);
	
	//��ʼ������NVIC
	NVIC_InitStruct.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);

	//ʹ�ܶ�ʱ��
	TIM_Cmd(TIM6, ENABLE);
}

// �жϷ�����
void TIM6_IRQHandler(void)
{
    // ���TIM6�Ƿ����˸����ж�
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
    {
        // ���ӿ��м�ʱ��
        idle_timer++;
		blink_timer++;
		alarm_timer++;
		breath_timer++;
		second_counter++;
		key_timer++;
        // ������м�ʱ���ﵽ���ֵ��������Ϊ0
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


    // ���TIM6�ĸ����жϱ�־
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    
}


