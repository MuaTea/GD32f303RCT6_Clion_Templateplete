#ifndef __JDY_H_
#define __JDY_H_

#include "stm32f10x.h"
#include <stdio.h>

void USART3_Init(void);
void USART3_SendChar(char c);
void USART3_SendString(char* str);
char USART3_ReceiveChar(void);
void Bluetooth_TestJDY31(void);

#endif
