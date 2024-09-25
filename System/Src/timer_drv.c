//
// Created by muatea on 2024/9/23.
//

#include "timer_drv.h"

void TIM5_Init(uint16_t psc, uint32_t per)
{
    // 使能外设时钟
    rcu_periph_clock_enable(RCU_TIMER5);

    // 定时器初始化
    timer_deinit(TIMER5);

    timer_parameter_struct initpara;
    initpara.counterdirection = TIMER_COUNTER_UP; // 向上计数
    initpara.period = per;  // 重装载值
    initpara.prescaler = psc;  // 分频系数
    timer_init(TIMER5, &initpara);

    timer_interrupt_enable(TIMER5, TIMER_INT_UP);
    nvic_irq_enable(TIMER5_IRQn, 0, 1);

    timer_enable(TIMER5); // 使能定时器
}

void TIMER5_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMER5, TIMER_INT_FLAG_UP) == SET)
    {
        timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP);
        gpio_bit_write(GPIOA, GPIO_PIN_0, !(gpio_output_bit_get(GPIOA, GPIO_PIN_0)));
    }
}
