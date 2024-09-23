//
// Created by muatea on 2024/9/23.
//

#include "GPS_drv.h"

#define APN ""  // 替换为您的 APN
#define USERNAME ""
#define PASSWORD ""
#define SERVER ""   // OneNet 服务器地址
#define PORT 5683   // 端口号

/*AT 指令发送函数
    sendATCommand() 函数用于发送 AT 指令并等待预期的响应。
    USART_SendString(command) 通过 USART 发送 AT 指令。
    USART_ReceiveLine() 接收模块的返回信息，并检查是否包含期望的响应 response。
    Delay(1) 实现了每次检查响应后的延时，确保模块有足够时间返回信息。
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

/*连接到 OneNet 云
    connectToOneNet() 函数执行了一系列的 AT 指令，用于通过 GPRS 模块连接到 OneNet 云服务器。
    AT+CGATT=1 使能 GPRS 功能。
    AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\" 设置 Bearer 的连接类型为 GPRS。
    AT+SAPBR=3,1,\"APN\",\"your_apn\" 设置 APN（Access Point Name）。
    AT+SAPBR=1,1 打开 Bearer，开始连接。
    AT+CSTT="your_apn","your_username","your_password" 设置 PDP 上下文，提供 APN、用户名和密码。
    AT+CIICR 激活 PDP。
    AT+CIFSR 获取模块的 IP 地址。
    最后，通过 AT+CIPSTART 指令建立 TCP 连接到 OneNet 服务器。
 */
void connectToOneNet(void) {
    sendATCommand("AT+CGATT=1\r\n", "OK", 5000);//使能 GPRS
    sendATCommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n", "OK", 2000); // 设置连接类型
    sendATCommand("AT+SAPBR=3,1,\"APN\",\"" APN "\"\r\n", "OK", 2000); // 设置 APN
    sendATCommand("AT+SAPBR=1,1\r\n", "OK", 5000); // 打开 Bearer
    sendATCommand("AT+CSTT=\"" APN "\",\"" USERNAME "\",\"" PASSWORD "\"\r\n", "OK", 2000); // 设置 PDP 上下文
    sendATCommand("AT+CIICR\r\n", "OK", 5000); // 激活 PDP
    sendATCommand("AT+CIFSR\r\n", "OK", 2000); // 获取 IP 地址

    char connectCmd[256];
    snprintf(connectCmd, sizeof(connectCmd), "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", SERVER, PORT);
    sendATCommand(connectCmd, "CONNECT", 5000); // 发起 TCP 连接
}

/*向 OneNet 发送数据
    sendDataToOneNet() 用于将数据发送到 OneNet。
    通过 AT+CIPSEND 命令，模块准备发送数据。
    当收到模块的 > 提示符后，使用 USART_SendString(data) 将数据发送给服务器。
    使用 \x1A（即 Ctrl+Z）表示数据传输结束。
    最后，等待模块返回 SEND OK 确认数据发送成功。
*/
void sendDataToOneNet(char* data) {
    char sendCmd[256];
    snprintf(sendCmd, sizeof(sendCmd), "AT+CIPSEND\r\n");
    sendATCommand(sendCmd,">",2000);//发送数据准备

    USART_SendString(USART0,data);
    USART_SendString(USART0,"\x1A");
    sendATCommand("","SEND OK",5000);
}

void GPS_Init(){

}