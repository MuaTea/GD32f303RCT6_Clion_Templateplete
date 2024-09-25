#ifndef __OLED_H
#define __OLED_H 

#include "stdlib.h"	
#include "gd32f30x.h"

//-----------------测试LED端口定义---------------- 

#define LED_ON GPIO_ResetBits(GPIOC,GPIO_Pin_12)
#define LED_OFF GPIO_SetBits(GPIOC,GPIO_Pin_12)

//-----------------OLED端口定义---------------- 

/*
GND -> 接地
VCC -> 电源
D0 -> SCL（时钟线）
D1 -> SDA（数据线）
RES -> 复位引脚
DC -> 数据/命令控制引脚
CS -> 片选引脚
*/

#define OLED_SCL_Clr() gpio_bit_reset(GPIOB, GPIO_PIN_8) // SC 对应 SCL
#define OLED_SCL_Set() gpio_bit_set(GPIOB, GPIO_PIN_8)

#define OLED_SDA_Clr() gpio_bit_reset(GPIOB, GPIO_PIN_9) // D1 对应 SDA
#define OLED_SDA_Set() gpio_bit_set(GPIOB, GPIO_PIN_9)

#define OLED_RES_Clr() gpio_bit_reset(GPIOB, GPIO_PIN_6) // RST 对应复位
#define OLED_RES_Set() gpio_bit_set(GPIOB, GPIO_PIN_6)

#define OLED_DC_Clr()  gpio_bit_reset(GPIOB, GPIO_PIN_5) // RS 对应数据/命令选择
#define OLED_DC_Set()  gpio_bit_set(GPIOB, GPIO_PIN_5)

#define OLED_CS_Clr()  gpio_bit_reset(GPIOB, GPIO_PIN_7) // CS 对应片选
#define OLED_CS_Set()  gpio_bit_set(GPIOB, GPIO_PIN_7)

#define OLED_BLK_Clr() gpio_bit_reset(GPIOB, GPIO_PIN_4) // BLK 对应背光控制
#define OLED_BLK_Set() gpio_bit_set(GPIOB, GPIO_PIN_4)




#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

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

