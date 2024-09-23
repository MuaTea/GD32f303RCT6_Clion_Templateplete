//
// Created by muatea on 2024/9/23.
//

#include "usart_drv.h"

//�����ַ�������
void USART_SendString(uint32_t usart_periph,char *str)
{
    while (*str){
        usart_data_transmit(usart_periph, *str++);
        // ����ܻ���Ҫ����ȴ�������ɵ��߼�
    }
}

//�����ַ���������ÿ�ν���һ���ַ���ֱ���������з���
void USART_ReceiveString(uint32_t usart_periph,char *buffer)
{
    char received_char;
    for(;;){
        received_char = (char) usart_data_receive(usart_periph);    // ����һ���ַ�
        if(received_char=='\n'){
                break;
        }
    *buffer++ = received_char;  // �洢���յ����ַ�
    }
    *buffer = '\0'; // �����ַ���
}

//���ڽ���һ�����ݣ�����ֱ�����յ����з� \n ��س��� \r Ϊֹ��
void USART_ReceiveLine(uint32_t usart_periph, char *buffer, size_t buffer_size) {
    size_t i = 0;
    char received_char;
    uint32_t start_time = timer_get_ms();  // ������һ����ȡ��ǰʱ��ĺ���

    // ȷ�� buffer ��С��Ч
    if (buffer_size == 0) {
        return;
    }


    // ѭ���������ݣ�ֱ�����յ����з���س�������ʱ
    while (i < (buffer_size - 1)) {
        // ��ʱ���
        if ((timer_get_ms() - start_time) > timeout_ms) {
            break;  // ��ʱ�˳�
        }

    // ѭ���������ݣ�ֱ�����յ����з���ﵽ buffer ��С����
    while (i < (buffer_size - 1)) {  // ����һ���ռ����ڴ洢������ '\0'
        // �ȴ����յ����ݣ���������ʽ��ѡ����ʹ���жϷ�ʽ��
        received_char = (char)usart_data_receive(usart_periph);

        // ����Ƿ��ǻ��з���س���
        if (received_char == '\n' || received_char == '\r') {
            break;
        }

        // �洢�� buffer ��
        buffer[i++] = received_char;
    }

    // �������Ͻ�����
    buffer[i] = '\0';
}

int check_USART_ReceiceLine(uint32_t usart_periph, char *buffer, size_t buffer_size){
    size_t i = 0;
    char received_char;

    // ȷ�� buffer ��С��Ч
    if (buffer_size == 0) {
        return 0; // ��� buffer_size ��Ч������ 0
    }

    // ѭ���������ݣ�ֱ�����յ����з���ﵽ buffer ��С����
    while (i < (buffer_size - 1)) { // ����һ���ռ����ڴ洢������ '\0'

        // �ȴ����յ����ݣ����Լ����������ʽ���жϷ�ʽ������Ϊ������ٶ�����ʽ���գ�
        while (usart_flag_get(usart_periph, USART_FLAG_RBNE) == RESET) {
            // �ȴ����ݿ��ã������Ҫ��������ʽ�����Լ��볬ʱ����
        }

        // ���յ�����
        received_char = (char) usart_data_receive(usart_periph);

        // ����Ƿ��ǻ��з���س���
        if (received_char == '\n' || received_char == '\r') {
            break; // ������յ����з���س�������������
        }

        // �洢�� buffer ��
        buffer[i++] = received_char;
    }

    // �� buffer ��ĩβ���Ͻ�����
    buffer[i] = '\0';

    // ���ؽ��յ����ַ���
    return (int) i; // ����ʵ�ʽ��յ����ַ�����
}

void USART_Init(void) {
    /* ����ṹ���������� GPIO �� USART */
    gpio_init_type GPIO_InitStructure;
    usart_parameter_struct USART_InitStructure;

    /* 1. ʹ�� GPIOA �� USART0 ��ʱ�� (USART0 ��Ӧ���� PA9 �� PA10) */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);

    /* 2. ���� PA9 (USART0 TX) Ϊ����������� */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

    /* 3. ���� PA10 (USART0 RX) Ϊ�������� */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* 4. ���� USART0 �Ĳ��� */
    usart_deinit(USART0); // ��λ USART0

    /* ���� USART0 ������Ϊ 115200 */
    USART_InitStructure.baudrate = 115200;
    /* ��������λ����Ϊ 8 */
    USART_InitStructure.word_length = USART_WL_8BIT;
    /* ����ֹͣλΪ 1 */
    USART_InitStructure.stop_bits = USART_STB_1BIT;
    /* ��������żУ�� */
    USART_InitStructure.parity = USART_PM_NONE;
    /* ���� RTS �� CTS Ӳ�������� */
    USART_InitStructure.hardware_flow_rts = USART_RTS_DISABLE;
    USART_InitStructure.hardware_flow_cts = USART_CTS_DISABLE;
    /* ʹ�ܽ��պͷ���ģʽ */
    USART_InitStructure.receive_config = USART_RECEIVE_ENABLE;
    USART_InitStructure.transmit_config = USART_TRANSMIT_ENABLE;

    /* ��ʼ�� USART0 */
    usart_init(USART0, &USART_InitStructure);

    /* 5. ʹ�� USART0 */
    usart_enable(USART0);
}