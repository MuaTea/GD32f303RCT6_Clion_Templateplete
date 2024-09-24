#include "state_drv.h"
#include "system.h"


u16 breath_timer = 0;//呼吸灯
u16 alarm_timer = 0;//切换状态时间
u16 blink_timer = 0;//闪烁
u8 led_state = 0;
u8 alarm_active = 0; // 全局报警激活状态变量
u8 state = STATE_0; // 0: 快速闪烁, 1: 呼吸灯

//指示灯快闪与呼吸灯之间切换
void LED_Control(u16 arr)
{
    // 根据报警状态决定 LED 行为模式
    if (alarm_active) // 如果报警功能被激活
    {
    	//static u8 state = STATE_0;// 0: 快速闪烁, 1: 呼吸灯
        // 根据当前模式来控制 LED
        switch (state)
        {
            case STATE_0: // 快速闪烁模式
                if (blink_timer >= 500) // 切换频率为 500ms
                {
                    blink_timer = 0;
                    led_state = (led_state == 0) ? 1 : 0; // 切换 LED 状态
                    TIM_SetCompare1(TIM1, (led_state == 0) ? 0 : arr / 2);
                }
                if (alarm_timer >= 5000) // 5 秒后转换为呼吸灯模式
                {                  
                    alarm_timer = 0; // 重置报警计时器
                    breath_timer = 0; // 重置呼吸灯计时器
                    state = STATE_1; // 转换为呼吸灯模式
                }
                break;

            case STATE_1: // 呼吸灯模式

                if (breath_timer >= 20) // 调整呼吸灯的速度
                {
                	alarm_timer = 0; // 重置报警计时器
                    breath_timer = 0;
                    static u16 pulse_width = 0;// 定义一个静态变量用于存储脉冲宽度，保持其值在函数调用之间
                    static s16 direction = 1;// 定义一个静态变量用于控制脉冲宽度的增加或减少方向，初始为增加
                    pulse_width += direction;// 根据当前方向调整脉冲宽度

                    if (pulse_width >= 50) direction = -1;// 如果脉冲宽度达到最大值，则改变方向为减少
                    if (pulse_width <= 0) direction = 1;// 如果脉冲宽度达到最小值，则改变方向为增加

                   	TIM_SetCompare1(TIM1, pulse_width);// 更新定时器 TIM1 的比较值，以调整呼吸灯的亮度或脉冲宽度
                }
                break;

            default:
                // 不同状态或错误处理
                // ...
                break;
        }
    }
    else // 普通模式
    {
        // 处理普通模式下的 LED 行为
        // ...
    }
}

//判断是否报警
void AlarmHandler(u16 value)
{

	//static u8 state = STATE_0;// 0: 快速闪烁, 1: 呼吸灯
    if (value != 0)
    {
        alarm_active = 1; // 启动报警模式
		state = STATE_0;// 调用LED控制函数
    }
    else
    {
        alarm_active = 0; // 关闭报警模式
        alarm_timer = 0; // 重置报警计时器
        TIM_SetCompare1(TIM1, 0);// 处理关闭报警模式的逻辑
        // 在普通模式中 LED 行为的处理
    }
}


//指示灯快闪与呼吸灯之间切换
void LED_Control1(u16 arr)
{
    // 根据报警状态决定 LED 行为模式
    if (alarm_active) // 如果报警功能被激活
    {
    	static u8 state1 = STATE1_0;// 0: 快速闪烁, 1: 呼吸灯
        // 根据当前模式来控制 LED
        switch (state1)
        {
            case STATE1_0: // 快速闪烁模式
                if (blink_timer >= 500) // 切换频率为 500ms
                {	
                	
                    blink_timer = 0;
                    led_state = (led_state == 0) ? 1 : 0; // 切换 LED 状态
                    TIM_SetCompare1(TIM1, (led_state == 0) ? 0 : arr / 2);
					alarm_timer = 0; // 重置报警计时器
                }
				
                if (alarm_timer >= 5000) // 5 秒后转换为呼吸灯模式
                {                  
                    alarm_timer = 0; // 重置报警计时器
                    breath_timer = 0; // 重置呼吸灯计时器
                    state = STATE_1; // 转换为呼吸灯模式
                }
                break;

            case STATE1_1: // 呼吸灯模式

                if (breath_timer >= 20) // 调整呼吸灯的速度
                {
                	alarm_timer = 0; // 重置报警计时器
                    breath_timer = 0;
                    static u16 pulse_width = 0;
                    static s16 direction = 1;
                    pulse_width += direction;

                    if (pulse_width >= 50) direction = -1;
                    if (pulse_width <= 0) direction = 1;

                   	TIM_SetCompare1(TIM1, pulse_width);
                }
                break;

            default:
                // 不同状态或错误处理
                // ...
                break;
        }
    }
    else // 普通模式
    {
        // 处理普通模式下的 LED 行为
        // ...
    }
}

//判断是否报警
void AlarmHandler1(u16 value)
{

	//static u8 state1 = STATE1_0;// 0: 快速闪烁, 1: 呼吸灯
    if (value != 0)
    {
        alarm_active = 1; // 启动报警模式
		LED_Control1(720);// 调用LED控制函数
    }
    else
    {
        alarm_active = 0; // 关闭报警模式
        alarm_timer = 0; // 重置报警计时器
        TIM_SetCompare1(TIM1, 0);// 处理关闭报警模式的逻辑
        // 在普通模式中 LED 行为的处理
    }
}

