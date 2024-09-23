//
// Created by muatea on 2024/9/23.
//

#ifndef GD32F303RCT6_STDPERIPH_LIB_TEMPLATE_GPS_DRV_H
#define GD32F303RCT6_STDPERIPH_LIB_TEMPLATE_GPS_DRV_H

#include "gd32f30x.h"
#include "usart_drv.h"
#include "Delay.h"
#include <string.h>
#include <stdio.h>

void sendATCommand(char* command,char* response,uint32_t timeout);
void connectToOneNet(void);
void sendDataToOneNet(char* data);

#endif //GD32F303RCT6_STDPERIPH_LIB_TEMPLATE_GPS_DRV_H
