#ifndef __ADC_DRV_H
#define __ADC_DRV_H

//ͷ�ļ�
#include "gd32f30x.h"
#include "systick.h"

//��������
void ADC_MyConfig(void);
u16 Get_ADC_Value(u8 n);
void DMA_ADC_Config(void);
double Get_Tempratture(void);

float Calculate_Flame_Intensity(void);
float Calculate_Smoke_Concentration(void);
float Calculate_Temp_Concentration(void);

#endif

