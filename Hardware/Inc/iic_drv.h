#ifndef __IIC_DRV_H
#define __IIC_DRV_H

#include "stm32f10x.h"

// 定义 SDA 和 SCL 引脚
#define SCL_PIN GPIO_Pin_11  // SCL 对应的 GPIO 引脚
#define SDA_PIN GPIO_Pin_12  // SDA 对应的 GPIO 引脚
#define I2C_GPIO_PORT GPIOC  // SCL 和 SDA 所在的 GPIO 端口

// 定义 I2C 设备地址 (例如 SGP30 的 I2C 地址)
#define SGP30_I2C_ADDRESS 0x58    // SGP30 I2C 地址（7位地址）

// 定义 SGP30 的测量命令 (假设测量命令为 0x2008)
#define SGP30_MEASURE_COMMAND 0x2008

// 其他可能的定义
#define I2C_ACK  0x00  // ACK 信号
#define I2C_NACK 0x01  // NACK 信号

#define A_CONST 512
#define SREF_H2 1000  // 假设的参考信号值，根据实际情况调整
#define SREF_ETHOH 1000  // 假设的参考信号值，根据实际情况调整

// 函数声明
void CO2_IIC_Init(void);
void SDA_InputMode_CO2(void);
void SDA_OutputMode_CO2(void);
void SDA_High_CO2(void);
void SDA_Low_CO2(void);
void SCL_High_CO2(void);
void SCL_Low_CO2(void);
uint8_t SDA_Read_CO2(void);
void CO2_I2C_Start(void);
void CO2_I2C_Stop(void);
void CO2_I2C_SendByte(uint8_t byte);
uint8_t CO2_I2C_WaitAck(void);
uint8_t CO2_I2C_ReadByte(uint8_t ack);
void CO2_I2C_SendAck(void);
void CO2_I2C_SendNack(void);
void SGP30_ReadData(void);
void SGP30_Init(void);

#endif  // __IIC_DRV_H




































//#ifndef __IIC_DRV_H
//#define __IIC_DRV_H
//
//#include "stm32f10x.h"
//
//// 定义 SDA 和 SCL 引脚
//#define SCL_PIN GPIO_Pin_11  // SCL 对应的 GPIO 引脚
//#define SDA_PIN GPIO_Pin_12  // SDA 对应的 GPIO 引脚
//#define I2C_GPIO_PORT GPIOC  // SCL 和 SDA 所在的 GPIO 端口
//
//// 定义 I2C 设备地址 (例如 SGP30 的 I2C 地址)
//#define SGP30_I2C_ADDRESS 0x58    // SGP30 I2C 地址（7位地址）
//
//// 定义 SGP30 的测量命令 (假设测量命令为 0x2008)
//#define SGP30_MEASURE_COMMAND 0x2008
//
//// 其他可能的定义
//#define I2C_ACK  0x00  // ACK 信号
//#define I2C_NACK 0x01  // NACK 信号
//
//#define A_CONST 512
//#define SREF_H2 1000  // 假设的参考信号值，根据实际情况调整
//#define SREF_ETHOH 1000  // 假设的参考信号值，根据实际情况调整
//
//
//void CO2_Init(void);
//void SDA_High_CO2(void);
//void SDA_Low_CO2(void);
//void SCL_High_CO2(void);
//void SCL_Low_CO2(void);
//uint8_t SDA_Read_CO2(void);
//void CO2_I2C_Start(void);
//void CO2_I2C_Stop(void);
//void CO2_I2C_SendByte(uint8_t byte);
//uint8_t CO2_I2C_WaitAck(void);
//uint8_t CO2_I2C_ReadByte(void);
//void CO2_I2C_SendAck(void);
//void CO2_I2C_SendNack(void);
//void SGP30_ReadData(void);
//void SGP30_Init(void);
//
//
//#endif
