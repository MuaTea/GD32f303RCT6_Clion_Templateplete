//
// Created by Peakors on 9/18/2023.
//

#ifndef STM32F103ZET6_STDPERIPH_LIB_TEMPLATE_RETARGET_H
#define STM32F103ZET6_STDPERIPH_LIB_TEMPLATE_RETARGET_H

#include "gd32f30x.h"
#include "stdio.h"

void printf_retarget(uint32_t bound, uint32_t usart_base);

#endif //STM32F103ZET6_STDPERIPH_LIB_TEMPLATE_RETARGET_H
