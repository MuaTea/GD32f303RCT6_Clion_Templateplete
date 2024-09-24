#include "jdy.h"

void USART3_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStruct;

    // 1. ���� GPIOB �� USART3 ��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    // 2. ���� USART3 TX (PB10) �� RX (PB11)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  // TX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // �����������
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  // RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  // ��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 3. ���� USART3
    USART_InitStructure.USART_BaudRate = 9600;  // JDY-31 Ĭ�ϲ�����Ϊ 9600
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);

    // 4. ���� USART3
    USART_Cmd(USART3, ENABLE);

    // 5. ���� NVIC �ж�
    NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
}

void USART3_SendChar(char c) {
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);  // �ȴ��������
    USART_SendData(USART3, c);
}

void USART3_SendString(char* str) {
    while (*str) {
        USART3_SendChar(*str++);
    }
}

char USART3_ReceiveChar(void) {
    while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);  // �ȴ��������
    return USART_ReceiveData(USART3);
}

void Bluetooth_TestJDY31(void) {
    // ���� AT ָ����� JDY-31
    USART3_SendString("AT\r\n");

    // ��ʱ�ȴ�ģ����Ӧ
    for (volatile int i = 0; i < 1000000; i++);

    // �������� JDY-31 ����Ӧ
    char response = USART3_ReceiveChar();
    if (response == 'O') {
        USART3_SendString("JDY-31 is working!\r\n");
    }
}
