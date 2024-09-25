#include "oled_temp.h"

extern float temperature;
extern float humidity;

void Oled_Temp(void)
{
    // ����Ҫʱ�����¶�Ũ��ֵ
    GXHT3X_ReadTempHum();
	delay_ms(1000);
    OLED_ShowString(30, 25,"Temp = ",16,1);
	OLED_ShowNum(80, 25, temperature, 2, 16, 1);
	OLED_Refresh();
}


