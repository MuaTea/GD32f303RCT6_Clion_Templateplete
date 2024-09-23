//
// Created by muatea on 2024/9/23.
//

#include "usart_drv.h"

//发送字符串函数
void USART_SendString(uint32_t usart_periph,char *str)
{
    while (*str){
        usart_data_transmit(usart_periph, *str++);
        // 你可能还需要加入等待发送完成的逻辑
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
void USART_ReceiveLine(uint32_t usart_periph, char *buffer, size_t buffer_size) {
    size_t i = 0;
    char received_char;
    uint32_t start_time = timer_get_ms();  // 假设有一个获取当前时间的函数

    // 确保 buffer 大小有效
    if (buffer_size == 0) {
        return;
    }


    // 循环接收数据，直到接收到换行符或回车符，或超时
    while (i < (buffer_size - 1)) {
        // 超时检测
        if ((timer_get_ms() - start_time) > timeout_ms) {
            break;  // 超时退出
        }

    // 循环接收数据，直到接收到换行符或达到 buffer 大小限制
    while (i < (buffer_size - 1)) {  // 留出一个空间用于存储结束符 '\0'
        // 等待接收到数据（非阻塞方式可选，或使用中断方式）
        received_char = (char)usart_data_receive(usart_periph);

        // 检查是否是换行符或回车符
        if (received_char == '\n' || received_char == '\r') {
            break;
        }

        // 存储到 buffer 中
        buffer[i++] = received_char;
    }

    // 在最后加上结束符
    buffer[i] = '\0';
}

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

void USART_Init(void) {
    /* 定义结构体用于配置 GPIO 和 USART */
    gpio_init_type GPIO_InitStructure;
    usart_parameter_struct USART_InitStructure;

    /* 1. 使能 GPIOA 和 USART0 的时钟 (USART0 对应的是 PA9 和 PA10) */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);

    /* 2. 配置 PA9 (USART0 TX) 为复用推挽输出 */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

    /* 3. 配置 PA10 (USART0 RX) 为浮空输入 */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* 4. 配置 USART0 的参数 */
    usart_deinit(USART0); // 复位 USART0

    /* 配置 USART0 波特率为 115200 */
    USART_InitStructure.baudrate = 115200;
    /* 配置数据位长度为 8 */
    USART_InitStructure.word_length = USART_WL_8BIT;
    /* 配置停止位为 1 */
    USART_InitStructure.stop_bits = USART_STB_1BIT;
    /* 配置无奇偶校验 */
    USART_InitStructure.parity = USART_PM_NONE;
    /* 禁用 RTS 和 CTS 硬件流控制 */
    USART_InitStructure.hardware_flow_rts = USART_RTS_DISABLE;
    USART_InitStructure.hardware_flow_cts = USART_CTS_DISABLE;
    /* 使能接收和发送模式 */
    USART_InitStructure.receive_config = USART_RECEIVE_ENABLE;
    USART_InitStructure.transmit_config = USART_TRANSMIT_ENABLE;

    /* 初始化 USART0 */
    usart_init(USART0, &USART_InitStructure);

    /* 5. 使能 USART0 */
    usart_enable(USART0);
}