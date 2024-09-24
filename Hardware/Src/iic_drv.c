#include "iic_drv.h"
#include "systick.h"
#include "stdio.h"

float CO2;

void CO2_IIC_Init(void) {
    // 使能GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // GPIO初始化
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = SCL_PIN | SDA_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStruct);
}

void SDA_OutputMode_CO2(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = SDA_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStruct);
}

void SDA_InputMode_CO2(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = SDA_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStruct);
}

// 设置 SDA 引脚为高电平
void SDA_High_CO2(void) {
    GPIO_SetBits(I2C_GPIO_PORT, SDA_PIN);
}

// 设置 SDA 引脚为低电平
void SDA_Low_CO2(void) {
    GPIO_ResetBits(I2C_GPIO_PORT, SDA_PIN);
}

// 设置 SCL 引脚为高电平
void SCL_High_CO2(void) {
    GPIO_SetBits(I2C_GPIO_PORT, SCL_PIN);
}

// 设置 SCL 引脚为低电平
void SCL_Low_CO2(void) {
    GPIO_ResetBits(I2C_GPIO_PORT, SCL_PIN);
}

// 读取 SDA 的状态
uint8_t SDA_Read_CO2(void) {
    return GPIO_ReadInputDataBit(I2C_GPIO_PORT, SDA_PIN);
}

// 起始信号
void CO2_I2C_Start(void) {
    SDA_OutputMode_CO2();
    SCL_High_CO2();
    delay_us(1);  // 确保时序满足要求
    SDA_Low_CO2();
    delay_us(1);
    SCL_Low_CO2();
}

// 停止信号
void CO2_I2C_Stop(void) {
    SDA_OutputMode_CO2();
    SCL_Low_CO2();
    SDA_Low_CO2();
    delay_us(1);
    SCL_High_CO2();
    delay_us(1);
    SDA_High_CO2();
}

// 发送字节
void CO2_I2C_SendByte(uint8_t byte) {
    SDA_OutputMode_CO2();
    for (int i = 0; i < 8; i++) {
        if (byte & 0x80) {
            SDA_High_CO2();
        } else {
            SDA_Low_CO2();
        }
        byte <<= 1;
        delay_us(1);
        SCL_High_CO2();
        delay_us(3);  // 延时，确保从设备采样SDA上的数据
        SCL_Low_CO2();
        delay_us(1);
    }
}

// 等待应答信号
uint8_t CO2_I2C_WaitAck(void) {
    SDA_InputMode_CO2();
    SCL_High_CO2();
    delay_us(1);
    uint8_t ack = SDA_Read_CO2();
    SCL_Low_CO2();
    return ack == 0 ? 1 : 0;
}

// 读字节数据
uint8_t CO2_I2C_ReadByte(uint8_t ack) {
    uint8_t byte = 0;
    SDA_InputMode_CO2();
    for (int i = 0; i < 8; i++) {
        SCL_Low_CO2();
        delay_us(1);
        SCL_High_CO2();
        byte <<= 1;
        if (SDA_Read_CO2()) {
            byte |= 0x01;
        }
        delay_us(1);
    }
    SCL_Low_CO2();
    if (ack) {
        CO2_I2C_SendAck();
    } else {
        CO2_I2C_SendNack();
    }
    return byte;
}

// 发送ACK
void CO2_I2C_SendAck(void) {
    SDA_OutputMode_CO2();
    SDA_Low_CO2();
    delay_us(1);
    SCL_High_CO2();
    delay_us(1);
    SCL_Low_CO2();
}

// 发送NACK
void CO2_I2C_SendNack(void) {
    SDA_OutputMode_CO2();
    SDA_High_CO2();
    delay_us(1);
    SCL_High_CO2();
    delay_us(1);
    SCL_Low_CO2();
}

// CRC校验
uint8_t calculateCRC(uint8_t* data, int length) {
    uint8_t crc = 0xFF;
    for (int i = 0; i < length; i++) {
        crc ^= data[i];
        for (int bit = 0; bit < 8; bit++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x31;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

// 读取SGP30数据
void SGP30_ReadData(void) {
    uint8_t buffer[6];
    CO2_I2C_Start();
    CO2_I2C_SendByte(SGP30_I2C_ADDRESS << 1);
    if (!CO2_I2C_WaitAck()) {
        printf("Address transmission failed!\n");
        CO2_I2C_Stop();
        return;
    }

    CO2_I2C_SendByte(SGP30_MEASURE_COMMAND >> 8);
    if (!CO2_I2C_WaitAck()) {
        printf("Measurement command high byte failed!\n");
        CO2_I2C_Stop();
        return;
    }

    CO2_I2C_SendByte(SGP30_MEASURE_COMMAND & 0xFF);
    if (!CO2_I2C_WaitAck()) {
        printf("Measurement command low byte failed!\n");
        CO2_I2C_Stop();
        return;
    }

    CO2_I2C_Stop();
    delay_ms(50);

    CO2_I2C_Start();
    CO2_I2C_SendByte((SGP30_I2C_ADDRESS << 1) | 0x01);
    if (!CO2_I2C_WaitAck()) {
        printf("Failed to send read request!\n");
        CO2_I2C_Stop();
        return;
    }

    for (int i = 0; i < 6; i++) {
        buffer[i] = CO2_I2C_ReadByte(i < 5 ? 1 : 0);
    }

    CO2_I2C_Stop();

    if (calculateCRC(buffer, 2) != buffer[2] || calculateCRC(buffer + 3, 2) != buffer[5]) {
        printf("CRC check failed!\n");
        return;
    }

    uint16_t co2 = (buffer[0] << 8) | buffer[1];
    uint16_t voc = (buffer[3] << 8) | buffer[4];

    CO2 = co2;
    printf("CO2: %d ppm, VOC: %d ppb\n", co2, voc);
}

// 初始化SGP30
void SGP30_Init(void) {
    CO2_I2C_Start();
    CO2_I2C_SendByte(SGP30_I2C_ADDRESS << 1);
    if (!CO2_I2C_WaitAck()) {
        printf("Address transmission failed!\n");
        CO2_I2C_Stop();
        return;
    }

    CO2_I2C_SendByte(0x20);  // Init command high byte
    CO2_I2C_WaitAck();
    CO2_I2C_SendByte(0x03);  // Init command low byte
    CO2_I2C_WaitAck();
    CO2_I2C_Stop();
    delay_ms(2);
}
















































//#include "iic_drv.h"
//#include "usart_drv.h"
//#include "systick.h"
//
//
//// 引脚初始化
//void CO2_Init(void) {
//    GPIO_InitTypeDef GPIO_InitStructure;
//
//    // 启用 GPIO 时钟
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   // 启用AFIO时钟，用于重映射
//
//    // 配置 SCL (PB6) 和 SDA (PB7) 为开漏输出
//    GPIO_InitStructure.GPIO_Pin = SCL_PIN | SDA_PIN;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  // 开漏输出
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStructure);
//
//	// 重映射I2C1到PB8和PB9
//    //GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
//    // 设置 SCL 和 SDA 高电平
//    GPIO_SetBits(I2C_GPIO_PORT, SCL_PIN | SDA_PIN);
//}
//
//// 设置 SDA 引脚为高电平
//void SDA_High_CO2(void) {
//    GPIO_SetBits(I2C_GPIO_PORT,  SDA_PIN);
//}
//
//// 设置 SDA 引脚为低电平
//void SDA_Low_CO2(void) {
//    GPIO_ResetBits(I2C_GPIO_PORT, SDA_PIN);
//}
//
//// 设置 SCL 引脚为高电平
//void SCL_High_CO2(void) {
//    GPIO_SetBits(I2C_GPIO_PORT, SCL_PIN);
//}
//
//// 设置 SCL 引脚为低电平
//void SCL_Low_CO2(void) {
//    GPIO_ResetBits(I2C_GPIO_PORT, SCL_PIN);
//}
//// 读取 SDA 的状态
//uint8_t SDA_Read_CO2(void) {
//    return GPIO_ReadInputDataBit(I2C_GPIO_PORT, SDA_PIN);
//}
//
//void CO2_I2C_Start(void) {
//    SDA_High_CO2();
//    SCL_High_CO2();
//    delay_us(1);  // 确保时序满足要求
//    SDA_Low_CO2();
//    delay_us(1);
//    SCL_Low_CO2();
//}
//
//void CO2_I2C_Stop(void) {
//    SCL_Low_CO2();
//    SDA_Low_CO2();
//    delay_us(1);
//    SCL_High_CO2();
//    delay_us(1);
//    SDA_High_CO2();
//}
//
//void CO2_I2C_SendByte(uint8_t byte) {
//    for (int i = 0; i < 8; i++) {
//        if (byte & 0x80) {
//            SDA_High_CO2();
//        } else {
//            SDA_Low_CO2();
//        }
//        byte <<= 1;
//        delay_us(1);
//        SCL_High_CO2();  // 拉高 SCL，传输数据
//        delay_us(1);
//        SCL_Low_CO2();
//        delay_us(2);
//    }
//}
//
//
//uint8_t CO2_I2C_WaitAck(void) {
//    SDA_High_CO2();  // 释放 SDA
//    delay_us(1);
//    SCL_High_CO2();  // 拉高 SCL 等待从设备的 ACK
//    delay_us(1);
//
//    uint8_t ack = SDA_Read_CO2();  // 读取 SDA 线的状态
//
//    SCL_Low_CO2();  // 拉低 SCL，结束等待
//    return ack == 0 ? 1 : 0;  // 返回 1 表示收到 ACK
//}
//
//uint8_t CO2_I2C_ReadByte(void) {
//    uint8_t byte = 0;
//    SDA_High_CO2();  // 释放 SDA，准备读取
//
//    for (int i = 0; i < 8; i++) {
//        SCL_Low_CO2();
//        delay_us(1);
//        SCL_High_CO2();
//        byte <<= 1;
//        if (SDA_Read_CO2()) {
//            byte |= 0x01;
//        }
//        delay_us(1);
//    }
//    SCL_Low_CO2();
//    return byte;
//}
//
//void CO2_I2C_SendAck(void) {
//    SDA_Low_CO2();
//    delay_us(1);
//    SCL_High_CO2();
//    delay_us(1);
//    SCL_Low_CO2();
//}
//
//void CO2_I2C_SendNack(void) {
//    SDA_High_CO2();
//    delay_us(1);
//    SCL_High_CO2();
//    delay_us(1);
//    SCL_Low_CO2();
//}
//
//// CRC校验函数，使用8-bit多项式0x31
//uint8_t calculateCRC(uint8_t* data, int length) {
//    uint8_t crc = 0xFF;  // 初始值
//    for (int i = 0; i < length; i++) {
//        crc ^= data[i];
//        for (int bit = 0; bit < 8; bit++) {
//            if (crc & 0x80) {
//                crc = (crc << 1) ^ 0x31;
//            } else {
//                crc <<= 1;
//            }
//        }
//    }
//    return crc;
//}
//
//void SGP30_ReadData(void) {
//    uint8_t buffer[6];
//
//    // 启动 I2C 通信
//    CO2_I2C_Start();
//    CO2_I2C_SendByte(SGP30_I2C_ADDRESS << 1);  // 发送 I2C 地址 + 写
//    if (!CO2_I2C_WaitAck()) {
//        printf("Address transmission failed!\n");
//        CO2_I2C_Stop();
//        return;
//    }
//
//    // 发送读取数据的命令 (例如发送测量命令)
//    CO2_I2C_SendByte(SGP30_MEASURE_COMMAND >> 8);  // 发送高字节
//    CO2_I2C_WaitAck();
//    CO2_I2C_SendByte(SGP30_MEASURE_COMMAND & 0xFF);  // 发送低字节
//    CO2_I2C_WaitAck();
//    CO2_I2C_Stop();
//
//    // 延时等待测量完成
//    delay_ms(50);
//
//    // 开始 I2C 读取数据
//    CO2_I2C_Start();
//    CO2_I2C_SendByte((SGP30_I2C_ADDRESS << 1) | 0x01);  // 发送 I2C 地址 + 读
//    if (!CO2_I2C_WaitAck()) {
//        printf("Failed to send read request!\n");
//        CO2_I2C_Stop();
//        return;
//    }
//
//    // 读取 6 字节数据 (2 个 16 位数据 + CRC 校验)
//    for (int i = 0; i < 6; i++) {
//        buffer[i] = CO2_I2C_ReadByte();
//        if (i < 5) {
//            CO2_I2C_SendAck();  // 发送 ACK
//        } else {
//            CO2_I2C_SendNack();  // 最后一个字节发送 NACK
//        }
//    }
//    CO2_I2C_Stop();
//
//    // 校验CRC，如果不匹配，丢弃数据
//    if (calculateCRC(buffer, 2) != buffer[2] || calculateCRC(buffer + 3, 2) != buffer[5]) {
//        printf("CRC check failed!\n");
//        return;
//    }
//
//    // 处理数据，例如解析 CO2、VOC 值等
//    uint16_t co2 = (buffer[0] << 8) | buffer[1];
//    uint16_t voc = (buffer[3] << 8) | buffer[4];
//
//    printf("CO2: %d ppm, VOC: %d ppb\n", co2, voc);
//}
//
//
//void SGP30_Init(void) {
//    // 启动 I2C 通信并发送 Init_air_quality 命令
//    CO2_I2C_Start();
//    CO2_I2C_SendByte(SGP30_I2C_ADDRESS << 1);  // 发送 I2C 地址 + 写
//    if (!CO2_I2C_WaitAck()) {
//        printf("Address transmission failed!\n");
//        CO2_I2C_Stop();
//        return;
//    }
//
//    // 发送 Init_air_quality 命令 (0x2003)
//    CO2_I2C_SendByte(0x20);  // 发送高字节
//    CO2_I2C_WaitAck();
//    CO2_I2C_SendByte(0x03);  // 发送低字节
//    CO2_I2C_WaitAck();
//    CO2_I2C_Stop();
//
//    // 延时2ms，等待初始化完成
//    delay_ms(2);
//}
