#ifndef __IIC_DRV_H
#define __IIC_DRV_H

#include "stm32f10x.h"

// ���� SDA �� SCL ����
#define SCL_PIN GPIO_Pin_11  // SCL ��Ӧ�� GPIO ����
#define SDA_PIN GPIO_Pin_12  // SDA ��Ӧ�� GPIO ����
#define I2C_GPIO_PORT GPIOC  // SCL �� SDA ���ڵ� GPIO �˿�

// ���� I2C �豸��ַ (���� SGP30 �� I2C ��ַ)
#define SGP30_I2C_ADDRESS 0x58    // SGP30 I2C ��ַ��7λ��ַ��

// ���� SGP30 �Ĳ������� (�����������Ϊ 0x2008)
#define SGP30_MEASURE_COMMAND 0x2008

// �������ܵĶ���
#define I2C_ACK  0x00  // ACK �ź�
#define I2C_NACK 0x01  // NACK �ź�

#define A_CONST 512
#define SREF_H2 1000  // ����Ĳο��ź�ֵ������ʵ���������
#define SREF_ETHOH 1000  // ����Ĳο��ź�ֵ������ʵ���������

// ��������
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
//// ���� SDA �� SCL ����
//#define SCL_PIN GPIO_Pin_11  // SCL ��Ӧ�� GPIO ����
//#define SDA_PIN GPIO_Pin_12  // SDA ��Ӧ�� GPIO ����
//#define I2C_GPIO_PORT GPIOC  // SCL �� SDA ���ڵ� GPIO �˿�
//
//// ���� I2C �豸��ַ (���� SGP30 �� I2C ��ַ)
//#define SGP30_I2C_ADDRESS 0x58    // SGP30 I2C ��ַ��7λ��ַ��
//
//// ���� SGP30 �Ĳ������� (�����������Ϊ 0x2008)
//#define SGP30_MEASURE_COMMAND 0x2008
//
//// �������ܵĶ���
//#define I2C_ACK  0x00  // ACK �ź�
//#define I2C_NACK 0x01  // NACK �ź�
//
//#define A_CONST 512
//#define SREF_H2 1000  // ����Ĳο��ź�ֵ������ʵ���������
//#define SREF_ETHOH 1000  // ����Ĳο��ź�ֵ������ʵ���������
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
