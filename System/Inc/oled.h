#ifndef __OLED_H
#define __OLED_H 

#include "stdlib.h"	
#include "gd32f30x.h"

//-----------------����LED�˿ڶ���---------------- 

#define LED_ON GPIO_ResetBits(GPIOC,GPIO_Pin_12)
#define LED_OFF GPIO_SetBits(GPIOC,GPIO_Pin_12)

//-----------------OLED�˿ڶ���---------------- 

/*
GND -> �ӵ�
VCC -> ��Դ
D0 -> SCL��ʱ���ߣ�
D1 -> SDA�������ߣ�
RES -> ��λ����
DC -> ����/�����������
CS -> Ƭѡ����
*/

#define OLED_SCL_Clr() gpio_bit_reset(GPIOB, GPIO_PIN_8) // SC ��Ӧ SCL
#define OLED_SCL_Set() gpio_bit_set(GPIOB, GPIO_PIN_8)

#define OLED_SDA_Clr() gpio_bit_reset(GPIOB, GPIO_PIN_9) // D1 ��Ӧ SDA
#define OLED_SDA_Set() gpio_bit_set(GPIOB, GPIO_PIN_9)

#define OLED_RES_Clr() gpio_bit_reset(GPIOB, GPIO_PIN_6) // RST ��Ӧ��λ
#define OLED_RES_Set() gpio_bit_set(GPIOB, GPIO_PIN_6)

#define OLED_DC_Clr()  gpio_bit_reset(GPIOB, GPIO_PIN_5) // RS ��Ӧ����/����ѡ��
#define OLED_DC_Set()  gpio_bit_set(GPIOB, GPIO_PIN_5)

#define OLED_CS_Clr()  gpio_bit_reset(GPIOB, GPIO_PIN_7) // CS ��ӦƬѡ
#define OLED_CS_Set()  gpio_bit_set(GPIOB, GPIO_PIN_7)

#define OLED_BLK_Clr() gpio_bit_reset(GPIOB, GPIO_PIN_4) // BLK ��Ӧ�������
#define OLED_BLK_Set() gpio_bit_set(GPIOB, GPIO_PIN_4)




#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

void OLED_ClearPoint(u8 x,u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void OLED_WR_Byte(u8 dat,u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
void OLED_DrawCircle(u8 x,u8 y,u8 r);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);
void OLED_ShowChar6x8(u8 x,u8 y,u8 chr,u8 mode);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode);
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);
void OLED_Init(void);

#endif

