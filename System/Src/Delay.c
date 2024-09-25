#include "../../StartLibrary/gd32f30x.h"

extern uint32_t SystemCoreClock;  // 引用系统时钟频率，配置为120MHz

/**
  * @brief  微秒级延时
  * @param  xus 延时时长，范围：0~233015,xus 不能大于 1118481 us (2^24)/(120/8)
  * @retval 无
  */
void Delay_us(uint32_t xus)
{
	uint32_t ticks = (SystemCoreClock / 8000000) * xus;  // 根据系统时钟计算每微秒的ticks
	SysTick->LOAD = ticks - 1;                // 设置定时器重装值
	SysTick->VAL = 0x00;                      // 清空当前计数值
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;  // 设置时钟源为HCLK，启动定时器
	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));  // 等待计数到0
	SysTick->CTRL = 0x00;                     // 关闭定时器
}

/**
  * @brief  毫秒级延时
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_ms(uint32_t xms)
{
	while(xms--)
	{
		Delay_us(1000);  // 每次延时1毫秒
	}
}

/**
  * @brief  秒级延时
  * @param  xs 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_s(uint32_t xs)
{
	while(xs--)
	{
		Delay_ms(1000);  // 每次延时1秒
	}
}

volatile uint32_t SysTick_Count = 0;

// SysTick 初始化，设置时钟源为系统时钟，SysTick 每 1ms 触发一次中断
void SysTick_Init(void)
{
    uint32_t SystemCoreClock = rcu_clock_freq_get(CK_SYS);  // 获取系统时钟频率

    SysTick_Config(SystemCoreClock / 1000);  // 每 1ms 产生一次 SysTick 中断
}

// SysTick 中断处理函数
void SysTick_Handler(void)
{
    if (SysTick_Count > 0) {
        SysTick_Count--;  // 递减计数器
    }
}

// 毫秒级延时函数
void Systick_DelayMs(uint32_t ms)
{
    SysTick_Count = ms;
    while (SysTick_Count != 0) {
        // 等待计数器归零
    }
}
