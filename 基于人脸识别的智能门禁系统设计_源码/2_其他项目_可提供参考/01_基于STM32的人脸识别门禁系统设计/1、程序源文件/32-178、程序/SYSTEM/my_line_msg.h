/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ 51开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.0
//修改日期:2020/09/08
//程序功能：获取以回车换行结束的串口数据
//版本：2020/09/08     V1.0    完成基本功能
***********************************************/
#ifndef __MY_LINE_MSG_H
#define __MY_LINE_MSG_H
#include "my_include.h"


#define COUNT_MSG_LINE_MAX                      4//
#define LEN_MSG_LINE_MAX                        32//信息容量
#define USE_BUF_LINE_MESSAGE                    1//是否使用自定义串口缓冲区
/****************************************************************************************/

#if USE_BUF_LINE_MESSAGE>0
#define My_LINEMessage_Receive                  My_LineMessage_SaveToBuffer
#define MY_LINE_BUF_LEN                         128
#else
#define My_LINEMessage_Receive                  My_LineMessage_StateMachine
#endif

typedef struct 
{
    bool enable;
    u8 length;
    char payload[LEN_MSG_LINE_MAX];
}_line_msg_obj;

#if USE_BUF_LINE_MESSAGE>0
void My_LineMessage_SaveToBuffer(u8 dat);
#endif
void My_LineMessage_StateMachine(u8 msgByte);
void My_LineMessage_Process(void);
void OnGetLineMessage(const _line_msg_obj *lineMsgRec);
#endif
