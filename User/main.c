#include "../StartLibrary/gd32f30x.h"
#include "Delay.h"
#include "gd32f30x_gpio.h"
#include "gd32f30x_rcu.h"


// 假设 LED 连接在 GPIOC Pin 13
#define LED_PIN GPIO_PIN_0
#define LED_PORT GPIOA

// 初始化 LED GPIO 引脚
void led_init(void) {
    // 启用 GPIO 时钟
    rcu_periph_clock_enable(RCU_GPIOA);

    // 设置 GPIOC Pin 13 为推挽输出
    gpio_init(LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_PIN);

    // 默认关闭 LED
    gpio_bit_reset(LED_PORT, LED_PIN);
}

// 点亮 LED
void led_on(void) {
    gpio_bit_set(LED_PORT, LED_PIN);
}

// 熄灭 LED
void led_off(void) {
    gpio_bit_reset(LED_PORT, LED_PIN);
}

int main(void) {
    // 初始化 LED 引脚
    led_init();

    // 主循环
    while (1) {
        led_on();     // 点亮 LED
        Delay_ms(500);  // 延迟 500 毫秒
        led_off();    // 关闭 LED
        Delay_ms(500);  // 延迟 500 毫秒
    }
}
