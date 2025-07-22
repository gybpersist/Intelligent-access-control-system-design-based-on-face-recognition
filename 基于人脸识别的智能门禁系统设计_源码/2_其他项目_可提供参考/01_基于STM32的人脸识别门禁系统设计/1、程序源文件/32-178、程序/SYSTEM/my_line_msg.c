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
#include "my_line_msg.h"

static _line_msg_obj recMsg[COUNT_MSG_LINE_MAX];

#if USE_BUF_LINE_MESSAGE>0
static u8 recBuffer[MY_LINE_BUF_LEN];
static u8 index_save_buf = 0;
static u8 index_read_buf = 0;
void My_LineMessage_SaveToBuffer(u8 dat)
{
    recBuffer[index_save_buf]=dat;
    index_save_buf++;
    if(index_save_buf==ArrayCount(recBuffer))
    {
        index_save_buf=0;
    }
}
#endif
/****************************************************************
�������ܣ����ݽ���״̬�����ڴ����жϵ���
msgByte�����ڽ��յ����ֽ�����
****************************************************************/
void My_LineMessage_StateMachine(u8 msgByte)
{
    static u8 index_msg_read=0;
    static u8 index_dat=0;
    static u8 msgByte_last=0;
    if(msgByte_last=='\r' && msgByte=='\n')
    {
        recMsg[index_msg_read].payload[index_dat-1] = 0;
        recMsg[index_msg_read].length = index_dat-1;
        recMsg[index_msg_read].enable = true;
        index_dat = 0;
        index_msg_read++;//ָ����һ����Ϣ
        index_msg_read %= COUNT_MSG_LINE_MAX;//��������һ������ת����һ������ֹ����Խ��
    }
    else if(index_dat<=LEN_MSG_LINE_MAX)
    {
        recMsg[index_msg_read].payload[index_dat] = msgByte;
        index_dat++;
    }
    msgByte_last=msgByte;
}
/****************************************************************
�������ܣ�������յ����ݣ����������е���
****************************************************************/
void My_LineMessage_Process(void)
{
    static u8 index_msg_process=0;
    u8 i;

#if USE_BUF_LINE_MESSAGE>0
    while(index_read_buf!=index_save_buf)
    {
        My_LineMessage_StateMachine(recBuffer[index_read_buf]);
        index_read_buf++;
        if(index_read_buf==ArrayCount(recBuffer))
        {
            index_read_buf=0;
        }
    }
#endif
    if(recMsg[index_msg_process].enable == true)//������յ���һ������������
    {
        OnGetLineMessage(&recMsg[index_msg_process]);
        recMsg[index_msg_process].enable = false;
        memset(recMsg[index_msg_process].payload,0,recMsg[index_msg_process].length);
        //ָ����һ����Ϣ
        index_msg_process++;
        if(index_msg_process==COUNT_MSG_LINE_MAX)
        {
            index_msg_process=0;
        }
    }
}
/****************************************************************
�������ܣ���Ϣ���մ���ص�����
�˴�Ϊ������պ������������������ض���ʵ��
lineMsgRec�����յ�����Ϣ��ָ��
****************************************************************/
void __weak OnGetLineMessage(const _line_msg_obj *lineMsgRec)
{

}

