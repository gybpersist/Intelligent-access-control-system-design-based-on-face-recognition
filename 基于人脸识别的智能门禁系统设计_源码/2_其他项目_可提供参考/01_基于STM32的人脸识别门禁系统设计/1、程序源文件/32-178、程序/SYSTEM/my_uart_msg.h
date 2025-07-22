/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ 51������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V1.3
//�޸�����:2020/09/08
//�����ܣ���װ�ͼ�ͨ�õĲ���
//�汾��2020/02/17     V1.0    ��ɻ�������
//�汾��2020/02/24     V1.1    �Ż����ֹ���
//�汾��2020/07/12     V1.2    1�����ӻ���������
                               2���޸���Ϣ�ص���������
//�汾��2020/09/08     V1.3    1�������Ż�
***********************************************/
#ifndef __MY_UART_MSG_H
#define __MY_UART_MSG_H
#include "my_include.h"

/************************************
�Զ���Э�飺
����      ��ʼ�ֽ�    ��������    �����ֽ�
��ʶ      start       data        end
ʾ��      0x2a        0x50 0x78   0x23
***************************************/
#define COUNT_MSG_UART_MAX                      4//
#define LEN_MSG_UART_MAX                        32//��Ϣ����
#define MSG_START                               0x2a//���ڷ���������ʼ���ֽ�
#define MSG_END                                 0x23//���ڷ���������ʼ���ֽ�
#define USE_BUF_UART_MESSAGE                    0//�Ƿ�ʹ���Զ��崮�ڻ�����
/****************************************************************************************/

#if USE_BUF_UART_MESSAGE>0
#define My_UARTMessage_Receive                  My_UartMessage_SaveToBuffer
#define MY_UART_BUF_LEN                         128
#else
#define My_UARTMessage_Receive                  My_UartMessage_StateMachine
#endif

typedef struct 
{
    bool enable:1;
    u16 length:15;
    char payload[LEN_MSG_UART_MAX];
}_uart_msg_obj;

#if USE_BUF_UART_MESSAGE>0
void My_UartMessage_SaveToBuffer(u8 dat);
#endif
void My_UartMessage_Send(USART_TypeDef* USARTx, _uart_msg_obj *msgPtr);
void My_UartMessage_StateMachine(u8 msgByte);
void My_UartMessage_Process(void);
void OnGetUartMessage(const _uart_msg_obj *uartMsgRec);
#endif
