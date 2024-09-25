#include "tim_drv.h"

u16 idle_timer;
u16 second_counter = 0;
extern u16 blink_timer;
extern u16 breath_timer;
extern u16 alarm_timer;
extern u32 key_timer;

// 定时器初始化
void TIM_MyConfig(u16 arr,u16 psc)
{
    timer_parameter_struct timer_initpara;
    nvic_irq_enable(TIMER6_IRQn, 1, 2);

    // 使能 TIMER6 的时钟
    rcu_periph_clock_enable(RCU_TIMER6);

    // 初始化定时器
    timer_deinit(TIMER6);
    timer_initpara.prescaler = psc;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = arr;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER6, &timer_initpara);

    // 配置定时器中断
    timer_interrupt_enable(TIMER6, TIMER_INT_UP);
    timer_flag_clear(TIMER6, TIMER_FLAG_UP);

    // 使能定时器
    timer_enable(TIMER6);
}



