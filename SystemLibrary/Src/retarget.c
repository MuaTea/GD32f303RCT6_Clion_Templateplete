#include "retarget.h"

#ifdef GCC_PRINTF_RETARGET

#if !defined(OS_USE_SEMIHOSTING)

#define STDIN_FILE_NO 0
#define STDOUT_FILE_NO 1
#define STDERR_FILE_NO 2

uint32_t G_uart_base;  // 修改为存储USART基地址

void Printf_Init(uint32_t usart_base) {
    G_uart_base = usart_base;
    /* Disable I/O buffering for STDOUT stream, so that
     * chars are sent out as soon as they are printed. */
    setvbuf(stdout, NULL, _IONBF, 0);
}

int _isatty(int fd) {
    if (fd >= STDIN_FILE_NO && fd <= STDERR_FILE_NO)
        return 1;
    return 0;
}

int _write(int fd, char *ptr, int len) {
    if (fd == STDOUT_FILE_NO || fd == STDERR_FILE_NO) {
        for (uint32_t i = 0; i < len; i++) {
            // 等待发送缓冲区为空
            while (!(USART_STAT0(G_uart_base) & USART_STAT0_TBE));
            // 发送数据
            USART_DATA(G_uart_base) = ptr[i];
        }
        // 等待传输完成
        while (!(USART_STAT0(G_uart_base) & USART_STAT0_TC));

        return len;
    }
    return -1;
}

int _close(int fd) {
    if (fd >= STDIN_FILE_NO && fd <= STDERR_FILE_NO)
        return 0;
    return -1;
}

int _lseek(int fd, int ptr, int dir) {
    (void) fd;
    (void) ptr;
    (void) dir;
    return -1;
}

int _read(int fd, char *ptr, int len) {
    if (fd == STDIN_FILE_NO) {
        // 等待接收数据
        while (!(USART_STAT0(USART2) & USART_STAT0_RBNE));
        *ptr = USART_DATA(USART2);
        return 1;
    }
    return -1;
}

#endif //#if !defined(OS_USE_SEMIHOSTING)

#elif ARMCC_PRINTF_RETARGET

// ...ARMCC 重定向的代码部分

#endif

void printf_retarget(uint32_t bound, uint32_t usart_base) {
    /* 这里我们使用GD32的初始化函数进行串口配置 */

    // 使能GPIO和USART时钟，具体根据芯片的引脚配置
    rcu_periph_clock_enable(RCU_GPIOA);  // 假设使用USART0（PA9为TX引脚）
    rcu_periph_clock_enable(RCU_USART0);

    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);  // 配置TX引脚
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10); // 配置RX引脚

    // 配置USART波特率、数据位、停止位等
    usart_deinit(usart_base);
    usart_baudrate_set(usart_base, bound);
    usart_word_length_set(usart_base, USART_WL_8BIT);
    usart_stop_bit_set(usart_base, USART_STB_1BIT);
    usart_parity_config(usart_base, USART_PM_NONE);
    usart_hardware_flow_rts_config(usart_base, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(usart_base, USART_CTS_DISABLE);
    usart_receive_config(usart_base, USART_RECEIVE_ENABLE);
    usart_transmit_config(usart_base, USART_TRANSMIT_ENABLE);
    usart_enable(usart_base);

    // 将 printf 重定向到指定的USART
    Printf_Init(usart_base);
}
