#include "relay_drv.h"
#include "system.h"

//继电器初始化
void Relay_Init(void)
{
	//GPIO初始化
	GPIO_InitTypeDef GPIO_InitStructure;

    // 启用 GPIOA 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置 GPIOA 引脚 0 为推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//控制继电器
void Relay_Control(u8 state)
{
    if (state) {
        // 继电器开
        //GPIO_SetBits(GPIOA, GPIO_Pin_4);
		PAin(4) = 1;
    } else {
        // 继电器关
        //GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		PAin(4) = 0;
    }
}


