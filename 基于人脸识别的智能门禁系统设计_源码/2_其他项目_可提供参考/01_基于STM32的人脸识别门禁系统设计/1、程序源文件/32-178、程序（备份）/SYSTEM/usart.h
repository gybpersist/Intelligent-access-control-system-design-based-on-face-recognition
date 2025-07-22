/****************************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V2.7
//修改日期:2020/11/24
//程序功能：封装和简化USART的操作
//V2.3  2020/06/03
//V2.4  2020/07/14  修改函数名称并增加宏定义兼容旧版本
//V2.5  2020/08/27  优化部分程序
//V2.6  2020/10/22  1、删除USARTx_Printf的set与get程序，改为头文件extern
//V2.7  2020/11/24  1、优化部分程序
****************************************************************/
#ifndef __USART_H
#define __USART_H
#include "my_include.h"

#define EN_USART1               1//使能串口1
#define USART1_IT_RXNE          1
#define USART1_IT_TXE           0
#define USART1_IT_IDLE          0
////////////////////////////////////////////
#define EN_USART2               1//使能串口2
#define USART2_IT_RXNE          1
#define USART2_IT_TXE           0
#define USART2_IT_IDLE          0
////////////////////////////////////////////
#define EN_USART3               1//使能串口3
#define USART3_IT_RXNE          1
#define USART3_IT_TXE           0
#define USART3_IT_IDLE          0

#define PIN_USART1_TX           PA9
#define PIN_USART1_RX           PA10
#define PIN_USART2_TX           PA2
#define PIN_USART2_RX           PA3
#define PIN_USART3_TX           PB10
#define PIN_USART3_RX           PB11

#define OnUSART1_ReceiveData(n)     
#define OnUSART2_ReceiveData(n)     
#define OnUSART3_ReceiveData(n)

#if USART1_IT_IDLE>0
#define LEN_BUF_USART1_RX       256
void OnUSART1_ReceiveBuff(u8 *buf,u16 len);
#endif
#if USART2_IT_IDLE>0
#define LEN_BUF_USART3_RX       256
void OnUSART2_ReceiveBuff(u8 *buf,u16 len);
#endif
#if USART3_IT_IDLE>0
#define LEN_BUF_USART3_RX       256
void OnUSART3_ReceiveBuff(u8 *buf,u16 len);
#endif
//增加宏定义兼容旧版本
#define SetPrintfUSART          My_USART_SetPrintfUSART
#define GetPrintfUSART          My_USART_GetPrintfUSART
#define USARTSendByte           My_USART_SendByte
#define USARTSendBytes          My_USART_SendBytes
#define USARTSendString         My_USART_SendString
#define USART_printf            My_USART_printf
#define USARTx_Init             My_USART_Init

extern USART_TypeDef* USART_Printf;

void My_USART_SendByte(USART_TypeDef* USARTx, uint8_t Data);
void My_USART_SendBytes(USART_TypeDef* USARTx, const uint8_t *Data, uint16_t length);
void My_USART_SendString(USART_TypeDef* USARTx, const char *str);
void My_USART_printf(USART_TypeDef* USARTx,const char *format, ...);
void My_USART_Init(USART_TypeDef* USARTx, u32 baud);
u16 My_USART_GetVersion(void);

#endif


