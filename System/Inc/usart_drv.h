//
// Created by muatea on 2024/9/23.
//

#ifndef GD32F303RCT6_STDPERIPH_LIB_TEMPLATE_USART_DRV_H
#define GD32F303RCT6_STDPERIPH_LIB_TEMPLATE_USART_DRV_H

#include <stddef.h>
#include "gd32f30x_usart.h"
#include "gd32f30x.h"
#include "gd32f30x_gpio.h"

void USART_SendString(uint32_t usart_periph,char *str);
void USART_ReceiveLine(uint32_t usart_periph, char *buffer, size_t buffer_size);
int check_USART_ReceiceLine(uint32_t usart_periph, char *buffer, size_t buffer_size);

#endif //GD32F303RCT6_STDPERIPH_LIB_TEMPLATE_USART_DRV_H
