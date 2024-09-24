#include "jdy.h"

void USART3_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStruct;

    // 1. 启用 GPIOB 和 USART3 的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    // 2. 配置 USART3 TX (PB10) 和 RX (PB11)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  // TX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // 复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  // RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  // 浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 3. 配置 USART3
    USART_InitStructure.USART_BaudRate = 9600;  // JDY-31 默认波特率为 9600
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);

    // 4. 启用 USART3
    USART_Cmd(USART3, ENABLE);

    // 5. 配置 NVIC 中断
    NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
}

void USART3_SendChar(char c) {
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);  // 等待发送完成
    USART_SendData(USART3, c);
}

void USART3_SendString(char* str) {
    while (*str) {
        USART3_SendChar(*str++);
    }
}

char USART3_ReceiveChar(void) {
    while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);  // 等待接收完成
    return USART_ReceiveData(USART3);
}

void Bluetooth_TestJDY31(void) {
    // 发送 AT 指令，测试 JDY-31
    USART3_SendString("AT\r\n");

    // 延时等待模块响应
    for (volatile int i = 0; i < 1000000; i++);

    // 接收来自 JDY-31 的响应
    char response = USART3_ReceiveChar();
    if (response == 'O') {
        USART3_SendString("JDY-31 is working!\r\n");
    }
}
