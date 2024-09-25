//
// Created by muatea on 2024/9/23.
//

#include "PWM_drv.h"

void PWM_Init(u16 arr, u16 psc) {
    /* 1. GPIO ���� */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_TIMER1);

    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);

    /* 2. ��ʱ������ */
    timer_parameter_struct timer_initpara;
    timer_initpara.prescaler = psc;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = arr;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1, &timer_initpara);

    /* 3. PWM ���� */
    timer_oc_parameter_struct timer_ocintpara;
    timer_channel_output_struct_para_init(&timer_ocintpara);

    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_LOW;
    timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_channel_output_config(TIMER1, TIMER_CH_0, &timer_ocintpara);

    /* ��ʼռ�ձ�����Ϊ 0 */
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, 0);  // ��ʼռ�ձ�Ϊ0
    timer_channel_output_mode_config(TIMER1, TIMER_CH_0, TIMER_OC_MODE_PWM1);
    timer_channel_output_shadow_config(TIMER1, TIMER_CH_0, TIMER_OC_SHADOW_ENABLE);

    /* 4. ʹ�ܶ�ʱ�� */
    timer_auto_reload_shadow_enable(TIMER1);
    timer_enable(TIMER1);
    timer_primary_output_config(TIMER1, ENABLE);
}