//
// Created by muatea on 2024/9/23.
//

#include "GPS_drv.h"

#define APN ""  // �滻Ϊ���� APN
#define USERNAME ""
#define PASSWORD ""
#define SERVER ""   // OneNet ��������ַ
#define PORT 5683   // �˿ں�

/*AT ָ��ͺ���
    sendATCommand() �������ڷ��� AT ָ��ȴ�Ԥ�ڵ���Ӧ��
    USART_SendString(command) ͨ�� USART ���� AT ָ�
    USART_ReceiveLine() ����ģ��ķ�����Ϣ��������Ƿ������������Ӧ response��
    Delay(1) ʵ����ÿ�μ����Ӧ�����ʱ��ȷ��ģ�����㹻ʱ�䷵����Ϣ��
*/
void sendATCommand(char* command,char* response,uint32_t timeout) {
    USART_SendString(USART0,command);
    uint32_t time = 0;
    char buffer[256] = {0};

    while (time < timeout){
        if (check_USART_ReceiceLine(USART0,buffer,sizeof(buffer))>0){
            if (strstr(buffer,response)!=NULL){
                return;
            }
        }
        time++;
        Delay_s(1);
    }
}

/*���ӵ� OneNet ��
    connectToOneNet() ����ִ����һϵ�е� AT ָ�����ͨ�� GPRS ģ�����ӵ� OneNet �Ʒ�������
    AT+CGATT=1 ʹ�� GPRS ���ܡ�
    AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\" ���� Bearer ����������Ϊ GPRS��
    AT+SAPBR=3,1,\"APN\",\"your_apn\" ���� APN��Access Point Name����
    AT+SAPBR=1,1 �� Bearer����ʼ���ӡ�
    AT+CSTT="your_apn","your_username","your_password" ���� PDP �����ģ��ṩ APN���û��������롣
    AT+CIICR ���� PDP��
    AT+CIFSR ��ȡģ��� IP ��ַ��
    ���ͨ�� AT+CIPSTART ָ��� TCP ���ӵ� OneNet ��������
 */
void connectToOneNet(void) {
    sendATCommand("AT+CGATT=1\r\n", "OK", 5000);//ʹ�� GPRS
    sendATCommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n", "OK", 2000); // ������������
    sendATCommand("AT+SAPBR=3,1,\"APN\",\"" APN "\"\r\n", "OK", 2000); // ���� APN
    sendATCommand("AT+SAPBR=1,1\r\n", "OK", 5000); // �� Bearer
    sendATCommand("AT+CSTT=\"" APN "\",\"" USERNAME "\",\"" PASSWORD "\"\r\n", "OK", 2000); // ���� PDP ������
    sendATCommand("AT+CIICR\r\n", "OK", 5000); // ���� PDP
    sendATCommand("AT+CIFSR\r\n", "OK", 2000); // ��ȡ IP ��ַ

    char connectCmd[256];
    snprintf(connectCmd, sizeof(connectCmd), "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", SERVER, PORT);
    sendATCommand(connectCmd, "CONNECT", 5000); // ���� TCP ����
}

/*�� OneNet ��������
    sendDataToOneNet() ���ڽ����ݷ��͵� OneNet��
    ͨ�� AT+CIPSEND ���ģ��׼���������ݡ�
    ���յ�ģ��� > ��ʾ����ʹ�� USART_SendString(data) �����ݷ��͸���������
    ʹ�� \x1A���� Ctrl+Z����ʾ���ݴ��������
    ��󣬵ȴ�ģ�鷵�� SEND OK ȷ�����ݷ��ͳɹ���
*/
void sendDataToOneNet(char* data) {
    char sendCmd[256];
    snprintf(sendCmd, sizeof(sendCmd), "AT+CIPSEND\r\n");
    sendATCommand(sendCmd,">",2000);//��������׼��

    USART_SendString(USART0,data);
    USART_SendString(USART0,"\x1A");
    sendATCommand("","SEND OK",5000);
}

void GPS_Init(){

}