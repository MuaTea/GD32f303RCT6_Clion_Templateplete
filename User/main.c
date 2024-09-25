#include "systick.h"
#include "PWM_drv.h"
#include "retarget.h"

// 硬件初始化
void HardWare_Init() {
    Systick_Init(120);

    /* PWM_Init: 2kHz */
    PWM_Init(50-1, 1200-1);  // 0.5ms, PWM: 2KHz

    //printf重定向
    printf_retarget(115200, USART0);
}

// 主函数
int main() {
    u8 fx = 1;  // 用于控制亮度的增减方向
    u16 n = 0;  // 用于记录当前 PWM 信号的占空比
    HardWare_Init();

    for (;;) {

        printf("Hello World ^_^'\r\f");

        // 调整亮度方向
        if (fx) {
            n++;  // 增加占空比
            if (n > 50 - 1) {  // 达到最大占空比时改变方向
                fx = 0;
            }
        } else {
            n--;  // 减少占空比
            if (n < 1) {  // 达到最小占空比时改变方向
                fx = 1;
            }
        }
        // 设置占空比
        timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, n);

        // 调整延迟时间，控制呼吸灯速度
        delay_ms(20);
    }
}
