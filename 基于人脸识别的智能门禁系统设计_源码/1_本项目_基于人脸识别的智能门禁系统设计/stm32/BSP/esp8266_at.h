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

#define  BUFF_UNIT           512    //����������
#define  R_NUM               15     //���ջ���������
#define  T_NUM               10     //���ͻ���������  
#define  C_NUM               10     //�����������
#define BYTE0(dwTemp)       (*( char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
	

#define  PRODUCTKEY           "8fhVQ062Bz"                                 //��ƷID
#define  PRODUCTKEY_LEN       strlen(PRODUCTKEY)                            //��ƷID����
#define  DEVICENAME           "eq0"                                  //�豸��  
#define  DEVICENAME_LEN       strlen(DEVICENAME)                            //�豸������
#define  DEVICESECRE          "8fe969e6a31d8035dfa5bd8c25e238f0"            //�豸��Կ   
#define  DEVICESECRE_LEN      strlen(DEVICESECRE)                           //�豸��Կ����
//oneJsonʹ�õ�topic 
#define  S_TOPIC_NAME         "$sys/8fhVQ062Bz/eq0/thing/property/post/reply"   //��Ҫ���ĵ�����  
#define  P_TOPIC_NAME         "$sys/8fhVQ062Bz/eq0/thing/property/post"        //��Ҫ����������  
//������ʹ�õ�topic
//#define  S_TOPIC_NAME_DP  "$sys/74gEGTo2s2/eq0/dp/post/json/accepted"
//#define  P_TOPIC_NAME_DP  "$sys/74gEGTo2s2/eq0/dp/post/json"
///sys/k07usPkN7eJ/LyjLamp/thing/service/property/set
extern char ClientID[128];     //�ⲿ������������ſͻ���ID�Ļ�����
extern int  ClientID_len;      //�ⲿ������������ſͻ���ID�ĳ���
extern char Username[128];     //�ⲿ��������������û����Ļ�����
extern int  Username_len;	   //�ⲿ��������������û����ĳ���
extern char Passward[256];     //�ⲿ�����������������Ļ�����
extern int  Passward_len;	   //�ⲿ�����������������ĳ���
extern char ServerIP[128];     //�ⲿ������������ŷ�����IP��������
extern int  ServerPort;        //�ⲿ������������ŷ������Ķ˿ں�
extern uint8_t ESP8266_Init(void);
extern void ESP8266_Restore(void);

extern void ESP8266_ATSendBuf(uint8_t* buf,uint16_t len);		//��ESP8266����ָ����������
extern void ESP8266_ATSendString(char* str);								//��ESP8266ģ�鷢���ַ���
extern void ESP8266_ExitUnvarnishedTrans(void);							//ESP8266�˳�͸��ģʽ
extern uint8_t ESP8266_ConnectAP(char* ssid,char* pswd);		//ESP8266�����ȵ�
extern uint8_t ESP8266_ConnectServer(char* mode,char* ip,uint16_t port);	//ʹ��ָ��Э��(TCP/UDP)���ӵ�������
extern uint8_t ESP8266_USERCFG(char* clientid,char* user,char* pswd);
void IoT_Parameter_Init(void);
uint8_t ESP8266_USERCLIENT(char* clientid);
uint8_t ESP8266_ConnectIOT( char* ip,uint16_t port);
uint8_t ESP8266_Sub( char* topic);
uint8_t ESP8266_Pub( char* topic,char * payload);
#endif
