#include "relay_drv.h"
#include "system.h"

//�̵�����ʼ��
void Relay_Init(void)
{
	//GPIO��ʼ��
	GPIO_InitTypeDef GPIO_InitStructure;

    // ���� GPIOA ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // ���� GPIOA ���� 0 Ϊ�������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//���Ƽ̵���
void Relay_Control(u8 state)
{
    if (state) {
        // �̵�����
        //GPIO_SetBits(GPIOA, GPIO_Pin_4);
		PAin(4) = 1;
    } else {
        // �̵�����
        //GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		PAin(4) = 0;
    }
}


