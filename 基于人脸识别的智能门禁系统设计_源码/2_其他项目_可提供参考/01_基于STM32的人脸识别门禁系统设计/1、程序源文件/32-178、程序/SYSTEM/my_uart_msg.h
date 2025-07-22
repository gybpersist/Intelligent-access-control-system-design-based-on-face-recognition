/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ 51开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.3
//修改日期:2020/09/08
//程序功能：封装和简化通用的操作
//版本：2020/02/17     V1.0    完成基本功能
//版本：2020/02/24     V1.1    优化部分功能
//版本：2020/07/12     V1.2    1、增加缓冲区功能
                               2、修改消息回调函数定义
//版本：2020/09/08     V1.3    1、程序优化
***********************************************/
#ifndef __MY_UART_MSG_H
#define __MY_UART_MSG_H
#include "my_include.h"

/************************************
自定义协议：
定义      起始字节    数据内容    结束字节
标识      start       data        end
示例      0x2a        0x50 0x78   0x23
***************************************/
#define COUNT_MSG_UART_MAX                      4//
#define LEN_MSG_UART_MAX                        32//信息容量
#define MSG_START                               0x2a//串口发送数据起始高字节
#define MSG_END                                 0x23//串口发送数据起始低字节
#define USE_BUF_UART_MESSAGE                    0//是否使用自定义串口缓冲区
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
