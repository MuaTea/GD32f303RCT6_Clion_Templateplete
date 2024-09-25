#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "gd32f30x.h"

void Systick_Init(uint8_t ahbclk);
void delay_us(uint32_t n);
void delay_ms(uint32_t n);
void delay_nms(uint32_t n);

#endif

