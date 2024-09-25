#ifndef __DELAY_H
#define __DELAY_H

void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void Delay_s(uint32_t s);

void SysTick_Init(void);
void SysTick_Handler(void);
void Systick_DelayMs(uint32_t ms);

#endif
