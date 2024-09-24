#include "iic_drv.h"
#include "systick.h"
#include "stdio.h"

float CO2;

void CO2_IIC_Init(void) {
    // ʹ��GPIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // GPIO��ʼ��
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

// ���� SDA ����Ϊ�ߵ�ƽ
void SDA_High_CO2(void) {
    GPIO_SetBits(I2C_GPIO_PORT, SDA_PIN);
}

// ���� SDA ����Ϊ�͵�ƽ
void SDA_Low_CO2(void) {
    GPIO_ResetBits(I2C_GPIO_PORT, SDA_PIN);
}

// ���� SCL ����Ϊ�ߵ�ƽ
void SCL_High_CO2(void) {
    GPIO_SetBits(I2C_GPIO_PORT, SCL_PIN);
}

// ���� SCL ����Ϊ�͵�ƽ
void SCL_Low_CO2(void) {
    GPIO_ResetBits(I2C_GPIO_PORT, SCL_PIN);
}

// ��ȡ SDA ��״̬
uint8_t SDA_Read_CO2(void) {
    return GPIO_ReadInputDataBit(I2C_GPIO_PORT, SDA_PIN);
}

// ��ʼ�ź�
void CO2_I2C_Start(void) {
    SDA_OutputMode_CO2();
    SCL_High_CO2();
    delay_us(1);  // ȷ��ʱ������Ҫ��
    SDA_Low_CO2();
    delay_us(1);
    SCL_Low_CO2();
}

// ֹͣ�ź�
void CO2_I2C_Stop(void) {
    SDA_OutputMode_CO2();
    SCL_Low_CO2();
    SDA_Low_CO2();
    delay_us(1);
    SCL_High_CO2();
    delay_us(1);
    SDA_High_CO2();
}

// �����ֽ�
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
        delay_us(3);  // ��ʱ��ȷ�����豸����SDA�ϵ�����
        SCL_Low_CO2();
        delay_us(1);
    }
}

// �ȴ�Ӧ���ź�
uint8_t CO2_I2C_WaitAck(void) {
    SDA_InputMode_CO2();
    SCL_High_CO2();
    delay_us(1);
    uint8_t ack = SDA_Read_CO2();
    SCL_Low_CO2();
    return ack == 0 ? 1 : 0;
}

// ���ֽ�����
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

// ����ACK
void CO2_I2C_SendAck(void) {
    SDA_OutputMode_CO2();
    SDA_Low_CO2();
    delay_us(1);
    SCL_High_CO2();
    delay_us(1);
    SCL_Low_CO2();
}

// ����NACK
void CO2_I2C_SendNack(void) {
    SDA_OutputMode_CO2();
    SDA_High_CO2();
    delay_us(1);
    SCL_High_CO2();
    delay_us(1);
    SCL_Low_CO2();
}

// CRCУ��
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

// ��ȡSGP30����
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

// ��ʼ��SGP30
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
//// ���ų�ʼ��
//void CO2_Init(void) {
//    GPIO_InitTypeDef GPIO_InitStructure;
//
//    // ���� GPIO ʱ��
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   // ����AFIOʱ�ӣ�������ӳ��
//
//    // ���� SCL (PB6) �� SDA (PB7) Ϊ��©���
//    GPIO_InitStructure.GPIO_Pin = SCL_PIN | SDA_PIN;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  // ��©���
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStructure);
//
//	// ��ӳ��I2C1��PB8��PB9
//    //GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
//    // ���� SCL �� SDA �ߵ�ƽ
//    GPIO_SetBits(I2C_GPIO_PORT, SCL_PIN | SDA_PIN);
//}
//
//// ���� SDA ����Ϊ�ߵ�ƽ
//void SDA_High_CO2(void) {
//    GPIO_SetBits(I2C_GPIO_PORT,  SDA_PIN);
//}
//
//// ���� SDA ����Ϊ�͵�ƽ
//void SDA_Low_CO2(void) {
//    GPIO_ResetBits(I2C_GPIO_PORT, SDA_PIN);
//}
//
//// ���� SCL ����Ϊ�ߵ�ƽ
//void SCL_High_CO2(void) {
//    GPIO_SetBits(I2C_GPIO_PORT, SCL_PIN);
//}
//
//// ���� SCL ����Ϊ�͵�ƽ
//void SCL_Low_CO2(void) {
//    GPIO_ResetBits(I2C_GPIO_PORT, SCL_PIN);
//}
//// ��ȡ SDA ��״̬
//uint8_t SDA_Read_CO2(void) {
//    return GPIO_ReadInputDataBit(I2C_GPIO_PORT, SDA_PIN);
//}
//
//void CO2_I2C_Start(void) {
//    SDA_High_CO2();
//    SCL_High_CO2();
//    delay_us(1);  // ȷ��ʱ������Ҫ��
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
//        SCL_High_CO2();  // ���� SCL����������
//        delay_us(1);
//        SCL_Low_CO2();
//        delay_us(2);
//    }
//}
//
//
//uint8_t CO2_I2C_WaitAck(void) {
//    SDA_High_CO2();  // �ͷ� SDA
//    delay_us(1);
//    SCL_High_CO2();  // ���� SCL �ȴ����豸�� ACK
//    delay_us(1);
//
//    uint8_t ack = SDA_Read_CO2();  // ��ȡ SDA �ߵ�״̬
//
//    SCL_Low_CO2();  // ���� SCL�������ȴ�
//    return ack == 0 ? 1 : 0;  // ���� 1 ��ʾ�յ� ACK
//}
//
//uint8_t CO2_I2C_ReadByte(void) {
//    uint8_t byte = 0;
//    SDA_High_CO2();  // �ͷ� SDA��׼����ȡ
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
//// CRCУ�麯����ʹ��8-bit����ʽ0x31
//uint8_t calculateCRC(uint8_t* data, int length) {
//    uint8_t crc = 0xFF;  // ��ʼֵ
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
//    // ���� I2C ͨ��
//    CO2_I2C_Start();
//    CO2_I2C_SendByte(SGP30_I2C_ADDRESS << 1);  // ���� I2C ��ַ + д
//    if (!CO2_I2C_WaitAck()) {
//        printf("Address transmission failed!\n");
//        CO2_I2C_Stop();
//        return;
//    }
//
//    // ���Ͷ�ȡ���ݵ����� (���緢�Ͳ�������)
//    CO2_I2C_SendByte(SGP30_MEASURE_COMMAND >> 8);  // ���͸��ֽ�
//    CO2_I2C_WaitAck();
//    CO2_I2C_SendByte(SGP30_MEASURE_COMMAND & 0xFF);  // ���͵��ֽ�
//    CO2_I2C_WaitAck();
//    CO2_I2C_Stop();
//
//    // ��ʱ�ȴ��������
//    delay_ms(50);
//
//    // ��ʼ I2C ��ȡ����
//    CO2_I2C_Start();
//    CO2_I2C_SendByte((SGP30_I2C_ADDRESS << 1) | 0x01);  // ���� I2C ��ַ + ��
//    if (!CO2_I2C_WaitAck()) {
//        printf("Failed to send read request!\n");
//        CO2_I2C_Stop();
//        return;
//    }
//
//    // ��ȡ 6 �ֽ����� (2 �� 16 λ���� + CRC У��)
//    for (int i = 0; i < 6; i++) {
//        buffer[i] = CO2_I2C_ReadByte();
//        if (i < 5) {
//            CO2_I2C_SendAck();  // ���� ACK
//        } else {
//            CO2_I2C_SendNack();  // ���һ���ֽڷ��� NACK
//        }
//    }
//    CO2_I2C_Stop();
//
//    // У��CRC�������ƥ�䣬��������
//    if (calculateCRC(buffer, 2) != buffer[2] || calculateCRC(buffer + 3, 2) != buffer[5]) {
//        printf("CRC check failed!\n");
//        return;
//    }
//
//    // �������ݣ�������� CO2��VOC ֵ��
//    uint16_t co2 = (buffer[0] << 8) | buffer[1];
//    uint16_t voc = (buffer[3] << 8) | buffer[4];
//
//    printf("CO2: %d ppm, VOC: %d ppb\n", co2, voc);
//}
//
//
//void SGP30_Init(void) {
//    // ���� I2C ͨ�Ų����� Init_air_quality ����
//    CO2_I2C_Start();
//    CO2_I2C_SendByte(SGP30_I2C_ADDRESS << 1);  // ���� I2C ��ַ + д
//    if (!CO2_I2C_WaitAck()) {
//        printf("Address transmission failed!\n");
//        CO2_I2C_Stop();
//        return;
//    }
//
//    // ���� Init_air_quality ���� (0x2003)
//    CO2_I2C_SendByte(0x20);  // ���͸��ֽ�
//    CO2_I2C_WaitAck();
//    CO2_I2C_SendByte(0x03);  // ���͵��ֽ�
//    CO2_I2C_WaitAck();
//    CO2_I2C_Stop();
//
//    // ��ʱ2ms���ȴ���ʼ�����
//    delay_ms(2);
//}
