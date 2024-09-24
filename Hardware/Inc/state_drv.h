#ifndef __STATE_DRV_H
#define __STATE_DRV_H

/* 头文件 */
#include "stm32f10x.h"

#define STATE_0		0
#define STATE_1		1
#define STATE1_0	2
#define STATE1_1	3


/* 函数声明 */
void LED_Control(u16 arr);
void AlarmHandler(u16 value);
void LED_Control1(u16 arr);
void AlarmHandler1(u16 value);


#endif

