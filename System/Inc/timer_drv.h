//
// Created by muatea on 2024/9/23.
//

#ifndef GD32F303RCT6_STDPERIPH_LIB_TEMPLATE_TIMER_DRV_H
#define GD32F303RCT6_STDPERIPH_LIB_TEMPLATE_TIMER_DRV_H

#include "gd32f30x.h"

void TIM5_Init(uint16_t psc, uint32_t per);
void TIMER5_IRQHandler(void);


#endif //GD32F303RCT6_STDPERIPH_LIB_TEMPLATE_TIMER_DRV_H
