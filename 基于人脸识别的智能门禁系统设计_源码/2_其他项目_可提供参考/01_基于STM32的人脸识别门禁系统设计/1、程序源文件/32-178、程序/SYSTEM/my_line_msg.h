/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ 51������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V1.0
//�޸�����:2020/09/08
//�����ܣ���ȡ�Իس����н����Ĵ�������
//�汾��2020/09/08     V1.0    ��ɻ�������
***********************************************/
#ifndef __MY_LINE_MSG_H
#define __MY_LINE_MSG_H
#include "my_include.h"


#define COUNT_MSG_LINE_MAX                      4//
#define LEN_MSG_LINE_MAX                        32//��Ϣ����
#define USE_BUF_LINE_MESSAGE                    1//�Ƿ�ʹ���Զ��崮�ڻ�����
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
