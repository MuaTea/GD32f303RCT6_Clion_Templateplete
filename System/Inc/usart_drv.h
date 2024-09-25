//
// Created by muatea on 2024/9/23.
//

#ifndef GD32F303RCT6_STDPERIPH_LIB_TEMPLATE_USART_DRV_H
#define GD32F303RCT6_STDPERIPH_LIB_TEMPLATE_USART_DRV_H

#include <stddef.h>
#include "gd32f30x_usart.h"
#include "gd32f30x.h"
#include "gd32f30x_gpio.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define USART_DEBUG USART0

void USART_SendString(uint32_t usart_periph, unsigned char *str, uint32_t len);
//void USART_ReceiveLine(uint32_t usart_periph, char *buffer, size_t buffer_size);
int check_USART_ReceiceLine(uint32_t usart_periph, char *buffer, size_t buffer_size);
void UsartPrintf(uint32_t usart_periph, const char *format, ...);
void USART_SendData(uint32_t usart_base, uint16_t data);
void USART0_Init(unsigned int baud);
void USART1_Init(uint32_t baudval);
void USART2_Init(unsigned int baud);

#endif //GD32F303RCT6_STDPERIPH_LIB_TEMPLATE_USART_DRV_H
