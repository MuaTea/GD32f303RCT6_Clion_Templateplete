//
// Created by muatea on 2024/9/23.
//

#include "usart_drv.h"

//发送字符串函数
void USART_SendString(uint32_t usart_periph, unsigned char *str, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++) {
        usart_data_transmit(usart_periph, str[i]);
        // 这里可以加入等待发送完成的逻辑，确保传输顺利
        while (usart_flag_get(usart_periph, USART_FLAG_TBE) == RESET);  // 等待数据寄存器空闲
    }
}


//接收字符串函数（每次接收一个字符，直到遇到换行符）
void USART_ReceiveString(uint32_t usart_periph,char *buffer)
{
    char received_char;
    for(;;){
        received_char = (char) usart_data_receive(usart_periph);    // 接收一个字符
        if(received_char=='\n'){
                break;
        }
    *buffer++ = received_char;  // 存储接收到的字符
    }
    *buffer = '\0'; // 结束字符串
}

//用于接收一行数据（比如直到接收到换行符 \n 或回车符 \r 为止）
// void USART_ReceiveLine(uint32_t usart_periph, char *buffer, size_t buffer_size) {
//     size_t i = 0;
//     char received_char;
//     uint32_t timeout_ms = 1000;
//     uint32_t start_time = timer_get_ms();  // 假设有一个获取当前时间的函数
//
//     // 确保 buffer 大小有效
//     if (buffer_size == 0) {
//         return;
//     }
//
//
//     // 循环接收数据，直到接收到换行符或回车符，或超时
//     while (i < (buffer_size - 1)) {
//         // 超时检测
//         if ((timer_get_ms() - start_time) > timeout_ms) {
//             break;  // 超时退出
//         }
//
//     // 循环接收数据，直到接收到换行符或达到 buffer 大小限制
//     while (i < (buffer_size - 1)) {  // 留出一个空间用于存储结束符 '\0'
//         // 等待接收到数据（非阻塞方式可选，或使用中断方式）
//         received_char = (char)usart_data_receive(usart_periph);
//
//         // 检查是否是换行符或回车符
//         if (received_char == '\n' || received_char == '\r') {
//             break;
//         }
//
//         // 存储到 buffer 中
//         buffer[i++] = received_char;
//     }
//
//     // 在最后加上结束符
//     buffer[i] = '\0';
//     }
// }

int check_USART_ReceiceLine(uint32_t usart_periph, char *buffer, size_t buffer_size){
    size_t i = 0;
    char received_char;

    // 确保 buffer 大小有效
    if (buffer_size == 0) {
        return 0; // 如果 buffer_size 无效，返回 0
    }

    // 循环接收数据，直到接收到换行符或达到 buffer 大小限制
    while (i < (buffer_size - 1)) { // 留出一个空间用于存储结束符 '\0'

        // 等待接收到数据（可以加入非阻塞方式或中断方式，这里为简单起见假定阻塞式接收）
        while (usart_flag_get(usart_periph, USART_FLAG_RBNE) == RESET) {
            // 等待数据可用，如果需要非阻塞方式，可以加入超时机制
        }

        // 接收到数据
        received_char = (char) usart_data_receive(usart_periph);

        // 检查是否是换行符或回车符
        if (received_char == '\n' || received_char == '\r') {
            break; // 如果接收到换行符或回车符，结束接收
        }

        // 存储到 buffer 中
        buffer[i++] = received_char;
    }

    // 在 buffer 的末尾加上结束符
    buffer[i] = '\0';

    // 返回接收到的字符数
    return (int) i; // 返回实际接收到的字符数量
}

void UsartPrintf(uint32_t usart_periph, const char *format, ...)
{
    char buffer[256];  // 定义缓冲区存储格式化后的字符串
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);  // 将格式化的输出存入缓冲区
    va_end(args);

    // 通过指定的 USART 发送格式化后的字符串
    USART_SendString(usart_periph, (unsigned char *)buffer, strlen(buffer));
}

void USART_SendData(uint32_t usart_base, uint16_t data) {
    // 等待发送数据寄存器空闲
    while (!(USART_STAT0(usart_base) & USART_STAT0_TBE));

    // 写入数据到数据寄存器
    USART_DATA(usart_base) = (data & 0x1FF); // 确保数据为9位以内
}

void USART0_Init(unsigned int baud)
{
    // 配置GPIO和USART外设
    rcu_periph_clock_enable(RCU_GPIOA);  // 使能GPIOA时钟
    rcu_periph_clock_enable(RCU_USART0); // 使能USART0时钟
    rcu_periph_clock_enable(RCU_AF);
    // GPIOA9 - TX (复用推挽输出)
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

    // GPIOA10 - RX (浮空输入)
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    // 配置USART参数
    usart_deinit(USART0);
    usart_baudrate_set(USART0, baud);                              // 设置波特率
    usart_word_length_set(USART0, USART_WL_8BIT);                  // 8位数据
    usart_stop_bit_set(USART0, USART_STB_1BIT);                    // 1位停止位
    usart_parity_config(USART0, USART_PM_NONE);                    // 无校验
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);     // 无RTS流控制
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);     // 无CTS流控制
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);            // 使能接收
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);          // 使能发送
    usart_enable(USART0);                                          // 使能USART0

    // 配置USART中断
    nvic_irq_enable(USART0_IRQn, 0, 2);
    usart_interrupt_enable(USART0, USART_INT_RBNE);                // 使能接收中断
}

void USART1_Init(uint32_t baudval)
{
    // 使能外设时钟
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_USART1);

    // 初始化GPIO: PA2 (USART1_TX) 和 PA3 (USART1_RX)
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_3);

    // USART初始化
    usart_deinit(USART1);

    usart_baudrate_set(USART1, baudval); // 配置波特率
    usart_parity_config(USART1, USART_PM_NONE); // 校验位
    usart_word_length_set(USART1, USART_WL_8BIT); // 数据长度
    usart_stop_bit_set(USART1, USART_STB_1BIT); // 设置停止位
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE); // 设置发送模式
    usart_receive_config(USART1, USART_RECEIVE_ENABLE); // 设置接收模式

    usart_flag_clear(USART1, USART_FLAG_TC); // 清理发送完成标志
    usart_flag_clear(USART1, USART_FLAG_RBNE); // 清理接收非空标志

    nvic_irq_enable(USART1_IRQn, 0, 0);

    // 使能USART2接收中断和空闲中断
    usart_interrupt_enable(USART1, USART_INT_RBNE);  // 使能接收中断

    usart_enable(USART1); // 使能USART

}


void USART2_Init(unsigned int baud)
{
    // 配置GPIO和USART外设
    rcu_periph_clock_enable(RCU_GPIOB);  // 使能GPIOB时钟
    rcu_periph_clock_enable(RCU_USART2); // 使能USART2时钟

    // GPIOB10 - TX (复用推挽输出)
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    // GPIOB11 - RX (浮空输入)
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

    // 配置USART参数
    usart_deinit(USART2);
    usart_baudrate_set(USART2, baud);                              // 设置波特率
    usart_word_length_set(USART2, USART_WL_8BIT);                  // 8位数据
    usart_stop_bit_set(USART2, USART_STB_1BIT);                    // 1位停止位
    usart_parity_config(USART2, USART_PM_NONE);                    // 无校验
    usart_hardware_flow_rts_config(USART2, USART_RTS_DISABLE);     // 无RTS流控制
    usart_hardware_flow_cts_config(USART2, USART_CTS_DISABLE);     // 无CTS流控制
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);            // 使能接收
    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);          // 使能发送
    usart_enable(USART2);                                          // 使能USART2

    // 配置USART中断
    nvic_irq_enable(USART2_IRQn, 0, 0);

    // 使能USART2接收中断和空闲中断
    usart_interrupt_enable(USART2, USART_INT_RBNE);  // 使能接收中断
}
