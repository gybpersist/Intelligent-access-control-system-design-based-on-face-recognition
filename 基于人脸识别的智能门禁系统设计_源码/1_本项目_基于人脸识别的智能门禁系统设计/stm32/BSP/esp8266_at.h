#ifndef __ESP8266_AT_H
#define __ESP8266_AT_H

#include "main.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#define USART2_MAX_RECV_LEN 512
extern uint8_t usart2_txbuf[USART2_MAX_RECV_LEN];
extern uint8_t usart2_rxbuf[USART2_MAX_RECV_LEN];
extern uint8_t usart2_rxone[1];
extern uint16_t usart2_rxcounter;

#define  BUFF_UNIT           512    //缓冲区长度
#define  R_NUM               15     //接收缓冲区个数
#define  T_NUM               10     //发送缓冲区个数  
#define  C_NUM               10     //命令缓冲区个数
#define BYTE0(dwTemp)       (*( char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
	

#define  PRODUCTKEY           "8fhVQ062Bz"                                 //产品ID
#define  PRODUCTKEY_LEN       strlen(PRODUCTKEY)                            //产品ID长度
#define  DEVICENAME           "eq0"                                  //设备名  
#define  DEVICENAME_LEN       strlen(DEVICENAME)                            //设备名长度
#define  DEVICESECRE          "8fe969e6a31d8035dfa5bd8c25e238f0"            //设备秘钥   
#define  DEVICESECRE_LEN      strlen(DEVICESECRE)                           //设备秘钥长度
//oneJson使用的topic 
#define  S_TOPIC_NAME         "$sys/8fhVQ062Bz/eq0/thing/property/post/reply"   //需要订阅的主题  
#define  P_TOPIC_NAME         "$sys/8fhVQ062Bz/eq0/thing/property/post"        //需要发布的主题  
//数据流使用的topic
//#define  S_TOPIC_NAME_DP  "$sys/74gEGTo2s2/eq0/dp/post/json/accepted"
//#define  P_TOPIC_NAME_DP  "$sys/74gEGTo2s2/eq0/dp/post/json"
///sys/k07usPkN7eJ/LyjLamp/thing/service/property/set
extern char ClientID[128];     //外部变量声明，存放客户端ID的缓冲区
extern int  ClientID_len;      //外部变量声明，存放客户端ID的长度
extern char Username[128];     //外部变量声明，存放用户名的缓冲区
extern int  Username_len;	   //外部变量声明，存放用户名的长度
extern char Passward[256];     //外部变量声明，存放密码的缓冲区
extern int  Passward_len;	   //外部变量声明，存放密码的长度
extern char ServerIP[128];     //外部变量声明，存放服务器IP或是域名
extern int  ServerPort;        //外部变量声明，存放服务器的端口号
extern uint8_t ESP8266_Init(void);
extern void ESP8266_Restore(void);

extern void ESP8266_ATSendBuf(uint8_t* buf,uint16_t len);		//向ESP8266发送指定长度数据
extern void ESP8266_ATSendString(char* str);								//向ESP8266模块发送字符串
extern void ESP8266_ExitUnvarnishedTrans(void);							//ESP8266退出透传模式
extern uint8_t ESP8266_ConnectAP(char* ssid,char* pswd);		//ESP8266连接热点
extern uint8_t ESP8266_ConnectServer(char* mode,char* ip,uint16_t port);	//使用指定协议(TCP/UDP)连接到服务器
extern uint8_t ESP8266_USERCFG(char* clientid,char* user,char* pswd);
void IoT_Parameter_Init(void);
uint8_t ESP8266_USERCLIENT(char* clientid);
uint8_t ESP8266_ConnectIOT( char* ip,uint16_t port);
uint8_t ESP8266_Sub( char* topic);
uint8_t ESP8266_Pub( char* topic,char * payload);
#endif
