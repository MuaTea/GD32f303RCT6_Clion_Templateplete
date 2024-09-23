#include "../StartLibrary/gd32f30x.h"
#include "Delay.h"
#include "usart_drv.h"
#include "GPS_drv.h"

int main(void) {
    // 初始化 LED 引脚
    USART_Init();   // 初始化 USART
    connectToOneNet(); // 连接到 OneNet 云

    // 主循环
    for (;;) {
        const char* jsonData = "{\\\"datastreams\\\":[{\\\"id\\\":\\\"temperature\\\",\\\"value\\\":25.5}]}";// 示例 JSON 数据
        sendDataToOneNet(jsonData);// 发送数据到 OneNet
        Delay_us(10000);// 根据需要延时
    }
}
